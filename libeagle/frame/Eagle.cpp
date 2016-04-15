#include <sys/wait.h>
#include <unistd.h>
#include <google/protobuf/stubs/common.h>

#include "Eagle.h"
#include "ShareMem.h"
#include "Define.h"
#include "EagleNode.h"
#include "Log.h"
#include "ProcessManager.h"
#include "ChildSigManager.h"
#include "MasterSigManager.h"
#include "ProcessSem.h"
#include "EagleTime.h"
#include "Timer.h"
#include "StrUtil.h"
#include "Proctitle.h"
#include "PropertiesParser.h"
#include "EpollManager.h"
#include "SelectManager.h"
#include "AcceptHandler.h"
#include "MessageHandlerFactory.h"

namespace eagle
{

#define OP_STOP "stop"
#define OP_RELOAD "reload"

namespace
{
ShareMem &shareMem = ShareMemI::instance();
EagleTime &eagleTime = EagleTimeI::instance();

void startAcceptLoop(void *param)
{
    EagleI::instance().runWorker(0);
}

void stopWorker(void *param)
{
    EagleI::instance().stopWorker();
}
}

__attribute__((destructor)) void destoryEagle()  
{ 
    google::protobuf::ShutdownProtobufLibrary();
}

Eagle::Eagle() : m_servers(NULL), m_properties(NULL), 
    m_acceptManager(NULL), m_receiveManager(NULL)
{
}

Eagle::~Eagle()
{
    if (NULL != m_servers)
    {
        Server *server = *m_servers;
        for (int i = 0; NULL != server; server = *(m_servers + ++i))
        {
            delete server;
        }
        delete [](char *)m_servers;
    }
    if (NULL != m_properties) delete m_properties;
    if (NULL != m_acceptManager) delete m_acceptManager;
    if (NULL != m_receiveManager) delete m_receiveManager;
}

int Eagle::checkDir()
{
    char buf[MAX_FILENAME_LEN];
    int ret = EG_FAILED;

    for (; ; )
    {
        snprintf(buf, MAX_FILENAME_LEN, "%s/%s", m_program.prefix, CONF_DIR);
        if (!File::isExist(buf)) break;

        snprintf(buf, MAX_FILENAME_LEN, "%s/%s", m_program.prefix, LOG_DIR);
        if (!File::isExist(buf)) break;

        ret = EG_SUCCESS;
        break;
    }

    if (EG_SUCCESS != ret) 
    {
        printf("directory %s isn't exist\n", buf);

        return EG_FAILED;
    }

    return EG_SUCCESS;
}

void Eagle::initLog()
{
    char buf[MAX_FILENAME_LEN];

    snprintf(buf, MAX_FILENAME_LEN, "%s/logs/%s.log", m_program.prefix, 
            m_program.name);
    g_sysLog = new Log(buf, m_program.logLevel);
}

int Eagle::writePid()
{
    char buf[MAX_FILENAME_LEN];
    snprintf(buf, MAX_FILENAME_LEN, "%s/%s%s.pid", m_program.prefix, 
            m_program.name, m_program.ver);

    pid_t pid;
    int len = sizeof(pid);
    File file(buf, File::RDWR_CREATE_RESET);

    if (file.readByOffset((uint8_t *)&pid, len, 0) == EG_FAILED)
    {
        pid = 0;
    }
    snprintf(buf, MAX_FILENAME_LEN, "/proc/%d", pid);
    if (File::isExist(buf)) 
    {
        ERRORLOG2("%s %s has ran", m_program.name, m_program.ver);

        return EG_FAILED;
    }

    pid = getpid();
    file.writeByOffset((const uint8_t *)&pid, len, 0);

    return EG_SUCCESS;
}

int Eagle::readPid()
{
    char buf[MAX_FILENAME_LEN];
    snprintf(buf, MAX_FILENAME_LEN, "%s/%s%s.pid", m_program.prefix, 
            m_program.name, m_program.ver);

    pid_t pid;
    File file(buf, File::RDWR);

    if (file.readByOffset((uint8_t *)&pid, sizeof(pid_t), 0) == EG_FAILED)
        return 0;

    return pid;
}

int Eagle::delPid()
{
    char buf[MAX_FILENAME_LEN];
    snprintf(buf, MAX_FILENAME_LEN, "%s/%s%s.pid", m_program.prefix, 
            m_program.name, m_program.ver);

    return File::del(buf);
}

void Eagle::printPrompt(const char *option)
{
    printf("invalid option: \"%s\"\n" \
            "get help : %s -h or %s -?\n",
            option, m_program.name, m_program.name);
}

void Eagle::printVer()
{
    printf("%s version : %s\n", m_program.name, m_program.ver);
}

void Eagle::printUsage()
{
    printf("Usage: %s [-?hv] [-s signal] [-p prefix]\n" \
           "Options:\n" \
           "  -?,-h         : this help\n" \
           "  -v            : show version and exit\n" \
           "  -s signal     : send signal to a master process: reload, stop\n" \
           "  -p prefix     : set prefix path\n", m_program.name);
}

int Eagle::initServers(std::string &ip, std::map<std::string, int> &serverMap)
{
    Server *server;
    Socket *socket;
    std::map<std::string, int>::iterator iter;
    int len = serverMap.size() + 1;

    m_servers = (Server **)new char[sizeof(Server *) * len];
    *(m_servers + --len) = NULL;
    for (iter = serverMap.begin(); iter != serverMap.end(); ++iter)
    {
        socket = new Socket(ip.c_str(), iter->second, 1);
        if (!socket->isAvailable())
        {
            delete socket;
            ERRORLOG2("create socket %s:%d failed.", ip.c_str(), iter->second);

            return EG_FAILED;
        }
        server = new Server(iter->first.c_str(), iter->second, socket);
        *(m_servers + --len) = server;
    }

    return EG_SUCCESS;
}

int Eagle::initProcess()
{
    int ret;
    uint8_t *isStop;
    int processNum;
    std::string ip;
    std::map<std::string, int> serverMap;

    if (PropertiesParser::parseProProperties(ip, serverMap))
        return EG_FAILED;

    if (initServers(ip, serverMap) != EG_SUCCESS)
        return EG_FAILED;

    processNum = m_program.processNum + 1;
    ProcessSem sem(processNum + 1);
    isStop = (uint8_t *)shareMem.calloc(1);
    *isStop = 0;
    ret = ProcessManagerI::instance().spawn(processNum);
    if (ret == EG_CHILD)
    {
        ProcessManagerI::del();
        sem.wait();
        if(*isStop) ret = EG_FAILED;
        shareMem.free(isStop);

        if (ret == EG_CHILD && NODE_SERVER_INDEX == m_index)
        {
            EagleNodeI::instance().addServer(serverMap); 
        }

        return ret;
    }

    if (EG_FAILED == ret) 
    {
        *isStop = 1;
        ERRORLOG("spawnprocess err");
    }
    sem.op(processNum);
    shareMem.free(isStop);

    return ret;
}

void Eagle::initWorker()
{
    char buf[48];
    Socket *unixSocket;
    AcceptHandler *handler;
    Server *server = *m_servers;
    CallBack cb(::eagle::stopWorker);

    m_acceptManager = new SelectManager(1);
    m_receiveManager = new EpollManager(WORKER_THREAD_NUM);
    for (int i = 0; NULL != server; ++i, server = *(m_servers + i))
    {
        handler = new AcceptHandler(m_receiveManager, server->socket, server->port, m_index);
        handler->unlock();
        MessageHandlerFactoryI::instance().registerHandler(server->port, server->name);
        m_acceptManager->registerEvent(READ, handler);
        snprintf(buf, sizeof(buf), "/tmp/eagle%d.sock", server->port);
        unixSocket = new Socket(buf, 1);
        if (!unixSocket->isAvailable())
        {
            delete unixSocket;
            ERRORLOG1("create unix socket %s failed.", buf);
        }else
        {
            handler = new AcceptHandler(m_receiveManager, unixSocket, server->port, m_index);
            m_acceptManager->registerEvent(READ, handler);
        }
        server->socket = NULL;
        delete server;
    }
    delete [](char *)m_servers;
    m_servers = NULL;
    ChildSigManagerI::instance().init(cb);

    cb = CallBack(startAcceptLoop);
    Thread thread(cb);
}

void Eagle::initMaster()
{
    eagleTime.autoUpdate();
    MasterSigManagerI::instance().init();
    MasterSigManagerI::instance().block();
}

void Eagle::cleanMaster()
{
    MasterSigManagerI::instance().unBlock();
    MasterSigManagerI::instance().clean();
    MasterSigManagerI::del();
    ProcessManagerI::del();
    TimerI::del();
}

void Eagle::runWorker(const int isMasterThread)
{
    if (!isMasterThread)
    {
        m_acceptManager->loop();

        return;
    }

    initWorker();
    m_receiveManager->loop(); 
}

void Eagle::stopWorker()
{
    m_acceptManager->stopLoop();
    m_receiveManager->stopLoop(); 
}

int Eagle::masterCycle()
{
    sigset_t set;
    ProcessManager::Status status;

    initMaster();
    for (;;)
    {
        sigsuspend(&set);
        status = ProcessManagerI::instance().getStatus();

        if (ProcessManager::QUIT == status)
        {
            ProcessManagerI::instance().waitQuit();

            return EG_EXIT;
        }

        if (ProcessManager::SPAWN == status)
        {
            TimerI::instance().pause();
            if (ProcessManagerI::instance().reSpawn() == EG_SUCCESS)
            {
                cleanMaster();

                return EG_SUCCESS;
            }

            TimerI::instance().start();
        }
    }

    return EG_EXIT;
}

int Eagle::init(const int argc, char *const *argv, const char *ver)
{
    int ret;
    Proctitle proctitle;

    proctitle.init(argc, argv);
    StrUtil::copy(m_program.ver, ver);
    ret = parseOptions(argc, argv);
    if (EG_SUCCESS != ret) return EG_FAILED;

    if (checkDir() != EG_SUCCESS) return EG_FAILED;

    initLog();
    if (writePid() == EG_FAILED) return EG_FAILED;

    ret = initProcess();

    if (EG_PARENT == ret) 
    {
        proctitle.setMaster(argc, argv, "%s: master", m_program.name);
        ret = masterCycle();
        if (EG_EXIT == ret) delPid();
    }

    if (ret != EG_CHILD) return ret;

    if (NODE_SERVER_INDEX == m_index)
    {
        proctitle.setWorker(argc, argv, "%s: nodeserver", m_program.name);
        EagleNodeI::instance().run();

        return EG_EXIT;
    }

    proctitle.setWorker(argc, argv, "%s: worker-%d", m_program.name, m_index);

    return EG_SUCCESS;
}

int Eagle::sendSignal(const char *signal)
{
    int sig;
    pid_t pid = readPid();

    if (0 == pid)
    {
        ERRORLOG1("no %s process", m_program.name);

        return EG_FAILED;
    }

    if (strcmp(OP_STOP, signal) == 0)
    {
        sig = SIGTERM;
    }else if (strcmp(OP_STOP, signal) == 0)
    {
        sig = SIGUSR1;
    }
    kill(pid, sig);

    return EG_SUCCESS;
}

int Eagle::parseOptions(const int argc, char *const *argv)
{
    int i;
    char *sig;
    int ret = EG_SUCCESS;
    char *p = rindex(argv[0], '/');

    if (NULL != p)
    {
        StrUtil::copy(m_program.name, p + 1);
    }else
    {
        StrUtil::copy(m_program.name, argv[0]);
    }

    if (argc < 2)
    {
        printf("must have start option\n" \
            "get help : %s -h or %s -?\n",
            m_program.name, m_program.name);

        return EG_FAILED;
    }

    for (i = 1; i < argc; ++i)
    {

        p = argv[i];

        if (*p++ != '-') 
        {
            printPrompt(argv[i]);

            return EG_FAILED;
        }

        switch (*p++)
        {
            case '?':
            case 'h':
                printUsage();

                return EG_SHOW_HELP;
            case 'v':
                printVer(); 

                return EG_SHOW_VER;
            case 's':
                if (argv[++i]) 
                {
                    sig = argv[i];

                    ret = EG_SEND_SIGNAL;
                    break;
                }

                printf("option \"-s\" requires signal name\n");

                return EG_FAILED;
            case 'p':
                if (argv[++i]) 
                {
                    if (*(argv[i] + strlen(argv[i]) - 1) == '/')
                    {
                        *(argv[i] + strlen(argv[i]) - 1) = 0;
                    }
                    StrUtil::copy(m_program.prefix, argv[i]);

                    break;
                }
                printf("option \"-p\" requires directory name\n");

                return EG_FAILED;
            default:
                printPrompt(argv[i]);

                return EG_FAILED;
        }
    }

    if (NULL == m_program.prefix)
    {
        printf("must set prefix\n" \
            "get help : %s -h or %s -?\n",
            m_program.name, m_program.name);

        return EG_FAILED;

    }

    if (EG_SEND_SIGNAL == ret)
    {
        sendSignal(sig);
    }

    return ret;
}

}

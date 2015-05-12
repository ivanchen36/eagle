#include <sys/wait.h>
#include <unistd.h>

#include "Eagle.h"
#include "Log.h"
#include "ProcessManager.h"
#include "ChildSigManager.h"
#include "MasterSigManager.h"
#include "ProcessSem.h"
#include "EagleTime.h"

int g_isReceiveSigQuit = 0;

Eagle::Eagle()
{

}

Eagle::~Eagle()
{

}

int Eagle::spawnChildProcess(const int processNum)
{
    int ret;
    ProcessSem sem(processNum + 1);

    ret = ProcessManagerI::instance().spawnProcess(processNum);
    if (ret == 0)
    {
        sem.wait();

        return 0;
    }

    if (ret > 0)
    {
        EagleTimeI::instance().autoUpdate();
        sem.op(processNum);

        return 1;
    }

    //todo
    return -1;
}

void Eagle::init(const CallBack &notifyQuitCb)
{
    pid_t pid;
    int processNum = 4;
    int ret = spawnChildProcess(processNum);

    if (ret < 0)
    {
        ProcessManagerI::instance().quitAll(); 

        //todo
        return;
    }

    if (0 == ret)
    {
        ChildSigManagerI::instance().init(notifyQuitCb);
        DEBUGLOG("start child");

        return;
    }

    MasterSigManagerI::instance().init();
    while (!g_isReceiveSigQuit)
    {
        DEBUGLOG("wait sigquit");
        sleep(1);
    }

    while (processNum > 0)
    {
        pid = waitpid(-1, NULL, 0);
        if (pid > 0)
        {
            DEBUGLOG("child quit");
            processNum--;
        }else
        {
            DEBUGLOG("wait quit");
            sleep(1);
        }
    }
    exit(0);
}

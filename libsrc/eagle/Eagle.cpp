#include <sys/wait.h>
#include <unistd.h>

#include "Eagle.h"
#include "Log.h"
#include "ProcessManager.h"
#include "ChildSigManager.h"
#include "MasterSigManager.h"
#include "ProcessSem.h"
#include "EagleTime.h"
#include "Timer.h"

int EagleAttr::index = 0;
int EagleAttr::status = 0;

int Eagle::spawnChildProcess()
{
    int ret;
    uint8_t *isStop;
    int processNum = 4;
    ProcessSem sem(processNum + 1);

    isStop = (uint8_t *)ShareMemI::instance().calloc(1);
    ret = ProcessManagerI::instance().spawn(processNum);
    if (ret == 0)
    {
        sem.wait();
        if(*isStop) ret = -1;
        ShareMemI::instance().free(isStop);

        return ret;
    }

    if (ret < 0) 
    {
        *isStop = 1;
        ERRORLOG("spawnprocess err");
    }
    sem.op(processNum);
    ShareMemI::instance().free(isStop);

    return ret;
}

void Eagle::childInit(const CallBack &notifyQuitCb)
{
    ChildSigManagerI::instance().init(notifyQuitCb);
}

void Eagle::masterInit()
{
    EagleTimeI::instance().autoUpdate();
    MasterSigManagerI::instance().init();
    MasterSigManagerI::instance().block();
}

void Eagle::masterClean()
{
    TimerI::del();
    MasterSigManagerI::instance().unBlock();
    MasterSigManagerI::instance().clean();
}

int Eagle::masterCycle()
{
    sigset_t set;
    ProcessManager::Status status;

    masterInit();
    for (;;)
    {
        sigsuspend(&set);
        status = ProcessManagerI::instance().getStatus();

        if (ProcessManager::QUIT == status)
        {
            ProcessManagerI::instance().waitQuit();

            return -1;
        }

        if (ProcessManager::SPAWN == status)
        {
            TimerI::instance().pause();
            if (ProcessManagerI::instance().reSpawn() == 0)
            {
                masterClean();

                return 0;
            }
            TimerI::instance().start();
        }
    }
}

int Eagle::init(const CallBack &notifyQuitCb)
{
    pid_t pid;
    int ret = spawnChildProcess();

    if (ret > 0) ret = masterCycle();

    if (ret < 0) return -1;

    childInit(notifyQuitCb);

    return 0;
}

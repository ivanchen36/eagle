#include <sys/wait.h>
#include <unistd.h>

#include "Eagle.h"
#include "Log.h"
#include "ProcessManager.h"
#include "ChildSigManager.h"
#include "MasterSigManager.h"
#include "ProcessSem.h"

int g_isReceiveSigQuit = 0;

Eagle::Eagle()
{

}

Eagle::~Eagle()
{

}

void Eagle::init(const CallBack &notifyQuitCb)
{
    pid_t pid;
    int processNum = 4;
    ProcessSemPtr semPtr = new ProcessSem();

    int ret = ProcessManagerI::instance().spawnProcess(processNum);
    if (ret < 0)
    {
        ProcessManagerI::instance().quitAll(); 

        //todo
        return;
    }
    if (0 == ret)
    {
        ChildSigManagerI::instance().init(notifyQuitCb);
        semPtr->wait();
        DEBUGLOG("spawn child");

        return;
    }

    MasterSigManagerI::instance().init();

    while (!g_isReceiveSigQuit)
    {
        DEBUGLOG("wait sigquit");
        //semPtr->op(processNum);
        sleep(1);
    }

    while (processNum > 0)
    {
        DEBUGLOG("wait quit");
        pid = waitpid(-1, NULL, 0);
        if (pid > 0)
        {
            processNum--;
        }else
        {
            sleep(1);
        }
    }
    exit(0);
}

void Eagle::destroy()
{
}

#include <sys/wait.h>
#include <unistd.h>

#include "Eagle.h"
#include "Log.h"
#include "ProcessManager.h"
#include "ChildSigManager.h"
#include "MasterSigManager.h"

int g_isQuit = 0;

Eagle::Eagle()
{

}

Eagle::~Eagle()
{

}

void Eagle::init(NotifyQuitFunc func)
{
    pid_t pid;
    int processNum = 4;

    int ret = ProcessManagerI::instance().spawnProcess(processNum);
    if (ret < 0)
    {
        ProcessManagerI::instance().quitAllChild(); 

        //todo
        return;
    }
    if (0 == ret)
    {
        ChildSigManagerI::instance().init(func);
        DEBUGLOG("spawn child");

        return;
    }

    MasterSigManagerI::instance().init();

    while (0 == g_isQuit)
    {
        DEBUGLOG("wait sigquit");
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

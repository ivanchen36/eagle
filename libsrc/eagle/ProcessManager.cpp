#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

#include "ProcessManager.h"
#include "Log.h"

ProcessManager::ProcessManager() : childList(0)
{

}

ProcessManager::~ProcessManager()
{

}

int ProcessManager::spawnProcess(const int processNum)
{
    pid_t pid = 1;
    int num = processNum;

    while (num-- > 0 && (pid = fork()) > 0)
    {
        childList.push_back(pid);
    }
    if (0 == num) return 0;

    if (pid < 0)
    {
        ERRORLOG1("fork err, %s", strerror(errno));

        return -1;
    }

    if (0 == pid) return 0;

    return 1;
}

int ProcessManager::quitChild(const int pid)
{
    if (0 == kill(pid, SIGQUIT)) return 0;

    ERRORLOG2("kill %d err, %s", pid, strerror(errno));

    return -1;
}

void ProcessManager::quitAllChild()
{
    std::vector<pid_t>::const_iterator it;

    for(it = childList.begin(); it != childList.end(); ++it)
    {
        quitChild(*it);
    }
}

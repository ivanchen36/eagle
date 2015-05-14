#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>

#include "ProcessManager.h"
#include "Log.h"

ProcessManager::ProcessManager()
{
}

ProcessManager::~ProcessManager()
{

}


int ProcessManager::reSpawn()
{
    int pid;
    int num = m_processNum - m_processList.size();
    while (--num >= 0 && (pid = fork()) > 0)
    {
        m_processList.push_back(pid);
    }

    if (0 == pid) return 0;

    return 1;
}

int ProcessManager::spawn(int &processNum)
{
    int pid = 1;
    int num = processNum;


    while (--num >= 0 && (pid = fork()) > 0)
    {
        m_processList.push_back(pid);
    }

    if (0 == pid) return 0;

    if (-1 == num) 
    {
        m_processNum = processNum;

        return 1;
    }

    ERRORLOG1("fork err, %s", strerror(errno));
    processNum = m_processList.size();

    return -1;
}

void ProcessManager::check()
{
    int pid;
    int status;

    while (m_processList.size() > 0 && 
            (pid = waitpid(-1, &status, WNOHANG)) > 0)
    {
        INFOLOG1("process %d exit.", pid);
        m_processList.erase(std::remove(m_processList.begin(), 
                    m_processList.end(), pid));
    }

    if (pid < 0) ERRORLOG1("waitpid err, %s", strerror(errno));
}

void ProcessManager::waitQuit()
{
    while (0 != m_processList.size())
    {
        if (sched_yield() != 0) 
            ERRORLOG1("sched_yield err, %s", strerror(errno));
    }
}

int ProcessManager::stop(const int pid)
{
    if (0 == kill(pid, SIGQUIT)) return 0;

    ERRORLOG2("kill %d err, %s", pid, strerror(errno));

    return -1;
}

void ProcessManager::quit()
{
    std::vector<pid_t>::const_iterator it;

    m_processNum = 0;
    for(std::vector<pid_t>::const_iterator it = m_processList.begin(); 
            it != m_processList.end(); ++it)
    {
        stop(*it);
    }
}

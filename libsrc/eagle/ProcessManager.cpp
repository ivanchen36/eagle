#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>

#include "ProcessManager.h"
#include "Eagle.h"
#include "Log.h"

ProcessManager::ProcessManager()
    : m_processHead(NULL), m_aliveNum(0), 
    m_processNum(0)
{
}

ProcessManager::~ProcessManager()
{
    ProcessNode *process = NULL;

    while (NULL != m_processHead)
    {
        process = m_processHead->next;
        delete m_processHead;
        m_processHead = process;
    }
}


int ProcessManager::reSpawn()
{
    int pid;
    int index = 0;
    ProcessNode *process = m_processHead;
    int num = m_processNum - m_aliveNum;

    for (; NULL != process && 0 != process->pid; 
            process = process->next);
    if (NULL != process) index = process->index;
    while (--num >= 0 && (pid = fork()) > 0)
    {
        ++m_aliveNum;
        if (NULL == process) 
        {
            ERRORLOG("can't find dead process index");
            continue;
        }

        process->pid = pid;
        for (; NULL != process && 0 != process->pid; 
                process = process->next);
        if (NULL != process) index = process->index;
    }

    if (0 == pid)
    {
        EagleAttr::index = index;

        return 0;
    }

    return 1;
}

int ProcessManager::spawn(int &processNum)
{
    int pid;
    int num = 0;
    ProcessNode *process = NULL;

    while (++num <= processNum && (pid = fork()) > 0)
    {
        process = new ProcessNode(num, pid, process);
        ++m_aliveNum;
    }

    if (0 == pid)
    {
        EagleAttr::index = num;

        return 0;
    }

    if (pid > 0) 
    {
        m_processHead = process;
        m_processNum = m_aliveNum;

        return 1;
    }

    ERRORLOG1("fork err, %s", strerror(errno));
    processNum = m_aliveNum;

    return -1;
}

void ProcessManager::deleteProcess(const int pid)
{
    ProcessNode *process = m_processHead;

    for(; NULL != process; process = process->next)
    {
        if (pid == process->pid)
        {
            --m_aliveNum;
            process->pid = 0;

            return;
        }
    }
}

void ProcessManager::check()
{
    int pid;
    int status;

    while (m_aliveNum > 0 && (pid = waitpid(-1, &status, WNOHANG)) > 0)
    {
        INFOLOG1("process %d exit.", pid);
        deleteProcess(pid);
    }

    if (pid < 0) ERRORLOG1("waitpid err, %s", strerror(errno));
}

void ProcessManager::waitQuit()
{
    while (0 != m_aliveNum)
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
    ProcessNode *process = m_processHead;

    m_processNum = 0;
    for(; NULL != process; process = process->next)
    {
        stop(process->pid);
    }
}

#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>

#include "ProcessManager.h"
#include "Define.h"
#include "Eagle.h"
#include "Log.h"

ProcessManager::ProcessManager()
    :  m_aliveNum(0), m_processNum(0), m_processHead(NULL)
{
}

ProcessManager::~ProcessManager()
{
}

int ProcessManager::reSpawn()
{
    int pid;
    int index;
    ProcessNode *process = m_processHead;
    int num = m_processNum - m_aliveNum;

    if (num == 0) return EG_FAILED;
    for (; NULL != process && 0 != process->pid; 
            process = process->next);
    if (NULL != process) return EG_FAILED;

    index = process->index;
    while (--num >= 0 && (pid = fork()) > 0)
    {
        ++m_aliveNum;

        process->pid = pid;
        for (; NULL != process && 0 != process->pid; 
                process = process->next);
        if (NULL != process) return EG_FAILED;

        index = process->index;
    }

    if (0 == pid)
    {
        EagleI::instance().setIndex(index);

        return EG_CHILD;
    }

    return EG_PARENT;
}

int ProcessManager::spawn(int &processNum)
{
    int pid;
    int num = -1;
    int index = m_processNum;
    ProcessNode *process = m_processHead;

    while (++num < processNum && (pid = fork()) > 0)
    {
        index = num + m_processNum + 1;
        process = new ProcessNode(index, pid, process);
    }

    if (0 == pid)
    {
        EagleI::instance().setIndex(index + 1);

        return EG_CHILD;
    }

    if (pid > 0) 
    {
        m_processHead = process;
        m_processNum += num;
        m_aliveNum += num;

        return EG_PARENT;
    }

    ERRORLOG1("fork err, %s", strerror(errno));
    processNum = num;

    return EG_FAILED;
}

void ProcessManager::deleteProcess(const int pid)
{
    ProcessNode *process = m_processHead;

    --m_aliveNum;
    for(; NULL != process; process = process->next)
    {
        if (pid == process->pid)
        {
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
    if (0 == kill(pid, SIGQUIT)) return EG_SUCCESS;

    ERRORLOG2("kill %d err, %s", pid, strerror(errno));

    return EG_FAILED;
}

void ProcessManager::quit(const int processNum)
{
    ProcessNode *process;

    m_processNum = processNum == 0 ? 0 : m_processNum - processNum;
    for (process = m_processHead; NULL != process; process = m_processHead)
    {
        if (m_processNum >= process->index) break;

        stop(process->pid);
        m_processHead = m_processHead->next;
        delete process;
    }
}

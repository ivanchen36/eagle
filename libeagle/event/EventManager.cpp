#include "EventManager.h"

namespace eagle
{

EventManager::EventManager(const int workerNum)
    : m_workerNum(workerNum), m_curWorker(-1), m_isStop(0), m_isOverLoad(0)
{
    int pipeFd[2];

    if (pipe(pipeFd) == -1)
    {
        m_sendNotifyFd = -1;
        m_recvNotifyFd = -1;
        ERRORLOG1("pipe err, %s", strerror(errno));

        return;
    }

    m_recvNotifyFd = pipeFd[0];
    m_sendNotifyFd = pipeFd[1];

    if (workerNum > 1)
    {
        m_workers = (EventWorker *)new char[sizeof(EventWorker) * m_workerNum];
        for (int i = 0; i < m_workerNum; ++i)
        {
            new(&m_workers[i]) EventWorker(EG_MAX_EVENTHANDLER / workerNum + 1);
        }
        m_workerNum = workerNum;
    }else
    {
        m_workers = NULL;
    }
}

EventManager::~EventManager()
{
    if (NULL != m_workers)
    {
        delete [] (char *)m_workers;
    }

    if (-1 != m_sendNotifyFd)
    {
        close(m_sendNotifyFd);
        close(m_recvNotifyFd);
    }
}

void EventManager::stop()
{
    stopLoop();
    sched_yield();

    while (2 != m_isStop) sched_yield();

    if (NULL == m_workers) return;

    for (int i = 0; i < m_workerNum; ++i)
    {
        (m_workers + i)->~EventWorker();
    }
}

void EventManager::stopLoop()
{
    if (!__sync_bool_compare_and_swap(
                        const_cast<volatile int *>(&m_isStop), 0, 1)) return;

    if (write(m_sendNotifyFd, "", 1) == -1)
    {
        ERRORLOG1("write err, %s", strerror(errno));
    }
}

void EventManager::handleEvent(EventHandler *handler)
{
    int i;
    if (NULL == m_workers)
    {
        if (handler->hasRead()) 
        {
            handler->read();
            handler->inactivateRead();
        }
        if (handler->hasWrite())
        {
            handler->write();
            handler->inactivateWrite();
        }

        return;
    }

    for (; !m_isStop;)
    {
        for (i = 0; i < m_workerNum; ++i)
        {
            m_curWorker = ++m_curWorker % m_workerNum;
            if (!m_workers[m_curWorker].isBusy())
            {
                m_workers[m_curWorker].addEvent(handler);

                return;
            }
        }
        m_isOverLoad = 1;
        ERRORLOG("system over load.");
        sched_yield();
    }
    m_isOverLoad = 0;
}

}

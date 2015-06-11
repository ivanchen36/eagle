#include "EventManager.h"

EventManager::EventManager(const int workerNum)
    : m_workerNum(workerNum), m_curWorker(-1), m_isStop(0),
    m_bufPool(EG_MAX_EVENTHANDLER)
{
    int pipeFd[2];
    m_workers = (EventWorker *)new char[sizeof(EventWorker) * m_workerNum];
    for (int i = 0; i < m_workerNum; ++i)
    {
        new(&m_workers[i]) EventWorker(EG_MAX_EVENTHANDLER / workerNum);
    }

    if (pipe(pipeFd) == -1)
    {
        m_sendNotifyFd = -1;
        m_recvNotifyFd = -1;
        ERRORLOG1("pipe err, %s", strerror(errno));

        return;
    }

    m_recvNotifyFd = pipeFd[0];
    m_sendNotifyFd = pipeFd[1];
}

EventManager::~EventManager()
{
    delete [] (char *)m_workers;

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

    for (int i = 0; i < m_workerNum; ++i)
    {
        m_workers[i].~EventWorker();
    }
}

void EventManager::stopLoop()
{
    m_isStop = 1;
    if (write(m_sendNotifyFd, "", 1) == -1)
    {
        ERRORLOG1("write err, %s", strerror(errno));
    }
}

void EventManager::handleEvent(EventHandler *event)
{
    int i;
    for (; !m_isStop;)
    {
        for (i = 0; i < m_workerNum; ++i)
        {
            m_curWorker = ++m_curWorker % m_workerNum;
            if (!m_workers[m_curWorker].isBusy())
            {
                m_workers[m_curWorker].addEvent(event);

                return;
            }
        }
        ERRORLOG("system over load.");
        sched_yield();
    }
}

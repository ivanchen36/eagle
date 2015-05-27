#include "SelectManager.h"

SelectManager::SelectManager(const int workerNum) 
    : EventManager(workerNum), m_maxFd(m_recvNotifyFd)
{
    FD_ZERO(&m_readSet);
    FD_ZERO(&m_writeSet);
}

SelectManager::~SelectManager()
{
    FD_ZERO(&m_readSet);
    FD_ZERO(&m_writeSet);
}

void SelectManager::loop()
{
    int ret;
    char buf[1];

    for (; !m_isStop;)
    {
        initFdSet();
        ret = select(m_maxFd + 1, &m_readSet, &m_writeSet, NULL, NULL);
        if (ret == -1)
        {
            if (EINTR == errno)
            {
                DEBUGLOG("EINTR");
                continue;
            }

            ERRORLOG1("select err, %s", strerror(errno));

            return;
        }
        if (0 == ret)
        {
            ERRORLOG("select return 0");

            return;
        }

        if (FD_ISSET(m_recvNotifyFd, &m_readSet))
        {
            if (read(m_recvNotifyFd, buf, sizeof(buf)) == -1)
            {
                ERRORLOG1("write err, %s", strerror(errno));
            }
            if (0 == *buf)
            {
                m_isStop = 1;
                INFOLOG("recv exit notify, exit loop");
            }
            if (ret == 1) continue;
        }
        handleFdSet();
    }
}

void SelectManager::flushSelectEvent()
{
    if (write(m_sendNotifyFd, "\1", 1) == -1)
    {
        ERRORLOG1("write err, %s", strerror(errno));
    }
}

int SelectManager::registerEvent(int event, EventHandlerPtr &handler)
{
    int &reEvent = handler->getRegisterEvent();
    LockGuard guard(m_lock);

    if (NONE == reEvent)
    {
        reEvent = event;
        handler->inc();
        m_eventMap[handler->getFd()] = handler.ptr();
        flushSelectEvent();

        return EG_SUCCESS;
    }

    event |= reEvent;
    if (event != reEvent)
    {
        reEvent = event;
        flushSelectEvent();
    }

    return EG_SUCCESS;
}

int SelectManager::unregisterEvent(int event, EventHandlerPtr &handler)
{
    int &reEvent = handler->getRegisterEvent();
    LockGuard guard(m_lock);

    reEvent &= ~event;
    if (NONE != reEvent)
    {
        flushSelectEvent();
    }else
    {
        handler->dec();
        m_eventMap.erase(handler->getFd());
    }

    return EG_SUCCESS;
}

void SelectManager::initFdSet()
{
    int fd;
    int event;
    EventMap::const_iterator iter;

    FD_ZERO(&m_readSet);
    FD_ZERO(&m_writeSet);
    FD_SET(m_recvNotifyFd, &m_readSet);
    m_maxFd = m_recvNotifyFd;
    for (iter = m_eventMap.begin(); iter != m_eventMap.end(); ++iter)
    {
        fd = iter->first;
        event = iter->second->getRegisterEvent();

        if (m_maxFd < fd) m_maxFd = fd;
        if (READ & event) FD_SET(fd, &m_readSet);
        if (WRITE & event) FD_SET(fd, &m_writeSet);
    }
}

void SelectManager::handleFdSet()
{
    int fd;
    int hasEvent;
    EventHandler *handler;
    EventMap::const_iterator iter;

    for (iter = m_eventMap.begin(); iter != m_eventMap.end(); ++iter)
    {
        hasEvent = 0;
        fd = iter->first;
        handler = m_eventMap[fd];
        handler->inc();
        if (FD_ISSET(fd, &m_readSet))
        {
            hasEvent = 1;
            handler->activateRead();
        }
        if (FD_ISSET(fd, &m_writeSet))
        {
            hasEvent = 1;
            handler->activateWrite();
        }

        if (hasEvent) handleEvent(handler);
        handler->dec();
    }
}

#include "SelectManager.h"

namespace eagle
{

SelectManager::SelectManager(const int workerNum) 
    : EventManager(workerNum), m_maxFd(m_recvNotifyFd)
{
    FD_ZERO(&m_readSet);
    FD_ZERO(&m_writeSet);
}

SelectManager::~SelectManager()
{
    EventHandler *handler;
    EventMap::const_iterator iter;

    stop();
    FD_ZERO(&m_readSet);
    FD_ZERO(&m_writeSet);

    for (iter = m_eventMap.begin(); iter != m_eventMap.end(); )
    {
        handler = iter->second;
        if (handler->dec() == 0) delete handler;
        iter = m_eventMap.erase(iter);
    }
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

            break;
        }
        if (0 == ret)
        {
            ERRORLOG("select return 0");

            break;
        }

        if (FD_ISSET(m_recvNotifyFd, &m_readSet))
        {
            if (read(m_recvNotifyFd, buf, sizeof(buf)) == -1)
            {
                ERRORLOG1("write err, %s", strerror(errno));
            }
            if (0 == *buf)
            {
                INFOLOG("recv exit notify, exit loop");
            }
            if (ret == 1) continue;
        }
        handleFdSet();
    }
    m_isStop = 2;
}

void SelectManager::flushSelectEvent()
{
    if (write(m_sendNotifyFd, "\1", 1) == -1)
    {
        ERRORLOG1("write err, %s", strerror(errno));
    }
}

int SelectManager::registerEvent(int event, EventHandler *handler)
{
    int &reEvent = handler->getRegisterEvent();

    if (NONE == reEvent)
    {
        LockGuard guard(m_lock);
        reEvent = event;
        handler->inc();
        if (m_maxFd < handler->getFd()) m_maxFd = handler->getFd();
        m_eventMap[handler->getFd()] = handler;
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

int SelectManager::unregisterEvent(int event, EventHandler *handler)
{
    int &reEvent = handler->getRegisterEvent();

    reEvent &= ~event;
    flushSelectEvent();

    return EG_SUCCESS;
}

void SelectManager::initFdSet()
{
    int fd;
    int event;
    EventHandler *handler;
    EventMap::const_iterator iter;

    FD_ZERO(&m_readSet);
    FD_ZERO(&m_writeSet);
    FD_SET(m_recvNotifyFd, &m_readSet);
    for (iter = m_eventMap.begin(); iter != m_eventMap.end(); ++iter)
    {
        fd = iter->first;
        handler = iter->second;
        event = handler->getRegisterEvent();

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

    for (iter = m_eventMap.begin(); iter != m_eventMap.end(); )
    {
        hasEvent = 0;
        fd = iter->first;
        handler = iter->second;
        if (handler->getRegisterEvent() == NONE)
        {
            LockGuard guard(m_lock);
            if (handler->dec() == 0) delete handler;
            iter = m_eventMap.erase(iter);

            continue;
        }

        if (!handler->hasRead() && FD_ISSET(fd, &m_readSet))
        {
            hasEvent = 1;
            handler->activateRead();
        }
        if (!handler->hasWrite() && FD_ISSET(fd, &m_writeSet))
        {
            hasEvent = 1;
            handler->activateWrite();
        }

        if (hasEvent) handleEvent(handler);
        ++iter;
    }
}

}

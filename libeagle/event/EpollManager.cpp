#include "EpollManager.h"
#include "Log.h"

EpollManager::EpollManager(const int workerNum)
    : EventManager(workerNum)
{
    epoll_event ev;

    m_fd = epoll_create(1);
    if (-1 == m_fd)
    {
        ERRORLOG1("epoll_create err, %s", strerror(errno));

        return;
    }

    if (-1 == m_recvNotifyFd) return;

    ev.events = EPOLLIN | EPOLLET;
    ev.data.ptr = NULL;
    if (epoll_ctl(m_fd, EPOLL_CTL_ADD, m_recvNotifyFd, &ev) != 0)
    {
        ERRORLOG1("epoll_ctl err, %s", strerror(errno));
    }
}

EpollManager::~EpollManager()
{ 
    stop();
    if (-1 != m_fd) close(m_fd);
}

void EpollManager::loop()
{
    if (-1 == m_fd)
    {
        ERRORLOG("epoll not init, exit");

        return;
    }

    int i;
    int events;
    int eventNum;
    EventHandler *handler;

    for (; !m_isStop;)
    {
        eventNum = epoll_wait(m_fd, m_epollEvents, EG_EPOLL_MAX_EVENT, -1);
        if (-1 == eventNum)
        {
            if (EINTR == errno)
            {
                DEBUGLOG("EINTR");
                continue;
            }

            ERRORLOG1("epoll_wait err, %s", strerror(errno));

            break;
        }
        if (0 == eventNum)
        {
            ERRORLOG("epoll_wait return 0, exit");

            break;
        }

        for (i = 0; i < eventNum; ++i)            
        {
            handler = (EventHandler *)m_epollEvents[i].data.ptr;

            if (NULL == handler)
            {
                m_isStop = 1;
                INFOLOG("recv exit notify, exit loop");

                break;
            }

            /* bug : when handler delete by unregisterEvent at the same time */
            handler->inc(); 
            events = m_epollEvents[i].events;

            if ((events & (EPOLLERR | EPOLLHUP))
                    && (events & (EPOLLIN | EPOLLOUT)) ==0)
            {
                events |= EPOLLIN;
            }
            if (events & EPOLLIN)
            {
                handler->activateRead();
            }
            if (events & EPOLLOUT)
            {
                handler->activateWrite();
            }
            handleEvent(handler);
            if (handler->dec() == 0) delete handler;
        }
    }
    m_isStop = 2;
}

int EpollManager::registerEvent(int event, EventHandler *handler)
{
    if (-1 == m_fd) return EG_INVAL;

    int &reEvent = handler->getRegisterEvent();
    LockGuard guard(m_lock);

    if (NONE == reEvent)
    {
        reEvent = event;

        return addEvent(event, handler);
    }

    event |= reEvent;
    if (event == reEvent) return EG_SUCCESS;

    reEvent = event;
    return modifyEvent(event, handler);
}

int EpollManager::unregisterEvent(int event, EventHandler *handler)
{
    if (-1 == m_fd) return EG_INVAL;

    int &reEvent = handler->getRegisterEvent();
    LockGuard guard(m_lock);

    reEvent &= ~event;
    if (NONE == reEvent) return delEvent(event, handler);

    return modifyEvent(reEvent, handler);
}

int EpollManager::addEvent(int event, EventHandler *handler)
{
    epoll_event ev;
    int fd = handler->getFd();

    ev.events = EPOLLET;
    if (READ & event) ev.events |= EPOLLIN;
    if (WRITE & event) ev.events |= EPOLLOUT;
    ev.data.ptr = (void *)handler;

    if (epoll_ctl(m_fd, EPOLL_CTL_ADD, fd, &ev) != 0)
    {
        ERRORLOG1("epoll_ctl err, %s", strerror(errno));

        return EG_FAILED;
    }

    handler->inc();

    return EG_SUCCESS;
}

int EpollManager::modifyEvent(int event, EventHandler *handler)
{
    epoll_event ev;
    int fd = handler->getFd();

    ev.events = EPOLLET;
    if (READ & event) ev.events |= EPOLLIN;
    if (WRITE & event) ev.events |= EPOLLOUT;
    ev.data.ptr = (void *)handler;
    if (epoll_ctl(m_fd, EPOLL_CTL_MOD, fd, &ev) != 0)
    {
        ERRORLOG1("epoll_ctl err, %s", strerror(errno));

        return EG_FAILED;
    }

    return EG_SUCCESS;
}

int EpollManager::delEvent(int event, EventHandler *handler)
{
    int ret;
    int fd = handler->getFd();

    ret = epoll_ctl(m_fd, EPOLL_CTL_DEL, fd, NULL);
    if (handler->dec() == 0) delete handler;
    if (0 == ret) return EG_SUCCESS;

    ERRORLOG1("epoll_ctl err, %s", strerror(errno));

    return EG_FAILED;
}


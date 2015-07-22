/**
 *       Filename:  EpollManager.h
 *
 *    Description:  epoll event manager
 *
 *        Version:  1.0
 *        Created:  05/23/2015 11:25:16 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan Chen, 228268157@qq.com
 *   Organization:  
 */
#ifndef _EAGLE_EPOLLMANAGER_H_
#define _EAGLE_EPOLLMANAGER_H_
#include <sys/epoll.h>

#include "EventManager.h"

namespace eagle
{

class EpollManager : public EventManager
{
public:
    EpollManager(const int workerNum);
    virtual ~EpollManager();

    virtual void loop();
    virtual int registerEvent(int event, EventHandler *handler);
    virtual int unregisterEvent(int event, EventHandler *handler);

private:
    int addEvent(int event, EventHandler *handler);
    int modifyEvent(int event, EventHandler *handler);
    int delEvent(int event, EventHandler *handler);

    int m_fd; 
    struct epoll_event m_epollEvents[EG_EPOLL_MAX_EVENT];
};

}
#endif   /* ----- #ifndef _EAGLE_EPOLLMANAGER_H_  ----- */

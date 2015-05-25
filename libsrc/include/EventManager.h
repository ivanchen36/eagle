/**
 *       Filename:  EventManager.h
 *
 *    Description:  fd event manager
 *
 *        Version:  1.0
 *        Created:  05/18/2015 11:30:38 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan Chen, 228268157@qq.com
 *   Organization:  
 */
#ifndef  _EVENTMANAGER_H_
#define  _EVENTMANAGER_H_

#include "EventHandler.h"
#include "EventWorker.h"

class EventManager
{
public:
    EventManager(const int workerNum);
    ~EventManager();

    void stopLoop();
    virtual void loop() = 0;
    virtual int registerEvent(int event, EventHandlerPtr &handler) = 0;
    virtual int unregisterEvent(int event, EventHandlerPtr &handler) = 0;

protected:
    void handleEvent(EventHandler *event);

    int m_sendNotifyFd;
    int m_recvNotifyFd;
    int m_workerNum;
    int m_curWorker;
    int m_isStop;
    EventWorker *m_workers;
};

typedef AutoPtr<EventManager> EventManagerPtr;
#endif   /* ----- #ifndef _EVENTMANAGER_H_  ----- */

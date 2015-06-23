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
    virtual ~EventManager();

    void stopLoop();
    virtual void loop() = 0;
    virtual int registerEvent(int event, EventHandler *handler) = 0;
    virtual int unregisterEvent(int event, EventHandler *handler) = 0;

    int isOverLoad()
    {
        return m_isOverLoad;
    }

protected:
    void stop();
    void handleEvent(EventHandler *event);

    int m_sendNotifyFd;
    int m_recvNotifyFd;
    int m_workerNum;
    int m_curWorker;
    int m_isStop;
    int m_isOverLoad;
    MutexLock m_lock;
    EventWorker *m_workers;
};

typedef AutoPtr<EventManager> EventManagerPtr;
#endif   /* ----- #ifndef _EVENTMANAGER_H_  ----- */

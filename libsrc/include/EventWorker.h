/**
 *       Filename:  EventWorker.h
 *
 *    Description:  event worker
 *
 *        Version:  1.0
 *        Created:  05/23/2015 05:04:15 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan Chen, 228268157@qq.com
 *   Organization:  
 */
#ifndef  _EVENTWORKER_H_
#define  _EVENTWORKER_H_

#include "EventHandler.h"
#include "TaskThread.h"
#include "CycleQueue.h"

class EventWorker : public Task
{
public:
    enum Status
    {
        FREE,
        NORMAL,
        BUSY
    };

    EventWorker(const int maxEventSize);
    ~EventWorker();

    int excute();
    int addEvent(EventHandler *handler);
    
    int isBusy()
    {
        return m_status == BUSY;
    }

private:
    int handleEvent();
    typedef CycleQueue<EventHandler *> EventQueue;

    Status m_status;
    EventQueue m_queue;
    TaskThread *m_thread;
};
#endif   /* ----- #ifndef _EVENTWORKER_H_  ----- */

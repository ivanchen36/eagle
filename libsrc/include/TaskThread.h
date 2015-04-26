/**
 *       Filename:  TaskThread.h
 *
 *    Description:  task thread
 *
 *        Version:  1.0
 *        Created:  04/20/2015 10:49:16 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan Chen, 228268157@qq.com
 *   Organization:  
 */

#ifndef  _TASKThread_H_
#define  _TASKThread_H_

#include <semaphore.h>
#include <pthread.h>

#include "Task.h"
#include "AutoPtr.h"

class TaskThread
{
public:
    enum Status
    {
        WAIT = 1,
        RUN,
        STOP,
    };

    void wait();
    void run();
    void stop();
    void wait();

private:
    Status m_status;
    sem_t m_sem;
    pthread_t m_threadId;
};

typedef AutoPtr<TaskThread> TaskThreadPtr;

#endif   /* ----- #ifndef _taskthread_H_  ----- */

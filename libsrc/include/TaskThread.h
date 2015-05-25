/**
 *       Filename:  TaskThread.h
 *
 *    Description:  TaskThread
 *
 *        Version:  1.0
 *        Created:  04/27/2015 10:43:46 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan Chen, 228268157@qq.com
 *   Organization:  
 */
#ifndef  _TASKTHREAD_H_
#define  _TASKTHREAD_H_

#include "Log.h"
#include "Thread.h"
#include "ThreadSem.h"
#include "MutexLock.h"

class Task
{
public:
    Task(const char *name);
    ~Task();

    const char *getName();
    virtual int excute() = 0;

private:
    char *m_name;
};

typedef AutoPtr<Task> TaskPtr;

class TaskThread : public Thread
{
public:
    enum Status
    {
        WAIT = 1,
        RUN,
        STOP
    };

    TaskThread(const TaskPtr &task, const int stackSize = 0);
    ~TaskThread();

    int start();
    int pause();
    int stop();
    int isRun();
    int isWait();
    int isStop();
    virtual void run();

private:
    Status m_status;
    MutexLock m_lock;
    SemaphorePtr m_sem; 
    TaskPtr m_task;
};

typedef AutoPtr<TaskThread> TaskThreadPtr;
#endif   /* ----- #ifndef _TASKTHREAD_H_  ----- */

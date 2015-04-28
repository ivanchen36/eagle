/**
 *       Filename:  Thread.h
 *
 *    Description:  thread
 *
 *        Version:  1.0
 *        Created:  04/27/2015 10:43:46 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan Chen, 228268157@qq.com
 *   Organization:  
 */
#ifndef  _THREAD_H_
#define  _THREAD_H_
#include <pthread.h>

#include "ThreadSem.h"
#include "AutoPtr.h"
#include "MutexLock.h"

static void *threadStart(void *arg);

class Task
{
public:
    Task(const char *name);
    ~Task();

    const char *getName();
    virtual int excute(){}
private:
    Task()
    {
        ERRORLOG("err constructor");
    }
    char *m_name;

    friend class AutoPtr<Task>;
};

typedef AutoPtr<Task> TaskPtr;

class Thread
{
public:
    enum Status
    {
        WAIT = 1,
        RUN,
        STOP,
    };

    Thread(TaskPtr &task, const int stackSize = 0);
    ~Thread();

    int start();
    int pause();
    int stop();
    int isRun();
    int isWait();
    int isStop();

private:
    Thread()
    {
        ERRORLOG("err constructor");
    }

    void doTask();

    int m_isInit;
    pthread_t m_id;
    Status m_status;
    SemaphorePtr m_sem; 
    TaskPtr m_task;

    friend class AutoPtr<Thread>;
    friend void *threadStart(void *arg);
};

typedef AutoPtr<Thread> ThreadPtr;
#endif   /* ----- #ifndef _THREAD_H_  ----- */

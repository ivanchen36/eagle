/**
 *       Filename:  ThreadSem.h
 *
 *    Description:  thread semaphore
 *
 *        Version:  1.0
 *        Created:  04/26/2015 11:36:54 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan Chen, 228268157@qq.com
 *   Organization:  
 */
#ifndef  _THREADSEM_H_
#define  _THREADSEM_H_

#include <semaphore.h>

#include "SemEx.h"

class ThreadSem : public Semaphore
{
public:
    ThreadSem(const int val = 0);
    ~ThreadSem();

    virtual int post();
    virtual int wait();
    virtual int timedWait(const int sec);

private:
    int m_isInit;
    sem_t m_sem;
};

typedef AutoPtr<ThreadSem> ThreadSemPtr;

#endif   /* ----- #ifndef _THREADSEM_H_  ----- */
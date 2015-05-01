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

#include "AutoPtr.h"

static void *threadStart(void *arg);

class Thread
{
public:
    
    Thread(const int stackSize = 0);
    ~Thread();

    virtual void run(){}

private:
    pthread_t m_id;

    friend class AutoPtr<Thread>;
    friend void *threadStart(void *arg);
};

typedef AutoPtr<Thread> ThreadPtr;
#endif   /* ----- #ifndef _THREAD_H_  ----- */

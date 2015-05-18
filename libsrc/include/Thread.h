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
#include "CallBack.h"

class Thread
{
public:
    Thread(const CallBack &cb, const int isDetach = 1, 
            const int stackSize = 0);
    Thread(const int stackSize = 0);
    virtual ~Thread();

    virtual void run();
    int isDetach()
    {
        return m_isDetach != 0;
    }

private:
    void init(const int stackSize);

    int m_isDetach;
    pthread_t m_id;
    CallBack m_cb;
};

typedef AutoPtr<Thread> ThreadPtr;
#endif   /* ----- #ifndef _THREAD_H_  ----- */

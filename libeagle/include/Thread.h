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
#ifndef _EAGLE_THREAD_H_
#define _EAGLE_THREAD_H_
#include <pthread.h>

#include "AutoPtr.h"
#include "CallBack.h"

namespace eagle
{

class Thread
{
public:
    Thread(const int stackSize = 0);
    Thread(const CallBack &cb, const int isDetach = 1, 
            const int stackSize = 0);
    virtual ~Thread();

    virtual void run(){};

    pthread_t getId()
    {
        return m_id;
    }

private:
    void init(const int stackSize, void *(*cbFunc)(void *), void *cbParam);

    int m_isDetach;
    pthread_t m_id;
};

typedef AutoPtr<Thread> ThreadPtr;

}
#endif   /* ----- #ifndef _EAGLE_THREAD_H_  ----- */

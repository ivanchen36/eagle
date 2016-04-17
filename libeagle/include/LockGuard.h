/**
 *       Filename:  LockGuard.h
 *
 *    Description:  lock guard 
 *
 *        Version:  1.0
 *        Created:  04/17/2016 03:14:46 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan Chen, 228268157@qq.com
 *   Organization:  
 */
#ifndef _EAGLE_LOCKGUARD_H_
#define _EAGLE_LOCKGUARD_H_

#include <pthread.h>

#include "AutoPtr.h"

namespace eagle
{

template<class T> class LockGuard
{
public:
    LockGuard(T &lock) : m_lock(lock)
    {
        m_lock.lock();
    }

    ~LockGuard()
    {
        m_lock.unLock();
    }

private:
    void *operator new(size_t size);

    T &m_lock;
};

}
#endif   /* ----- #ifndef _EAGLE_LOCKGUARD_H_  ----- */

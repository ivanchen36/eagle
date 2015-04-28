/**
 *       Filename:  MutexLock.h
 *
 *    Description:  mutex lock
 *
 *        Version:  1.0
 *        Created:  04/23/2015 12:46:55 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan Chen, 228268157@qq.com
 *   Organization:  
 */
#ifndef  _MUTEXLOCK_H_
#define  _MUTEXLOCK_H_

#include <pthread.h>

#include "AutoPtr.h"

class MutexLock
{
public:
    MutexLock();
    ~MutexLock();

    int lock();
    int unLock();
    int tryLock();

private:
    pthread_mutex_t m_mutex;
};

typedef AutoPtr<MutexLock> MutexLockPtr;

class LockGuard
{
public:
    LockGuard(MutexLockPtr &lock);
    ~LockGuard();

private:
    void *operator new(size_t size);

    MutexLockPtr m_lock;
};

#endif   /* ----- #ifndef _MUTEXLOCK_H_  ----- */

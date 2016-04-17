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
#ifndef _EAGLE_MUTEXLOCK_H_
#define _EAGLE_MUTEXLOCK_H_

#include <pthread.h>

#include "AutoPtr.h"
#include "LockGuard.h"

namespace eagle
{

class MutexLock
{
public:
    MutexLock(const int isRecursive = 0, const int isPshared = 0);
    ~MutexLock();

    int lock();
    int unLock();
    int tryLock();

private:
    pthread_mutex_t m_mutex;
};

typedef AutoPtr<MutexLock> MutexLockPtr;

}
#endif   /* ----- #ifndef _EAGLE_MUTEXLOCK_H_  ----- */

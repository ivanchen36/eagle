/**
 *       Filename:  Lock.h
 *
 *    Description:  Lock
 *
 *        Version:  1.0
 *        Created:  04/23/2015 12:46:55 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan Chen, 228268157@qq.com
 *   Organization:  
 */


#ifndef  _LOCK_H_
#define  _LOCK_H_

#include <pthread.h>

class Lock
{
public:
    Lock(pthread_mutex_t lock);
    ~Lock();

private:
    void *operator new(size_t size);
    pthread_mutex_t m_lock;
};

#endif   /* ----- #ifndef _LOCK_H_  ----- */

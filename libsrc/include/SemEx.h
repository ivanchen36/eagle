/**
 *       Filename:  SemEx.h
 *
 *    Description:  semaphore
 *
 *        Version:  1.0
 *        Created:  04/27/2015 09:15:39 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan Chen, 228268157@qq.com
 *   Organization:  
 */
#ifndef  _SEMEX_H_
#define  _SEMEX_H_

#include "AutoPtr.h"

#define IS_AGAIN() (ETIMEDOUT == errno || \
        EAGAIN == errno || EINTR == errno)

class Semaphore
{
public:
    virtual int post(){ return 0; }
    virtual int wait(){ return 0; }
    virtual int timedWait(const int sec){ return 0; }

};

typedef AutoPtr<Semaphore> SemaphorePtr;
#endif   /* ----- #ifndef _SEMEX_H_  ----- */

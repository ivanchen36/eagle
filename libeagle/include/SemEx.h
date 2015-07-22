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
#ifndef _EAGLE_SEMEX_H_
#define _EAGLE_SEMEX_H_
#include "AutoPtr.h"
#include "Define.h"

namespace eagle
{

class Semaphore
{
public:
    virtual ~Semaphore() {};
    virtual int post() = 0;
    virtual int wait() = 0;
    virtual int timedWait(const int sec) = 0;

};

typedef AutoPtr<Semaphore> SemaphorePtr;

}
#endif   /* ----- #ifndef _EAGLE_SEMEX_H_  ----- */

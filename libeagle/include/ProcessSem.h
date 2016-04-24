/**
 *       Filename:  ProcessSem.h
 *
 *    Description:  process semaphore
 *
 *        Version:  1.0
 *        Created:  04/25/2015 11:13:48 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan Chen, 228268157@qq.com
 *   Organization:  
 */
#ifndef _EAGLE_PROCESSSEM_H_
#define _EAGLE_PROCESSSEM_H_

#include "SemEx.h"

namespace eagle
{

class ProcessSem : public Semaphore
{
public:
    ProcessSem(const int key, const int val = 0);
    virtual ~ProcessSem();

    virtual int post();
    virtual int wait();
    virtual int timedWait(const int msec);
    int op(const int val, const int msec = 0);
    int getVal();

private:
    int init(int val);
    int m_semId;
    char *m_ref;
};

typedef AutoPtr<ProcessSem> ProcessSemPtr;

}
#endif   /* ----- #ifndef _EAGLE_PROCESSSEM_H_  ----- */

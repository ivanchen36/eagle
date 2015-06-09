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
#ifndef  _PROCESSSEM_H_
#define  _PROCESSSEM_H_

#include "SemEx.h"

class ProcessSem : public Semaphore
{
public:
    ProcessSem(const int preceesNum = 1, const int val = 0);
    ~ProcessSem();

    virtual int post();
    virtual int wait();
    virtual int timedWait(const int sec);
    int op(const int val, const int sec = 0);

private:
    int init(int val);
    int m_semId;
    int *m_ref;
};

typedef AutoPtr<ProcessSem> ProcessSemPtr;

#endif   /* ----- #ifndef _PROCESSSEM_H_  ----- */

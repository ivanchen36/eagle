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

#include "AutoPtr.h"

class ProcessSem
{
public:
    ProcessSem(const int val = 0);
    ~ProcessSem();

    int op(const int val, const int sec = 0);
    int post();
    int wait();
    int timedWait(const int sec);

private:
    int init(int val);
    int m_semId;
};

typedef AutoPtr<ProcessSem> ProcessSemPtr;

#endif   /* ----- #ifndef _PROCESSSEM_H_  ----- */

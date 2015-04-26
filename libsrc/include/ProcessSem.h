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
    ProcessSem();
    ~ProcessSem();

    int post(const int val = 1);
    int wait(const int val = 1);
    int timedWait(const int val = 1);

private:
    int m_semId;
};

typedef AutoPtr<ProcessSem> ProcessSemPtr;

#endif   /* ----- #ifndef _PROCESSSEM_H_  ----- */

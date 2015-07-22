/**
 *       Filename:  LogTest.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  04/26/2015 02:18:19 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan Chen, 228268157@qq.com
 *   Organization:  
 */


#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "Log.h"
#include "MutexLock.h"
#include "EagleTime.h"

using namespace std;
using namespace eagle;

void test(MutexLock &lock, int level)
{
    DEBUGLOG1("lock level %d", level);
    LockGuard guard(lock);
    if (--level > 0)
    {
        test(lock, level);
    }
}

void doTest()
{
    MutexLock lock(1);

    test(lock, 4);

    MutexLock lock1;

    test(lock1, 4);
}

void test1(MutexLock &lock)
{
    pid_t pid=fork();
    if(pid<0)
    {
        ERRORLOG("fork err");
    }
    if (pid == 0) return;

    pid=fork();
    if (pid == 0) return;

    pid=fork();
}

void doTest1()
{
    MutexLock lock(0, 1);

    test1(lock);
}

/**
 * @brief main 
 */
int main ( int argc, char *argv[] )
{
    EagleTimeI::instance().autoUpdate();

    doTest1();

    return EXIT_SUCCESS;
}

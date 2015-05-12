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

void test(MutexLock &lock, int level)
{
    DEBUGLOG1("lock level %d", level);
    LockGuard guard(lock);
    if (--level > 0)
    {
        test(lock, level);
    }
}

void dotest()
{
    MutexLock lock(1);

    test(lock, 4);

    MutexLock lock1;

    test(lock1, 4);
}

/**
 * @brief main 
 */
int main ( int argc, char *argv[] )
{
    if (g_sysLog->redirectToOther(STDOUT_FILENO))
    {
        DEBUGLOG("redirectToOther err");
    }
    EagleTimeI::instance().autoUpdate();

    dotest();

    return EXIT_SUCCESS;
}

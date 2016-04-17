/**
 *       Filename:  PSemTest.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  04/26/2015 08:30:37 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan Chen, 228268157@qq.com
 *   Organization:  
 */

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sys/wait.h>

#include "Log.h"
#include "EagleTime.h"
#include "ProcessSem.h"

using namespace std;
using namespace eagle;

void test(SemaphorePtr &semPtr)
{
    pid_t pid=fork();
    if(pid<0)
    {
        ERRORLOG("fork err");
    }
    else if(pid == 0)
    {
        DEBUGLOG("child wait");
        if (semPtr->wait())
        {
            DEBUGLOG("wait err");
        }
        DEBUGLOG("child exit");
    }
    else
    {
        int status;
        int ret;

        DEBUGLOG1("child pid %d", pid);
        DEBUGLOG("parent sleep");
        sleep(3);
        DEBUGLOG("parent post");
        if (semPtr->post())
        {
            DEBUGLOG("post err");
        }
        sleep(2);
        semPtr->post();
        sleep(2);
        ret = waitpid(pid, NULL, 0);
        DEBUGLOG1("child %d exit", ret);
        DEBUGLOG("parent exit");
    }
}

void test1(SemaphorePtr &semPtr)
{
    int ret;

    pid_t pid=fork();
    if(pid<0)
    {
        ERRORLOG("fork err");
    }
    else if(pid == 0)
    {
        DEBUGLOG("child wait");
        while ((ret = semPtr->timedWait(1000)))
        {
            if (ret < 0)
            {
                DEBUGLOG("wait err");
            }else
            {
                DEBUGLOG("wait timeout");
            }
        }
        DEBUGLOG("child exit");
    }
    else
    {
        int status;

        DEBUGLOG1("child pid %d", pid);
        DEBUGLOG("parent sleep");
        sleep(10);
        DEBUGLOG("parent post");
        if (semPtr->post())
        {
            DEBUGLOG("post err");
        }
        ret = waitpid(pid, NULL, 0);
        DEBUGLOG1("child %d exit", ret);
        DEBUGLOG("parent exit");
    }
}

void doTest()
{
    SemaphorePtr semPtr = new ProcessSem(2);
    test1(semPtr);
}

/**
 * @brief main 
 */
int main (int argc, char *argv[] )
{
    doTest();

    return EXIT_SUCCESS;
}

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
#include "ProcessSem.h"

using namespace std;

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
        exit(3);
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
        ret = waitpid(pid, &status, WNOHANG | WUNTRACED);
        DEBUGLOG1("waitpid %d", ret);
        while (!ret)
        {
            DEBUGLOG("parent wait status");
            sleep(2);
            semPtr->post();
            ret = waitpid(pid, &status, WNOHANG | WUNTRACED);
        }
        if (WEXITSTATUS(status))
        {
            DEBUGLOG1("child status %d", status);
        }
        DEBUGLOG("parent exit");
        exit(3);
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
        while ((ret = semPtr->timedWait(1)))
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
        exit(3);
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
        ret = waitpid(pid, &status, WNOHANG | WUNTRACED);
        DEBUGLOG1("waitpid %d", ret);
        while (!ret)
        {
            DEBUGLOG("parent wait status");
            sleep(2);
            semPtr->post();
            ret = waitpid(pid, &status, WNOHANG | WUNTRACED);
        }
        if (WEXITSTATUS(status))
        {
            DEBUGLOG1("child status %d", status);
        }
        DEBUGLOG("parent exit");
        exit(3);
    }
}



/**
 * @brief main 
 */
int main (int argc, char *argv[] )
{
    if (g_sysLog->redirectToOther(STDOUT_FILENO))
    {
        DEBUGLOG("redirectToOther err");
    }
    SemaphorePtr semPtr = new ProcessSem();
    test1(semPtr);
    exit(3);

    return EXIT_SUCCESS;
}

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
#include "EagleTime.h"

using namespace std;

void test()
{
    DEBUGLOG("debug test");
    DEBUGLOG1("%s", "debug1 test");
    INFOLOG("info test");
    INFOLOG1("%s", "info1 test");
    WARNLOG("warn test");
    WARNLOG1("%s", "warn1 test");
    ERRORLOG("error test");
    ERRORLOG1("%s", "error1 test");
}

void test1()
{
    if (g_sysLog->redirectToOther(STDOUT_FILENO))
    {
        DEBUGLOG("redirectToOther err");
    }
    DEBUGLOG("debug test1");
    DEBUGLOG1("%s", "debug1 test1");
    INFOLOG("info test1");
    INFOLOG1("%s", "info1 test1");
    WARNLOG("warn test1");
    WARNLOG1("%s", "warn1 test1");
    ERRORLOG("error test1");
    ERRORLOG1("%s", "error1 test1");
}

void test2()
{
    if (g_sysLog->redirectToLog(STDOUT_FILENO))
    {
        DEBUGLOG("redirectToLog err");
    }
    printf("test2\n");
}

/**
 * @brief main 
 */
int main ( int argc, char *argv[] )
{
    int i;

    DEBUGLOG1("%d", sizeof(EagleTime));
    EagleTimeI::instance().autoUpdate();
    test1();
    for (i = 0; i < 20; ++i)
    {
        sleep(10);
    }
    test1();
    for (i = 0; i < 10; ++i)
    {
        sleep(10);
    }

    return EXIT_SUCCESS;
}

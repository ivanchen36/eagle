/**
 *       Filename:  EagleTest.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/04/2015 10:49:15 PM
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
#include "Eagle.h"

using namespace std;

int g_waitQuit;

void notifyQuit(void *pram)
{
    g_waitQuit = 1;
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
    CallBack cb(notifyQuit);
    EAGLE_INIT(cb);

    while (0 == g_waitQuit)
    {
        DEBUGLOG("child wait sigquit");
        sleep(10);
    }

    return EXIT_SUCCESS;
}

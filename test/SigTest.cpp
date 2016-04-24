/**
 *       Filename:  CHTest.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  04/03/2016 12:53:07 AM
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
#include "MasterSigManager.h"

using namespace std;
using namespace eagle;

void doTest()
{
    pid_t pid = fork();
    if (pid == 0)
    {
        DEBUGLOG("wait for sig");
        MasterSigManagerI::instance().init(); 
        MasterSigManagerI::instance().setBlock();
        MasterSigManagerI::instance().block();
        DEBUGLOG("receive sig, quit");

        return;
    }

    sleep(1);
    kill(pid, SIGQUIT);
    waitpid(-1, NULL, 0);
    sleep(2);
}

/**
 * @brief main 
 */
int main ( int argc, char *argv[] )
{
    doTest();

    return EXIT_SUCCESS;
}

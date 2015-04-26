/**
 *       Filename:  Test.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  04/22/2015 07:29:30 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan Chen, 228268157@qq.com
 *   Organization:  
 */

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>

#include "AutoPtr.h"

using namespace std;

void init()
{
    pid_t pid=fork();
    if(pid<0)
    {
        fprintf(stderr,"err");
    }
    else if(pid == 0)
    {
        cout << "child" << endl;
        cout << "exit" << endl;
    }
    else
    {
        int status;
        cout << "child pid " << pid << endl;
        int ret = waitpid(pid, &status, WNOHANG | WUNTRACED);
        cout << status << endl;
        while (!WEXITSTATUS(status))
        {
            sleep(2);
            ret = waitpid(pid, &status, WNOHANG | WUNTRACED);
        }
        cout << "child exit" << endl;
        cout << ret << endl;
        exit(3);
    }
}

/**
 * @brief main 
 */
int main (int argc, char *argv[])
{
    init();
    exit(3);

    return EXIT_SUCCESS;
}

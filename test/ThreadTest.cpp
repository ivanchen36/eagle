/**
 *       Filename:  ThreadTest.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  04/28/2015 12:11:07 AM
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
#include "TaskThread.h"

using namespace std;

class HelloTask : public Task
{
public:
    HelloTask(const char *name) : Task(name){}
    int excute()
    {
        if (m_index > 3) 
        {
            m_index = 0;
            return -1;
        }

        ++m_index;
        sleep(1);
        DEBUGLOG("hello word");

        return 0;
    }

private:
    int m_index;
};

/**
 * @brief main 
 */
int main ( int argc, char *argv[] )
{
    if (g_sysLog->redirectToOther(STDOUT_FILENO))
    {
        DEBUGLOG("redirectToLog err");
    }

    TaskPtr task = new HelloTask ("say hello");

    DEBUGLOG("create thread");
    TaskThreadPtr thread = new TaskThread(task);

    DEBUGLOG("start thread");
    thread->start();
    sleep(50);
    DEBUGLOG("pause thread");
    thread->pause();
    sleep(3);
    DEBUGLOG("start thread");
    thread->start();
    sleep(10);
    DEBUGLOG("stop thread");
    thread->stop();

    return EXIT_SUCCESS;
}

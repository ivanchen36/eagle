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

int g_isQuit = 0;

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

class HelloThread : public Thread
{
public:
    HelloThread()
    {
        DEBUGLOG("HelloThread");
    }
    ~HelloThread()
    {
        DEBUGLOG("~HelloThread");
    }
    virtual void run()
    {
        DEBUGLOG("run hello");
        int i = 4;
        while (--i > 0)
        {
            sleep(2);
            DEBUGLOG("hello");
        }
        g_isQuit = 1;
    }
};

void test()
{
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
}

void test1()
{
    {
        HelloThread thread;
        usleep(10);
    }
    DEBUGLOG("wait");
    while(!g_isQuit)
    {
        sleep(2);
    }
    sleep(2);
}

void sayHello(void *param)
{
    DEBUGLOG("run hello");
    int i = 4;
    while (--i > 0)
    {
        sleep(2);
        DEBUGLOG("hello");
    }
    g_isQuit = 1;
}

void test2()
{
    {
        CallBack cb(sayHello, NULL);
        Thread thread(cb);
    }
    DEBUGLOG("wait");
    while(!g_isQuit)
    {
        sleep(2);
    }
    sleep(2);
}

/**
 * @brief main 
 */
int main ( int argc, char *argv[] )
{
    if (g_sysLog->redirectToOther(STDOUT_FILENO))
    {
        DEBUGLOG("redirectToLog err");
    }

    test2(); 
    
    return EXIT_SUCCESS;
}

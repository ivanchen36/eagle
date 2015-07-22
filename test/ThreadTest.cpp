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
#include <sys/epoll.h>
#include <sys/types.h>
#include <signal.h>

#include "Log.h"
#include "TaskThread.h"

using namespace std;
using namespace eagle;

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
    TaskThreadPtr thread = new TaskThread(task.ptr());

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

void epollTest(void *param)
{
#define MAX_EVENT 12

    int events;
    int fd = *(int *)param;
    struct epoll_event epollEvents[MAX_EVENT];

    for (; ;)
    {
        DEBUGLOG("epoll_wait");
        events = epoll_wait(fd, epollEvents, EG_EPOLL_MAX_EVENT, -1);
        if (events == -1)
        {
            ERRORLOG1("epoll_wait err, %s", strerror(errno));

            return;
        }

        DEBUGLOG1("epoll_wait %d", events);
        DEBUGLOG1("epoll_wait data %d", epollEvents[0].data.ptr);

        return;
    }
}

void sigHandler(const int sig)
{
    DEBUGLOG1("sigHandler %d", sig);
}

void doEpollTest()
{
    int fd = epoll_create(1);
    
    {
        CallBack cb(epollTest, &fd);
        Thread thread(cb);
    }

    int pipeFd[2];
    epoll_event ev;

    ev.events = EPOLLIN | EPOLLET;
    ev.data.ptr = NULL;
    pipe(pipeFd);
    if (epoll_ctl(fd, EPOLL_CTL_ADD, pipeFd[0], &ev) != 0)
    {
        ERRORLOG1("epoll_ctl err, %s", strerror(errno));
    }

    sleep(1);
    write(pipeFd[1], "", 1);
    close(fd);
    sleep(1);
}

/**
 * @brief main 
 */
int main ( int argc, char *argv[] )
{
    test2();
    
    return EXIT_SUCCESS;
}

/**
 *       Filename:  CyQueueTest.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/23/2015 04:40:47 PM
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
#include "CycleQueue.h"
#include "EagleTime.h"
#include "Thread.h"

using namespace std;

void consumer(void *param)
{
    int val;
    CycleQueue<int> *queue = (CycleQueue<int> *)param;
    for (; ;)
    {
        if (queue->empty())
        {
            DEBUGLOG("queue empty");
            sleep(2);
        }else
        {
            queue->pop(val);
            DEBUGLOG1("read val %d", val);
            if (val == 5120) return; 
        }
    }
}

void producer(CycleQueue<int> &queue)
{
    for (int i = 0; i <= 5120; ++i)
    {
        if (queue.full())
        {
            DEBUGLOG("queue full");
            sleep(2);
        }else
        {
            queue.push(i);
            if (i % 888 == 0) sleep(1);
        }
    }
}

void doTest()
{
    CycleQueue<int> queue(512);
    CallBack cb(consumer, (void *)&queue);
    Thread clientThread(cb);
    producer(queue);
    sleep(3);
}

void consumer1(void *param)
{
    int val;
    CycleQueue<int> *queue = (CycleQueue<int> *)param;
    for (; ;)
    {
        if (queue->empty())
        {
            //DEBUGLOG("queue empty");
            sched_yield();
        }else
        {
            queue->pop(val);
            if (val == 0x5FFFFFFF) return; 
        }
    }
}

void producer1(CycleQueue<int> &queue)
{
    for (int i = 0; i <= 0x3FFFFFFF; ++i)
    {
        if (queue.full())
        {
            //DEBUGLOG("queue full");
            sched_yield();
        }else
        {
            queue.push(i);
        }
    }
    for (int i = 0; i <= 0x5FFFFFFF; ++i)
    {
        if (queue.full())
        {
            //DEBUGLOG("queue full");
            sched_yield();
        }else
        {
            queue.push(i);
        }
    }
}

void doTest1()
{
    CycleQueue<int> queue(10240);
    CallBack cb(consumer1, (void *)&queue);
    Thread clientThread(cb);
    producer1(queue);
    sleep(3);
}

/**
 * @brief main 
 */
int main ( int argc, char *argv[] )
{
    doTest1();

    return EXIT_SUCCESS;
}

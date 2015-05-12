/**
 *       Filename:  Timer.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/09/2015 10:59:22 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan Chen, 228268157@qq.com
 *   Organization:  
 */
#ifndef  _TIMER_H_
#define  _TIMER_H_
#include <stdint.h>
#include <string.h>

#include "RbTree.h"
#include "CallBack.h"
#include "Singleton.h"
#include "MutexLock.h"
#include "TaskThread.h"

class Timer
{
public:
    struct TaskNode
    {
        char *name;
        int interval;
        int times;
        int isAsync;
        CallBack cb;
        TaskNode *nextTask;
        TaskNode *nextTimer;

        TaskNode(const char *name, const int msec, 
                const int isAsync, const int times, const CallBack &cb)
            : interval(msec), isAsync(isAsync), times(times), cb(cb)
        {
            int len = strlen(name);
            this->name = new char[strlen(name) + 1];
            strcpy(this->name, name);
        }

        ~TaskNode()
        {
            if (NULL != name) delete name;
        }
    };

    Timer();
    ~Timer();

    void excute();
    int addTask(const char *name, const int msec,
            const CallBack &cb, const int isAsync = 0, 
            const int times = -1);
    int delTask(const char *name);

private:
    typedef RbTree<uint64_t, Timer::TaskNode *> TaskMap;

    int setTimer(int msec);
    int addTimer(const uint64_t startTime, TaskNode *task);
    TaskNode *find(const char *name);
    void delTask(TaskNode *task);

    uint64_t m_nextExcuteTime;
    TaskNode *m_taskListHead;
    MutexLock m_lock;
    TaskMap m_taskMap;

    friend class StartTimer;
};

typedef Singleton<Timer> TimerI;
#endif   /* ----- #ifndef _TIMER_H_  ----- */

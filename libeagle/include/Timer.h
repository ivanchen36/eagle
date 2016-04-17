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
#ifndef _EAGLE_TIMER_H_
#define _EAGLE_TIMER_H_
#include <stdint.h>
#include <string.h>

#include "RbTree.h"
#include "CallBack.h"
#include "Singleton.h"
#include "MutexLock.h"
#include "TaskThread.h"
#include "StrUtil.h"
#include "SpinLock.h"
#include "EagleTime.h"

namespace eagle
{

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

        TaskNode(const char *n, const int m, 
                const int i, const int t, const CallBack &c)
            : interval(m), times(t), isAsync(i), cb(c)
        {
            StrUtil::copy(this->name, n);
        }

        ~TaskNode()
        {
            if (NULL != name) delete name;
        }
    };

    Timer();
    ~Timer();

    void start();
    void stop();
    void pause();
    void loop();
    void execute();
    int addTask(const char *name, const int msec,
            const CallBack &cb, const int isAsync = 0, 
            const int times = -1);
    int delTask(const char *name);

private:
    enum Status
    {
        START,
        PAUSE,
        STOP
    };
    typedef RbTree<uint64_t, Timer::TaskNode *> TaskMap;

    void quitThread();
    int setTimer(int msec);
    void addTimer(const uint64_t startTime, TaskNode *task);
    TaskNode *find(const char *name);
    void delTask(TaskNode *task);

    Status m_status;
    SpinLock m_lock;
    uint64_t m_nextExecuteTime;
    TaskNode *m_taskListHead;
    TaskMap m_taskMap;
    ThreadSem m_sem;
    Thread *m_thread;
    EagleTime &m_eagleTime;
};

typedef Singleton<Timer> TimerI;

}
#endif   /* ----- #ifndef _EAGLE_TIMER_H_  ----- */

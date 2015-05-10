#include <sys/time.h>
#include <signal.h>

#include "Timer.h"
#include "Thread.h"
#include "EagleTime.h"
#include "Log.h"

namespace
{
void excuteTimer(int sig)
{
    TimerI::instance().excute();
}
}

Timer::Timer() : m_nextExcuteTime((uint64_t)-1), m_taskListHead(NULL)
{ 
    struct sigaction sa;

    bzero(&sa, sizeof(struct sigaction)); 
    sa.sa_handler = excuteTimer;
    sigemptyset(&sa.sa_mask); 
    if (sigaction(SIGALRM, &sa, NULL) == -1) 
    {
        ERRORLOG2("sigaction %d err, %s", 
                SIGALRM, strerror(errno));
    }
}

Timer::~Timer()
{
    TaskNode *cur;
    TaskNode *next;
    TaskMap::Iterator iter;

    signal(SIGALRM, SIG_DFL);
    for (iter = m_taskMap.begin(); iter != m_taskMap.end(); ++iter)
    {
        cur = iter->val;
        for (cur = iter->val; NULL != cur; cur = next)
        {
            next = cur->nextTimer;
            delete cur;
        }
    }
    m_taskMap.clear();
}

void Timer::init()
{
    
}

int Timer::setTimer(int msec)
{
    struct itimerval   itv;

    itv.it_interval.tv_sec = 0;
    itv.it_interval.tv_usec = 0;
    itv.it_value.tv_sec = msec / 1000;
    itv.it_value.tv_usec = (msec % 1000) * 1000;

    if (0 == setitimer(ITIMER_REAL, &itv, NULL)) return 0;

    ERRORLOG1("setitimer error: %s!", strerror(errno));

    return -1;
}

Timer::TaskNode *Timer::find(const char *name)
{
    for (TaskNode *tmp = m_taskListHead; NULL != tmp; tmp = tmp->nextTask)
    {
        if (0 == strcmp(name, tmp->name))      
        {
            return tmp;
        }
    }

    return NULL;
}

int Timer::addTimer(TaskNode *task)
{
    uint64_t startTime = EagleTimeI::instance().getMsec() + task->interval;
    TaskMap::Iterator iter = m_taskMap.find(startTime);

    if (m_taskMap.end() == iter)
    {
        task->nextTimer = NULL;
        m_taskMap.insert(startTime, task);
        if (1 == m_taskMap.size() || m_nextExcuteTime > startTime)
        {
            setTimer(task->interval);
            m_nextExcuteTime = startTime;
        }
    }else
    {
        task->nextTimer = iter->val;
        iter->val = task;
    }
}

void Timer::excute()
{
    TaskNode *cur;
    TaskNode *next;
    TaskMap::Iterator iter = m_taskMap.getMin();
    if (iter == m_taskMap.end()) return;
    
    LockGuard guard(m_lock);
    cur = iter->val;
    m_taskMap.erase(iter);
    while (NULL != cur)
    {
        next = cur->nextTimer;
        if (0 == *(cur->name)) 
        {
            delete cur;
        }else
        {
            if (cur->isAsync)
            {
                Thread thread(cur->cb);
            }else
            {
                cur->cb.excute();
            }
            if (-1 == cur->times || --cur->times > 0)
            {
                addTimer(cur);
            }else
            {
                delTask(cur);
                delete cur;
            }
        }
        cur = next;
    }
}

int Timer::addTask(const char *name, const int msec,
        const CallBack &cb, const int isAsync, const int times)
{
    LockGuard guard(m_lock);
    TaskNode *tmp = find(name);

    if (NULL != tmp)
    {
        ERRORLOG1("timer TaskNode %s is already exsit", name);

        return -1;
    }

    tmp = new TaskNode(name, msec, isAsync, times, cb);
    tmp->nextTask = m_taskListHead;
    m_taskListHead = tmp;
    addTimer(tmp);

    return 0;
}

void Timer::delTask(TaskNode *task)
{
    TaskNode *tmp = m_taskListHead;   
    if (tmp == task)
    {
        m_taskListHead = tmp->nextTask;

        return;
    }

    for (; NULL != tmp->nextTask; tmp = tmp->nextTask)
    {
        if (task == tmp->nextTask)
        {
            tmp->nextTask = task->nextTask;

            return;
        }
    }
}

int Timer::delTask(const char *name)
{
    LockGuard guard(m_lock);
    TaskNode *tmp = m_taskListHead;   

    if (0 == strcmp(name, tmp->name))
    {
        *(tmp->name) = 0;
        m_taskListHead = tmp->nextTask;

        return 0;
    }

    for (; NULL != tmp->nextTask; tmp = tmp->nextTask)
    {
        if (0 == strcmp(name, tmp->nextTask->name))      
        {
            *(tmp->nextTask->name) = 0;
            tmp->nextTask = tmp->nextTask->nextTask;

            return 0;
        }
    }

    ERRORLOG1("not find timer TaskNode %s", name);

    return -1;
}

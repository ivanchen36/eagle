#include <sys/time.h>
#include <signal.h>

#include "Timer.h"
#include "Thread.h"
#include "Define.h"
#include "Log.h"

namespace eagle
{

namespace
{
Timer *timer;
#ifdef _TEST_
void excuteTimer(union sigval val)
#else
void excuteTimer(int sig)
#endif
{
    timer->excute();
}
}

Timer::Timer() : m_isPause(0), m_nextExcuteTime((uint64_t)-1), 
    m_taskListHead(NULL), m_eagleTime(EagleTimeI::instance())
{ 
#ifdef _TEST_
    struct sigevent evp;

    memset (&evp, 0, sizeof (evp));
    evp.sigev_value.sival_ptr = &m_timer;
    evp.sigev_notify = SIGEV_THREAD;
    evp.sigev_notify_function = excuteTimer;

    if (0 != timer_create(CLOCK_REALTIME, &evp, &m_timer))
    {
        ERRORLOG1("timer_create err, %s", strerror(errno));
    }
#else
    struct sigaction sa;

    bzero(&sa, sizeof(struct sigaction)); 
    sa.sa_handler = excuteTimer;
    sigemptyset(&sa.sa_mask); 
    if (sigaction(SIGALRM, &sa, NULL) == -1) 
    {
        ERRORLOG2("sigaction %d err, %s", 
                SIGALRM, strerror(errno));
    }
#endif
    timer = this;
}

Timer::~Timer()
{
    TaskNode *cur;
    TaskNode *next;
    TaskMap::Iterator iter;

    pause();
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
#ifdef _TEST_
    if (0 != m_timer) timer_delete(m_timer);
#else
    signal(SIGALRM, SIG_DFL);
#endif
}

int Timer::setTimer(int msec)
{
#ifdef _TEST_
    struct itimerspec ts;

    m_isPause = msec > 0 ? 0 : 1;
    ts.it_interval.tv_sec = 0;
    ts.it_interval.tv_nsec = 0;
    ts.it_value.tv_sec = msec / 1000;
    ts.it_value.tv_nsec = (msec % 1000) * 1000000;

    if (0 == timer_settime(m_timer, 0, &ts, NULL)) return 0;
#else
    struct itimerval   itv;

    itv.it_interval.tv_sec = 0;
    itv.it_interval.tv_usec = 0;
    itv.it_value.tv_sec = msec / 1000;
    itv.it_value.tv_usec = (msec % 1000) * 1000;
    if (0 == setitimer(ITIMER_REAL, &itv, NULL)) return 0;
#endif

    ERRORLOG1("setitimer error: %s!", strerror(errno));

    return -1;
}

void Timer::start()
{
    uint64_t curTime = m_eagleTime.getMsec();
    LockGuard guard(m_lock);
    if (m_taskMap.empty()) return;

    if (m_nextExcuteTime > curTime)
    {
        setTimer(m_nextExcuteTime - curTime);
    }else
    {
        setTimer(1);
    }
}

void Timer::pause()
{
    LockGuard guard(m_lock);
    if (m_isPause) return;

    setTimer(0);
    if (sched_yield() != 0) 
        ERRORLOG1("sched_yield err, %s", strerror(errno));
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

void Timer::addTimer(const uint64_t startTime, TaskNode *task)
{
    TaskMap::Iterator iter = m_taskMap.find(startTime);

    if (m_taskMap.end() == iter)
    {
        task->nextTimer = NULL;
        m_taskMap.insert(startTime, task); 
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
    uint64_t tmp;
    uint64_t startTime = (uint64_t)-1;
    uint64_t curTime = m_eagleTime.getMsec();

    LockGuard guard(m_lock);
    TaskMap::Iterator iter = m_taskMap.getMin();

    if (curTime < m_nextExcuteTime) curTime = m_nextExcuteTime;
    for (; iter != m_taskMap.end(); iter = m_taskMap.getMin())
    {
        if (curTime < iter->key) break;

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
                    cur->cb.execute();
                }
                if (-1 == cur->times || --cur->times > 0)
                {
                    tmp = curTime + 1;
                    tmp = ALIGN(tmp, cur->interval);
                    addTimer(tmp, cur);
                    if (startTime > tmp) startTime = tmp;
                }else
                {
                    delTask(cur);
                    delete cur;
                }
            }
            cur = next;
        }
    }

    if (m_taskMap.end() == iter && (uint64_t)-1 == startTime)
    {
        m_isPause = 1;
        
        return;
    }

    if (m_taskMap.end() != iter && iter->key < startTime)
    {
        startTime = iter->key;
    }
    setTimer(startTime - curTime);
    m_nextExcuteTime = startTime;
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

    uint64_t curTime = m_eagleTime.getMsec();
    uint64_t startTime = curTime + 1;

    startTime = ALIGN(startTime, msec);
    tmp = new TaskNode(name, msec, isAsync, times, cb);
    tmp->nextTask = m_taskListHead;
    m_taskListHead = tmp;
    addTimer(startTime, tmp);
    if (m_nextExcuteTime > startTime)
    {
        setTimer(startTime - curTime);
        m_nextExcuteTime = startTime;
    }

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

    if (NULL == tmp) return -1;
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

    return -1;
}

}

#include <sys/time.h>
#include <signal.h>

#include "Timer.h"
#include "Thread.h"
#include "Define.h"
#include "Log.h"

namespace
{
void startTimerThread(void *param)
{
    ((eagle::Timer *)param)->loop();
}
}

namespace eagle
{

__attribute__((constructor)) void startTimer()
{
    TimerI::instance().start();
}

Timer::Timer() : m_status(STOP), m_nextExecuteTime((uint64_t)-1), 
    m_taskListHead(NULL), m_thread(NULL), m_serverTime(ServerTimeI::instance())
{ 
}

Timer::~Timer()
{
    stop();
}

void Timer::start()
{
    LockGuard<SpinLock> guard(m_lock);
    CallBack cb(startTimerThread, (void *)this);

    m_status = START; 
    m_thread = new Thread(cb, 0);
}

void Timer::pause()
{
    LockGuard<SpinLock> guard(m_lock);
    if (m_status != START) return;

    m_status = PAUSE;
    quitThread();
}

void Timer::quitThread()
{
    if (NULL == m_thread) return;

    m_sem.post();
    if (sched_yield() != 0) 
        ERRORLOG1("sched_yield err, %s", strerror(errno));
    if (NULL != m_thread) delete m_thread;
    m_thread = NULL;
}

void Timer::stop()
{
    LockGuard<SpinLock> guard(m_lock);
    if (m_status == STOP) return;

    TaskNode *cur;
    TaskNode *next;
    TaskMap::Iterator iter;

    m_status = STOP;
    quitThread();
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
    m_taskListHead = NULL;
}

void Timer::loop()
{
    int ret;
    static const uint64_t &curTime = ServerTimeI::instance().getMsec();

    for (;;)
    {
        ret = m_sem.timedWait(m_nextExecuteTime - curTime);

        if (EG_FAILED == ret) break;
        if (EG_AGAIN == ret)
        {
            execute();

            continue;
        }

        if (m_status != START) break;
    }
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

void Timer::execute()
{
    TaskNode *cur;
    TaskNode *next;
    uint64_t tmp;
    uint64_t startTime = (uint64_t)-1;
    static const uint64_t &msec = ServerTimeI::instance().getMsec();
    uint64_t curTime = msec < m_nextExecuteTime ? m_nextExecuteTime : msec;

    LockGuard<SpinLock> guard(m_lock);
    TaskMap::Iterator iter = m_taskMap.getMin();

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

    if (m_taskMap.end() != iter && iter->key < startTime)
    {
        startTime = iter->key;
    }
    m_nextExecuteTime = startTime;
}

int Timer::addTask(const char *name, const int msec,
        const CallBack &cb, const int isAsync, const int times)
{
    static const uint64_t &curTime = ServerTimeI::instance().getMsec();

    LockGuard<SpinLock> guard(m_lock);
    TaskNode *tmp = find(name);

    if (NULL != tmp)
    {
        ERRORLOG1("timer TaskNode %s is already exsit", name);

        return -1;
    }

    uint64_t startTime = curTime + 1;

    startTime = ALIGN(startTime, msec);
    tmp = new TaskNode(name, msec, isAsync, times, cb);
    tmp->nextTask = m_taskListHead;
    m_taskListHead = tmp;
    addTimer(startTime, tmp);
    if (m_nextExecuteTime > startTime)
    {
        m_nextExecuteTime = startTime;
        m_sem.post();
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
    LockGuard<SpinLock> guard(m_lock);
    TaskNode *tmp = m_taskListHead;   

    if (NULL == tmp) return -1;

    if (NULL != tmp->name && 0 == strcmp(name, tmp->name))
    {
        *(tmp->name) = 0;
        m_taskListHead = tmp->nextTask;

        return 0;
    }

    for (; NULL != tmp->nextTask; tmp = tmp->nextTask)
    {
        if (NULL != tmp->name && 0 == strcmp(name, tmp->nextTask->name))      
        {
            *(tmp->nextTask->name) = 0;
            tmp->nextTask = tmp->nextTask->nextTask;

            return 0;
        }
    }

    return -1;
}

}

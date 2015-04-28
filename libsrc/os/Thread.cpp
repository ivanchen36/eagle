#include "Log.h"
#include "Thread.h"

void *threadStart(void *arg)
{
    ((Thread *)arg)->doTask();
}

Task::Task(const char *taskName) : m_name(NULL)
{
    m_name = new char[strlen(taskName) + 1];
    strcpy(m_name, taskName);
}

Task::~Task()
{
    if (m_name) delete []m_name;
}

const char *Task::getName()
{
    return m_name;
}

Thread::Thread(TaskPtr &task, const int stackSize) 
: m_sem(new ThreadSem()), m_isInit(0), m_task(task),
    m_status(STOP)
{
    pthread_attr_t attr;
    int ret = pthread_attr_init(&attr);
    if (ret)
    {
        ERRORLOG1("pthread_attr_init err, ret %d", ret);

        return;
    }

    ret = pthread_create(&m_id, &attr, threadStart, (void *)this);
    if (ret)
    {
        ERRORLOG1("pthread_create err, ret %d", ret);

        return;
    }

    m_isInit = 1;
    m_status = WAIT;
    ret = pthread_attr_destroy(&attr);
    if (ret) ERRORLOG1("pthread_attr_destroy err, ret %d", ret);
}

Thread::~Thread()
{
    if (!m_isInit) return;

    stop();
}

int Thread::isStop()
{
   return STOP == m_status;
}

int Thread::isWait()
{
   return WAIT == m_status;
}

int Thread::isRun()
{
    return RUN == m_status;
}

int Thread::start()
{
    if (STOP == m_status) return -1;

    if (WAIT == m_status) m_status = RUN;
    m_sem->post();
}

int Thread::pause()
{
    if (WAIT == m_status) return 0;
    if (STOP == m_status) return -1;

    m_status = WAIT;
}

int Thread::stop()
{
    if (STOP == m_status) return 0;

    int ret;

    m_status = STOP;
    m_sem->post();
    ret = pthread_join(m_id, NULL);
    if (ret)
    {
        ERRORLOG1("pthread_create err, ret %d", ret);
    }else
    {
        INFOLOG1("%s thread exit", m_task->getName());
    }
}

void Thread::doTask()
{
    int ret;

    while (STOP != m_status)
    {
        ret = m_sem->timedWait(30);
        if (ret < 0)
        {
            ERRORLOG("sem wait err, thread stop");    
            m_status = STOP;

            return;
        }
        if (WAIT == m_status)
        {
            INFOLOG1("%s thread ...", m_task->getName());
            continue;
        }

        while (RUN == m_status && !m_task->excute());
        if (STOP == m_status) return;

        if (WAIT != m_status) m_status = WAIT;
    }
}

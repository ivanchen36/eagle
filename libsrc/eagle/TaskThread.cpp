#include "Log.h"
#include "TaskThread.h"

Task::Task(const char *taskName) : m_name(NULL)
{
    m_name = new char[strlen(taskName) + 1];
    strcpy(m_name, taskName);
}

Task::~Task()
{
    if (NULL != m_name) delete []m_name;
}

const char *Task::getName()
{
    return m_name;
}

TaskThread::TaskThread(const TaskPtr &task, const int stackSize) 
: Thread(stackSize), m_status(WAIT), m_sem(new ThreadSem()), m_task(task)
{
}

TaskThread::~TaskThread()
{
    stop();
}

int TaskThread::isStop()
{
    LockGuard guard(m_lock);

    return STOP == m_status;
}

int TaskThread::isWait()
{
    LockGuard guard(m_lock);

    return WAIT == m_status;
}

int TaskThread::isRun()
{
    LockGuard guard(m_lock);

    return RUN == m_status;
}

int TaskThread::start()
{
    LockGuard guard(m_lock);

    if (STOP == m_status) return EG_FAILED;

    if (WAIT == m_status) m_status = RUN;
    m_sem->post();

    return EG_SUCCESS;
}

int TaskThread::pause()
{
    LockGuard guard(m_lock);

    if (WAIT == m_status) return EG_SUCCESS;
    if (STOP == m_status) return EG_FAILED;

    m_status = WAIT;

    return EG_SUCCESS;
}

int TaskThread::stop()
{
    LockGuard guard(m_lock);

    if (STOP == m_status) return EG_SUCCESS;

    m_status = STOP;
    m_sem->post();

    return EG_SUCCESS;
}

void TaskThread::run()
{
    int ret;

    for (; ;)
    {
        {
            LockGuard guard(m_lock);
            if (STOP == m_status) return;

            if (WAIT != m_status) m_status = WAIT;
        }
        ret = m_sem->wait();
        if (ret < 0)
        {
            ERRORLOG1("sem wait err, %s task stop", m_task->getName());    
            m_status = STOP;

            return;
        }

        while (RUN == m_status && m_task->excute() == EG_SUCCESS);
    }
}

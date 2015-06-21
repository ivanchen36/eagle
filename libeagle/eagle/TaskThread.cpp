#include "Log.h"
#include "StrUtil.h"
#include "TaskThread.h"

Task::Task(const char *taskName) : m_name(NULL)
{
    StrUtil::copy(m_name, taskName);
}

Task::~Task()
{
    if (NULL != m_name) delete []m_name;
}

const char *Task::getName()
{
    return m_name;
}

TaskThread::TaskThread(Task *task, const int stackSize) 
: Thread(stackSize), m_status(WAIT), m_sem(new ThreadSem()), m_task(task)
{
}

TaskThread::~TaskThread()
{
    stop();
    while (EXIT != m_status) sched_yield();
    if (NULL != m_sem) delete m_sem;
}

int TaskThread::isStop()
{
    return STOP == m_status;
}

int TaskThread::isWait()
{
    return WAIT == m_status;
}

int TaskThread::isRun()
{
    return RUN == m_status;
}

int TaskThread::start()
{
    if (STOP == m_status || EXIT == m_status) return EG_FAILED;

    if (WAIT == m_status) m_status = RUN;
    m_sem->post();

    return EG_SUCCESS;
}

int TaskThread::pause()
{
    if (WAIT == m_status) return EG_SUCCESS;
    if (STOP == m_status || EXIT == m_status) return EG_FAILED;

    m_status = WAIT;

    return EG_SUCCESS;
}

int TaskThread::stop()
{
    if (STOP == m_status || EXIT == m_status) return EG_SUCCESS;

    m_status = STOP;
    m_sem->post();
    sched_yield();

    return EG_SUCCESS;
}

void TaskThread::run()
{
    int ret;

    for (; ;)
    {
        if (STOP == m_status) break;

        if (WAIT != m_status) m_status = WAIT;
        ret = m_sem->wait();
        if (ret < 0)
        {
            ERRORLOG1("sem wait err, %s task stop", m_task->getName());    
            m_status = STOP;

            break;
        }

        while (RUN == m_status && m_task->excute() == EG_SUCCESS);
    }
    m_status = EXIT;
}

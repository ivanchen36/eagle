#include "Log.h"
#include "TaskThread.h"

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

TaskThread::TaskThread(TaskPtr &task, const int stackSize) 
: Thread(stackSize), m_sem(new ThreadSem()), m_task(task), m_status(WAIT)
{
}

TaskThread::~TaskThread()
{
    stop();
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
    if (STOP == m_status) return -1;

    if (WAIT == m_status) m_status = RUN;
    m_sem->post();
}

int TaskThread::pause()
{
    if (WAIT == m_status) return 0;
    if (STOP == m_status) return -1;

    m_status = WAIT;
}

int TaskThread::stop()
{
    if (STOP == m_status) return 0;

    int ret;

    m_status = STOP;
    m_sem->post();
}

void TaskThread::run()
{
    int ret;

    while (STOP != m_status)
    {
        ret = m_sem->timedWait(30);
        if (ret < 0)
        {
            ERRORLOG1("sem wait err, %s task stop", m_task->getName());    
            m_status = STOP;

            return;
        }
        if (WAIT == m_status)
        {
            INFOLOG1("%s task ...", m_task->getName());
            continue;
        }

        while (RUN == m_status && !m_task->excute());
        if (STOP == m_status) return;

        if (WAIT != m_status) m_status = WAIT;
    }
}

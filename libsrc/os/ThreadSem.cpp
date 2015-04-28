#include <time.h>

#include "ThreadSem.h"
#include "Log.h"

ThreadSem::ThreadSem(const int val) : m_isInit(0)
{
    if (sem_init(&m_sem, 0, 0)) 
    {   
        ERRORLOG1("sem_init failed, %s", strerror(errno));

        return;
    }
    m_isInit = 1;
}

ThreadSem::~ThreadSem()
{
    if (sem_destroy(&m_sem))
    {
        ERRORLOG1("sem_destroy failed, %s", strerror(errno));
    }
}

int ThreadSem::post()
{
    if (!m_isInit) return -1;

    if (sem_post(&m_sem))
    {
        ERRORLOG1("sem_post failed, %s", strerror(errno));

        return -1;
    } 

    return 0;
}

int ThreadSem::wait()
{
    if (!m_isInit) return -1;

    if (sem_wait(&m_sem))
    {
        ERRORLOG1("sem_wait failed, %s", strerror(errno));

        return -1;
    } 

    return 0;
}

int ThreadSem::timedWait(const int sec)
{
    if (!m_isInit) return -1;

    struct timespec t;
    if (clock_gettime(CLOCK_REALTIME, &t) == -1)
    {
        ERRORLOG1("clock_gettime failed, %s", strerror(errno));
        t.tv_sec = time(NULL);
        t.tv_nsec = 0;
    }
    t.tv_sec += sec;
    if (sem_timedwait(&m_sem, &t))
    {
        if (ETIMEDOUT == errno || EAGAIN == errno) return 1;

        ERRORLOG1("sem_timedwait failed, %s", strerror(errno));

        return -1;
    }

    return 0;
}

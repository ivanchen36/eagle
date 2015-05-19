#include <time.h>

#include "ThreadSem.h"
#include "Log.h"
#include "EagleTime.h"

ThreadSem::ThreadSem(const int val) : m_isInit(0)
{
    if (sem_init(&m_sem, 0, 0) != 0) 
    {   
        ERRORLOG1("sem_init failed, %s", strerror(errno));

        return;
    }
    m_isInit = 1;
}

ThreadSem::~ThreadSem()
{
    if (sem_destroy(&m_sem) != 0)
    {
        ERRORLOG1("sem_destroy failed, %s", strerror(errno));
    }
}

int ThreadSem::post()
{
    if (!m_isInit) return EG_INVAL;

    if (sem_post(&m_sem) != 0)
    {
        ERRORLOG1("sem_post failed, %s", strerror(errno));

        return EG_FAILED;
    } 

    return EG_SUCCESS;
}

int ThreadSem::wait()
{
    if (!m_isInit) return EG_INVAL;

    if (sem_wait(&m_sem) != 0)
    {
        ERRORLOG1("sem_wait failed, %s", strerror(errno));

        return EG_FAILED;
    } 

    return EG_SUCCESS;
}

int ThreadSem::timedWait(const int sec)
{
    if (!m_isInit) return EG_INVAL;

    int ret;
    struct timespec t;

    t.tv_sec = EagleTimeI::instance().getSec() + sec;
    t.tv_nsec = 0;
    t.tv_sec += sec;
    for (; ;)
    {
        ret = sem_timedwait(&m_sem, &t);
        if (ret == 0) return EG_SUCCESS;

        if (EINTR == errno) continue;

        if (ETIMEDOUT == errno) return EG_AGAIN;

        ERRORLOG1("sem_timedwait failed, %s", strerror(errno));

        return EG_FAILED;
    }

    return EG_SUCCESS;
}

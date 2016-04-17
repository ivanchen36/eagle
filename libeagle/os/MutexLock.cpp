#include "MutexLock.h"
#include "Log.h"

namespace eagle
{

MutexLock::MutexLock(const int isRecursive, const int isPshared)
{
    int ret;
    pthread_mutexattr_t attr;

    pthread_mutexattr_init(&attr);
    if (isRecursive)
    {
        ret = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
        if (ret != 0) ERRORLOG1("pthread_mutexattr_settype err, %s", strerror(ret));
    }
    if (isPshared)
    {
        ret = pthread_mutexattr_setpshared(&attr,PTHREAD_PROCESS_SHARED);
        if (ret != 0) ERRORLOG1("pthread_mutexattr_setpshared err, %s", strerror(ret));
    }
    ret = pthread_mutex_init(&m_mutex, &attr);
    if (ret != 0) ERRORLOG1("pthread_mutex_init err, %s", strerror(ret));
    pthread_mutexattr_destroy(&attr);
}

MutexLock::~MutexLock()
{
    int ret = pthread_mutex_destroy(&m_mutex);
    if (ret != 0) ERRORLOG1("pthread_mutex_destroy err, %s", strerror(ret));
}

int MutexLock::lock()
{
    int ret = pthread_mutex_lock(&m_mutex);
    if (ret != 0)
    {
        ERRORLOG1("pthread_mutex_lock err, %s", strerror(ret));

        return -1;
    }

    return 0;
}

int MutexLock::unLock()
{
    int ret = pthread_mutex_unlock(&m_mutex);
    if (ret != 0)
    {
        ERRORLOG1("pthread_mutex_unlock err, %s", strerror(ret));

        return -1;
    }

    return 0;
}

int MutexLock::tryLock()
{
    int ret = pthread_mutex_trylock(&m_mutex);
    if (ret != 0)
    {
        if (EBUSY == ret) return 1;

        ERRORLOG1("pthread_mutex_trylock err, %s", strerror(ret));

        return -1;
    }

    return 0;
}

}

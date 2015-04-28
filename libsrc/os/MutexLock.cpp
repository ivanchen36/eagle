#include "MutexLock.h"
#include "Log.h"

MutexLock::MutexLock()
{
    int ret = pthread_mutex_init(&m_mutex, NULL);
    if (ret) ERRORLOG1("pthread_mutex_init err, ret %d", ret);
}

MutexLock::~MutexLock()
{
    int ret = pthread_mutex_destroy(&m_mutex);
    if (ret) ERRORLOG1("pthread_mutex_destroy err, ret %d", ret);
}

int MutexLock::lock()
{
    int ret = pthread_mutex_lock(&m_mutex);
    if (ret)
    {
        ERRORLOG1("pthread_mutex_lock err, ret %d", ret);

        return -1;
    }

    return 0;
}

int MutexLock::unLock()
{
    int ret = pthread_mutex_unlock(&m_mutex);
    if (ret)
    {
        ERRORLOG1("pthread_mutex_unlock err, ret %d", ret);

        return -1;
    }

    return 0;
}

int MutexLock::tryLock()
{
    int ret = pthread_mutex_trylock(&m_mutex);
    if (ret)
    {
        if (EBUSY == ret) return 1;

        ERRORLOG1("pthread_mutex_trylock err, ret %d", ret);

        return -1;
    }

    return 0;
}

LockGuard::LockGuard(MutexLockPtr &lock) : m_lock(lock)
{
    m_lock->lock();
}

LockGuard::~LockGuard()
{
    m_lock->unLock();
}

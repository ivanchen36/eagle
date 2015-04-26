#include "Lock.h"

Lock::Lock(const pthread_mutex_t lock):m_lock(lock)
{
    pthread_mutex_lock(&m_lock);
}

Lock::~Lock()
{
    pthread_mutex_unlock(&m_lock);
}

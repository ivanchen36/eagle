#include "Log.h"
#include "Thread.h"

void *threadStart(void *arg)
{
    Thread *thread = (Thread *)arg;
    thread->run();
}

Thread::Thread(const CallBack &cb, const int isDetach, const int stackSize)
    : m_isDetach(isDetach), m_cb(cb), m_id(0)
{
    init(stackSize);
}

Thread::Thread(const int stackSize) 
    : m_isDetach(0), m_id(0)
{
    init(stackSize);
}

void Thread::init(const int stackSize) 
{
    pthread_attr_t attr;
    int ret = pthread_attr_init(&attr);
    if (ret != 0)
    {
        ERRORLOG1("pthread_attr_init err, ret %d", ret);

        return;
    }

    if (stackSize > 0)
    {
        ret = pthread_attr_setstacksize(&attr, stackSize);
        if (ret != 0) ERRORLOG1("pthread_attr_setstacksize, ret %d", ret);
    }

    if (m_isDetach)
    {
        pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
    }

    ret = pthread_create(&m_id, &attr, threadStart, (void *)this);
    if (ret != 0)
    {
        m_id = 0;
        ERRORLOG1("pthread_create err, ret %d", ret);

        return;
    }

    ret = pthread_attr_destroy(&attr);
    if (ret != 0) ERRORLOG1("pthread_attr_destroy err, ret %d", ret);
}

void Thread::run()
{
    if (m_cb.excute())
    {
        ERRORLOG("cb is NULL or thread object has destructed");
    }
}

Thread::~Thread()
{
    if (m_isDetach) return;
    if (0 == m_id) return;

    int ret = pthread_join(m_id, NULL);
    if (ret != 0)
    {
        ERRORLOG1("pthread_join err, ret %d", ret);
    }
}

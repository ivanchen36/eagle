#include "Log.h"
#include "Thread.h"

void *threadStart(void *arg)
{
    ((Thread *)arg)->run();
}

Thread::Thread(const int stackSize) 
: m_id(0)
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

Thread::~Thread()
{
    if (0 == m_id) return;

    int ret = pthread_join(m_id, NULL);
    if (ret != 0)
    {
        ERRORLOG1("pthread_join err, ret %d", ret);
    }
}

#include "Log.h"
#include "Thread.h"

void *threadStart(void *arg)
{
    ((Thread *)arg)->run();
}

Thread::Thread(const int stackSize) 
: m_isInit(0)
{
    pthread_attr_t attr;
    int ret = pthread_attr_init(&attr);
    if (ret)
    {
        ERRORLOG1("pthread_attr_init err, ret %d", ret);

        return;
    }

    if (stackSize > 0)
    {
        ret = pthread_attr_setstacksize(&attr, stackSize);
        if (ret) ERRORLOG1("pthread_attr_setstacksize, ret %d", ret);
    }

    ret = pthread_create(&m_id, &attr, threadStart, (void *)this);
    if (ret)
    {
        ERRORLOG1("pthread_create err, ret %d", ret);

        return;
    }

    m_isInit = 1;
    ret = pthread_attr_destroy(&attr);
    if (ret) ERRORLOG1("pthread_attr_destroy err, ret %d", ret);
}

Thread::~Thread()
{
    if (!m_isInit) return;

    int ret = pthread_join(m_id, NULL);
    if (ret)
    {
        ERRORLOG1("pthread_join err, ret %d", ret);
    }
}

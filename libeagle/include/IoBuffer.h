/**
 *       Filename:  IoBuffer.h
 *
 *    Description:  io buffer
 *
 *        Version:  1.0
 *        Created:  06/23/2015 11:37:18 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan Chen, 228268157@qq.com
 *   Organization:  
 */
#ifndef _EAGLE_IOBUFFER_H_
#define _EAGLE_IOBUFFER_H_
#include <sched.h>

#include "Define.h"
#include "Pool.h"
#include "Singleton.h"

namespace eagle
{

struct IoBuffer
{
    IoBuffer *next;
    uint16_t offset;
    uint16_t cur;
    uint8_t buf[1];
    const static uint16_t size;

    IoBuffer()
    {
        cur = 0;
        offset = 0;
        next = NULL;
    }

    void reset()
    {
        cur = 0;
        offset = 0;
        next = NULL;
    }

    void *operator new(size_t s)
    {
        return (void *)new char [EG_IO_BUF_SIZE];
    }

    void operator delete(void* ptr)
    {
        delete [] (char *)ptr;
    }
};

union IoBufferUnion
{
    struct BufStruct
    {
        BufStruct *next;
        uint16_t offset;
        uint16_t cur;
        uint8_t buf[1];
    } ioBuf;
    char buf[EG_IO_BUF_SIZE];
};

class IoBufPoolManager
{
public:
    ~IoBufPoolManager()
    {
        if (NULL == m_bufPools) return;

        for (int i = 0; i < WORKER_THREAD_NUM; ++i)
        {
            m_bufPools[i].~Pool<IoBufferUnion>();
        }

        delete [] (char *)m_bufPools;
    }

    Pool<IoBufferUnion> *getBufPool()
    {
        int index = __sync_fetch_and_add(const_cast<volatile int *>(&m_poolIndex), 1);

        if (index >= WORKER_THREAD_NUM)
        {
            index = WORKER_THREAD_NUM % index; 
            m_poolIndex = WORKER_THREAD_NUM % m_poolIndex;
        }

        return m_bufPools + index;
    }

private:
    IoBufPoolManager()
    {
        m_bufPools = (Pool<IoBufferUnion> *)new char[sizeof(Pool<IoBufferUnion>) * WORKER_THREAD_NUM];
        for (int i = 0; i < WORKER_THREAD_NUM; ++i)
        {
            new(&m_bufPools[i]) Pool<IoBufferUnion>(EG_MAX_EVENTHANDLER / WORKER_THREAD_NUM + 1);
        }
    }

    int m_poolIndex;
    Pool<IoBufferUnion> *m_bufPools;

    friend class Singleton<IoBufPoolManager>;
};

typedef Singleton<IoBufPoolManager> IoBufPoolManagerI;

}
#endif   /* ----- #ifndef _EAGLE_IOBUFFER_H_  ----- */

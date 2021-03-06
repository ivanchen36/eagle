/**
 *       Filename:  CycleQueue.h
 *
 *    Description:  cycle queue
 *
 *        Version:  1.0
 *        Created:  05/23/2015 02:26:26 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan Chen, 228268157@qq.com
 *   Organization:  
 */
#ifndef _EAGLE_CYCLEQUEUE_H_
#define _EAGLE_CYCLEQUEUE_H_

#include "Define.h"
#include "SpinLock.h"
#include "Log.h"

namespace eagle
{

#define STATS_SIZE_MAX 0x6FFFFFFF

template<class T> class CycleQueue 
{
public:
    CycleQueue(const int s): m_size(s),
        m_pushSize(0), m_popSize(0)
    {
        m_nodeHead = new Node[m_size];
        m_head = m_nodeHead;
        m_tail = m_head;
        initNodeList(m_nodeHead, m_size);
    }

    ~CycleQueue()
    {
        if (m_nodeHead) delete [] m_nodeHead; 
    }

    int size()
    {
        LockGuard<SpinLock> guard(m_lock);

        return m_pushSize - m_popSize;
    }

    int empty()
    {
        return m_head == m_tail;
    }

    int full()
    {
        return m_head->next == m_tail;
    }

    int capacity()
    {
        return m_size;
    }

    int push(T &val)
    { 
        if(m_head->next == m_tail)
        {
            ERRORLOG("cycle queue is full");

            return EG_FAILED;
        }

        m_head->val = val;
        m_head = m_head->next;
        ++m_pushSize;
        if (m_popSize > STATS_SIZE_MAX)
        {
            LockGuard<SpinLock> guard(m_lock);
            m_pushSize -= m_popSize;
            m_popSize = 0;
        }

        return EG_SUCCESS;
    }

    int pop(T &val)
    {
        if(empty()) return EG_FAILED;

        val = m_tail->val;
        m_tail = m_tail->next;
        if (m_popSize > STATS_SIZE_MAX)
        {
            LockGuard<SpinLock> guard(m_lock);
            ++m_popSize;

            return EG_SUCCESS;
        }

        ++m_popSize;
        return EG_SUCCESS;
    } 

private:
    struct Node
    {
        T val;
        Node *next;
    };

    void initNodeList(Node *arr, const int arrLen)
    {
        int len = arrLen - 1;
        for(int i = 0; i < len; ++i)
        {
            (arr + i)->next = arr + i + 1;
        }
        (arr + len)->next = arr;
    }

    int m_size;
    uint32_t m_pushSize;
    uint32_t m_popSize;
    Node *m_head;
    Node *m_tail;
    Node *m_nodeHead;
    SpinLock m_lock;
};

}
#endif   /* ----- #ifndef _EAGLE_CYCLEQUEUE_H_  ----- */

/**
 *       Filename:  Pool.h
 *
 *    Description:  pool 
 *
 *        Version:  1.0
 *        Created:  05/23/2015 06:53:52 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan Chen, 228268157@qq.com
 *   Organization:  
 */
#ifndef _EAGLE_POOL_H_
#define _EAGLE_POOL_H_

#include "MutexLock.h"

namespace eagle
{

template <class T> class Pool
{
public:
    Pool(const int initSize) : m_capacity(initSize),
        m_btHead(NULL)
    {
        spawnNode();
    }

    ~Pool()
    {
        for (Bucket *bt = m_btHead; NULL != bt; bt = m_btHead)
        {
            m_btHead = m_btHead->next;
            delete [] bt->node;
            delete bt;
        }
    }

    T *pop()
    {
        LockGuard guard(m_lock);

        if (0 == m_size)
        {
            spawnNode();
        }

        Node *n = m_head;
        m_head = m_head->next; 
        --m_size;

        return (T *)n;
    }

    void push(T *val)
    {
        LockGuard guard(m_lock);

        Node *n = (Node *)val;

        n->next = m_head;
        m_head = n;
        ++m_size;
    }

private:
    struct Node
    {
        T val;
        Node *next;
    };

    struct Bucket
    {
        Node *node;
        Bucket *next;
    };

    void spawnNode()
    {
        Bucket *bt = new Bucket();

        bt->next = m_btHead;
        m_btHead = bt;
        bt->node = new Node[m_capacity];
        m_head = bt->node;
        initNodeList(bt->node, m_capacity);

        m_size = m_capacity;
        m_capacity += m_capacity;
    }

    void initNodeList(Node *arr, const int size)
    {
        int len = size - 1;
        for(int i = 0; i < len; ++i)
        {
            (arr + i)->next = arr + i + 1;
        }
        (arr + len)->next = NULL;
    }

    int m_size;
    int m_capacity;
    Node *m_head;
    Bucket *m_btHead;
    MutexLock m_lock;
};

}
#endif   /* ----- #ifndef _EAGLE_POOL_H_  ----- */

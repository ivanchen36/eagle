/**
 *       Filename:  ConsistentHashing.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  04/01/2016 01:16:09 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan Chen, 228268157@qq.com
 *   Organization:  
 */
#ifndef _CONSISTENTHASHING_H_
#define _CONSISTENTHASHING_H_

#include <stdint.h>
#include <string.h>
#include <map>
#include <vector>
#include <iostream>

#include "AlgoBase.h"
#include "AutoPtr.h"
#include "SpinLock.h"

namespace eagle
{
#define WORKER_SCALING 100

template<class T> class ConsistentHashing 
{
public:
    class Operator
    {
    public:
        virtual void dispatch(T &t) = 0;

    };

    ConsistentHashing(const int scalling = WORKER_SCALING)
        : m_scalling(scalling)
    {
    }

    ~ConsistentHashing()
    {
        typename std::map<int, VirtualObj*>::iterator iter; 

        for (iter = m_indexMap.begin(); iter != m_indexMap.end(); ++iter)
        {
            delete [](iter->second);
        }
    }

    int regist(int index)
    {
        VirtualObj *arr;
        AutoPtr<Operator> nullOperator;
        typename std::map<int, VirtualObj*>::iterator iter; 

        LockGuard<SpinLock> lock(m_lock);
        iter = m_indexMap.find(index); 
        if (m_indexMap.end() != iter) return -1;

        insertVirObj(index, nullOperator);

        return 0;
    }

    int regist(int index, AutoPtr<Operator> &op)
    {
        VirtualObj *arr;
        typename std::map<int, VirtualObj*>::iterator iter; 

        LockGuard<SpinLock> lock(m_lock);
        iter = m_indexMap.find(index); 
        if (m_indexMap.end() != iter) return -1;

        insertVirObj(index, op);

        return 0;
    }

    int unregist(int index)
    {
        VirtualObj *arr;
        typename std::map<int, VirtualObj*>::iterator iter; 
        std::vector<uint64_t *>::iterator iter1;

        LockGuard<SpinLock> lock(m_lock);
        iter = m_indexMap.find(index); 
        if (m_indexMap.end() == iter) return -1;

        arr = iter->second; 
        for (int i = 0; i < m_scalling; ++i)
        {
            if (!arr[i].isConflict)
            {
                iter1 = std::lower_bound(m_virList.begin(), m_virList.end(), 
                        (uint64_t *)(&arr[i]), pointerCompare<uint64_t *>);
                if (m_virList.end() != iter1) m_virList.erase(iter1);
            }
        }
        delete []arr;
        m_indexMap.erase(iter);
    }

    int push(const int key, T &t)
    {
        if (m_virList.empty()) return -1;

        static int intSize = sizeof(int);

        std::vector<uint64_t *>::iterator iter;
        uint64_t val = murmurHash((const uint8_t *)&key, intSize);
        std::cout << "int hashval " << val << std::endl;

        iter = std::lower_bound(m_virList.begin(), m_virList.end(), 
                        (uint64_t *)&val, pointerCompare<uint64_t *>);
        if (m_virList.end() == iter)
        {
            AutoPtr<Operator> &op = ((VirtualObj *)m_virList[0])->op;
            if (op) op->dispatch(t);

            return 0;
        }

        AutoPtr<Operator> &op = ((VirtualObj *)*iter)->op;
        if (op) op->dispatch(t);

        return 0;
    }

    int push(const std::string &key, T &t)
    {
        if (m_virList.empty()) return -1;

        static int intSize = sizeof(int);

        std::vector<uint64_t *>::iterator iter;
        uint64_t val = murmurHash((const uint8_t *)key.c_str(), key.length());
        std::cout << "string hashval " << val << std::endl;

        iter = std::lower_bound(m_virList.begin(), m_virList.end(), 
                        (uint64_t *)&val, pointerCompare<uint64_t *>);
        if (m_virList.end() == iter)
        {
            AutoPtr<Operator> &op = ((VirtualObj *)m_virList[0])->op;
            if (op) op->dispatch(t);

            return 0;
        }

        AutoPtr<Operator> &op = ((VirtualObj *)*iter)->op;
        if (op) op->dispatch(t);

        return 0;
    }

private:
    struct VirtualObj
    {
        uint64_t val; 
        uint32_t index; 
        uint32_t isConflict; 
        AutoPtr<Operator> op;
    };

    void insertVirObjBySort(const int index, VirtualObj *obj)
    {   
        std::vector<uint64_t *>::iterator iter;

        iter = std::lower_bound(m_virList.begin(), m_virList.end(), 
                (uint64_t *)obj, pointerCompare<uint64_t *>);
        if (m_virList.end() == iter)
        {
            m_virList.push_back((uint64_t *)obj);

            return;
        }

        VirtualObj *&tmp = (VirtualObj *&)(*iter);
        if (tmp->val != obj->val)
        {
            m_virList.insert(iter, (uint64_t *)obj);

            return;
        }

        if (tmp->index < obj->index)
        {
            obj->isConflict = 1;

            return;
        }

        tmp->isConflict = 1;
        tmp = obj;
    }

    void insertVirObj(const int index, AutoPtr<Operator> &op)
    {
        char buf[64];
        VirtualObj *arr = new VirtualObj[m_scalling];

        m_indexMap[index] = arr;
        m_virList.reserve(m_virList.size() + m_scalling);
        for (int i = 0; i < m_scalling; ++i)
        {
            sprintf(buf, "index.%d.seq.%d", index, i);
            arr[i].val = murmurHash((const uint8_t *)buf, strlen(buf));
            arr[i].index = index;
            arr[i].isConflict = 0;
            arr[i].op = op;
            insertVirObjBySort(index, &arr[i]);
        }
    }

    int m_scalling;
    SpinLock m_lock;
    std::vector<uint64_t *> m_virList;
    std::map<int, VirtualObj*> m_indexMap;
};

}
#endif   /* ----- #ifndef _CONSISTENTHASHING_H_  ----- */

/**
 *       Filename:  AutoPtr.h
 *
 *    Description:  smart pointer
 *
 *        Version:  1.0
 *        Created:  04/19/2015 11:19:07 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan Chen, 228268157@qq.com
 *   Organization:  
 */


#ifndef  _AUTOPTR_H_
#define  _AUTOPTR_H_

#include <algorithm>

#include "OsApi.h"

template<class T> class AutoPtr 
{
public:
    AutoPtr()
    {
        m_ptr = new T();
        m_ref = new int(1); 
    }

    AutoPtr(T *ptr)
    {
        m_ptr = ptr;
        if (!m_ptr) m_ptr = new T();
        m_ref = new int(1); 
    }

    AutoPtr(const AutoPtr &r)
    {
        m_ptr = r.m_ptr;
        m_ref = r.m_ref;
        OsApi::atomicAdd(*m_ref, 1);
    }

    template<class Y> AutoPtr(const AutoPtr<Y> &r)
    {
        m_ptr = r.m_ptr;
        m_ref = r.m_ref;
        OsApi::atomicAdd(*m_ref, 1);
    }

    ~AutoPtr()
    {
        OsApi::atomicSub(*m_ref, 1);
        if (!*m_ref)
        {
            delete m_ref;
            delete m_ptr;
        }
    }


    T *operator->() const
    {
        return m_ptr;
    }

    T &operator*() const
    {
        return *m_ptr;
    }

    operator bool() const
    {
        return m_ptr ? true : false;
    }

    void swap(AutoPtr &r)
    {
        std::swap(m_ptr, r.m_ptr);
    }

    AutoPtr& operator=(T *p)
    {
        OsApi::atomicSub(*m_ref, 1);
        if (!*m_ref)
        {
            delete m_ref;
            delete m_ptr;
        }

        m_ptr = p;
        if (!m_ptr) m_ptr = new T();
        m_ref = new int(1); 
    }

    template<class Y> AutoPtr& operator=(const AutoPtr<Y> &r)
    {
        if (m_ptr == r.m_ptr) return;

        OsApi::atomicSub(*m_ref, 1);
        if (!*m_ref)
        {
            delete m_ref;
            delete m_ptr;
        }

        m_ptr = r.m_ptr;
        m_ref = r.m_ref;
        OsApi::atomicAdd(*m_ref, 1);
    }

    AutoPtr& operator=(const AutoPtr& r)
    {
        if (m_ptr == r.m_ptr) return;

        OsApi::atomicSub(*m_ref, 1);
        if (!*m_ref)
        {
            delete m_ref;
            delete m_ptr;
        }

        m_ptr = r.m_ptr;
        m_ref = r.m_ref;
        OsApi::atomicAdd(*m_ref, 1);
    }

    template<class Y> AutoPtr dynamicCast(const AutoPtr<Y> &r)
    {
        return AutoPtr(dynamic_cast<T *>(r._ptr));
    }

    template<class Y> AutoPtr dynamicCast(Y *p)
    {
        return AutoPtr(dynamic_cast<T *>(p));
    }

    template<class Y> friend bool operator==(const AutoPtr<T> &l, const AutoPtr<Y> &r);
    template<class Y> friend bool operator<(const AutoPtr<T> &l, const AutoPtr<Y> &r);
    
private:

    T *m_ptr;
    int *m_ref;
};

template<class T, class Y>
inline bool operator==(const AutoPtr<T> &l, const AutoPtr<Y> &r)
{
    T* t = l.m_ptr;
    Y* y = r.m_ptr;
    if(t && y)
    {
        return *t == *y;
    }

    return !t && !y;
}

template<class T, class Y>
inline bool operator!=(const AutoPtr<T> &l, const AutoPtr<Y> &r)
{
    return !operator==(l, r);
}

template<class T, class Y>
inline bool operator<(const AutoPtr<T> &l, const AutoPtr<Y> &r)
{
    T* t = l.m_ptr;
    Y* y = r.m_ptr;
    if(t && y)
    {
        return *t < *y;
    }

    return !t && !y;
}

template<class T, class Y>
inline bool operator<=(const AutoPtr<T> &l, const AutoPtr<Y> &r)
{
    return l < r || l == r;
}

template<class T, class Y>
inline bool operator>(const AutoPtr<T> &l, const AutoPtr<Y> &r)
{
    return !(l < r || l == r);
}

template<class T, class Y>
inline bool operator>=(const AutoPtr<T> &l, const AutoPtr<Y> &r)
{
    return !(l < r);
}

#endif   /* ----- #ifndef _AUTOPTR_H_  ----- */

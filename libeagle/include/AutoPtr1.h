/**
 *       Filename:  AutoPtr11.h
 *
 *    Description:  smart pointer
 *
 *        Version:  1.0
 *        Created:  05/24/2015 01:32:26 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan Chen, 228268157@qq.com
 *   Organization:  
 */
#ifndef _EAGLE_AutoPtr11_H_
#define _EAGLE_AutoPtr11_H_

#include <algorithm>
#include <stdio.h>

#include "Log.h"

namespace eagle
{

class Reference
{
public:
    Reference()
    {
        m_ref = 0;
    }
    
    void inc()
    {
        __sync_fetch_and_add(const_cast<volatile int *>(&m_ref), 1);
    }

    int dec()
    {
        return __sync_sub_and_fetch(const_cast<volatile int *>(&m_ref), 1);
    }

protected:
    int m_ref;
};

template<class T> class AutoPtr1 
{
public:
    AutoPtr1()
    {
        m_ptr = NULL;
    }

    AutoPtr1(T *p)
    {
        m_ptr = p;
        if (NULL != m_ptr) m_ptr->inc();
    }

    AutoPtr1(const AutoPtr1 &r)
    {
        m_ptr = r.m_ptr;
        if (NULL == m_ptr) return;

        m_ptr->inc();
    }

    template<class Y> AutoPtr1(const AutoPtr1<Y> &r)
    {
        m_ptr = r.m_ptr;
        if (NULL == m_ptr) return;

        ptr->inc();
    }

    T *ptr()
    {
        return m_ptr;
    }

    ~AutoPtr1()
    {
        if (NULL == m_ptr) return;

        if (m_ptr->dec() == 0)
        {
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
        return m_ptr == NULL ? false : true;
    }

    void swap(AutoPtr1 &r)
    {
        std::swap(m_ptr, r.m_ptr);
    }

    AutoPtr1& operator=(T *p)
    {
        if (NULL != m_ptr)
        {
            if (m_ptr->dec() == 0)
            {
                delete m_ptr;
            }
        }

        m_ptr = p;
        if (NULL != m_ptr) m_ptr->inc();

        return *this;
    }

    template<class Y> AutoPtr1& operator=(const AutoPtr1<Y> &r)
    {
        if (m_ptr == r.m_ptr) return *this;

        if (NULL != m_ptr)
        {
            if (m_ptr->dec() == 0)
            {
                delete m_ptr;
            }
        }

        m_ptr = r.m_ptr;
        if (NULL == m_ptr) return *this;

        m_ptr->inc();

        return *this;
    }

    AutoPtr1& operator=(const AutoPtr1& r)
    {
        if (m_ptr == r.m_ptr) return *this;

        if (NULL != m_ptr)
        {
            if (m_ptr->dec() == 0)
            {
                delete m_ptr;
            }
        }

        m_ptr = r.m_ptr;
        if (NULL == m_ptr) return *this;

        m_ptr->inc();

        return *this;
    }

    template<class Y> AutoPtr1 dynamicCast(const AutoPtr1<Y> &r)
    {
        return AutoPtr1(dynamic_cast<T *>(r._ptr));
    }

    template<class Y> AutoPtr1 dynamicCast(Y *p)
    {
        return AutoPtr1(dynamic_cast<T *>(p));
    }

private:

    T *m_ptr;
};

template<class T, class Y>
inline bool operator==(const AutoPtr1<T> &l, const AutoPtr1<Y> &r)
{
    T* t = l.ptr();
    Y* y = r.ptr();
    if(t && y)
    {
        return *t == *y;
    }

    return !t && !y;
}

template<class T, class Y>
inline bool operator!=(const AutoPtr1<T> &l, const AutoPtr1<Y> &r)
{
    return !operator==(l, r);
}

template<class T, class Y>
inline bool operator<(const AutoPtr1<T> &l, const AutoPtr1<Y> &r)
{
    T* t = l.ptr();
    Y* y = r.ptr();
    if(t && y)
    {
        return *t < *y;
    }

    return !t && y;
}

template<class T, class Y>
inline bool operator<=(const AutoPtr1<T> &l, const AutoPtr1<Y> &r)
{
    return l < r || l == r;
}

template<class T, class Y>
inline bool operator>(const AutoPtr1<T> &l, const AutoPtr1<Y> &r)
{
    return !(l < r || l == r);
}

template<class T, class Y>
inline bool operator>=(const AutoPtr1<T> &l, const AutoPtr1<Y> &r)
{
    return !(l < r);
}

}
#endif   /* ----- #ifndef _EAGLE_AutoPtr11_H_  ----- */

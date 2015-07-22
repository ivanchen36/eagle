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
#ifndef _EAGLE_AUTOPTR_H_
#define _EAGLE_AUTOPTR_H_

#include <algorithm>
#include <stdio.h>

namespace eagle
{

template<class T> class AutoPtr 
{
public:
    AutoPtr()
    {
        m_ptr = NULL;
        m_ref = NULL; 
    }

    AutoPtr(T *p)
    {
        m_ptr = p;
        m_ref = NULL == m_ptr ? NULL : new int(1);
    }

    AutoPtr(const AutoPtr &r)
    {
        m_ptr = r.m_ptr;
        m_ref = r.m_ref;
        if (NULL == m_ptr) return;

        inc();
    }

    void inc()
    {
        __sync_fetch_and_add(const_cast<volatile int *>(m_ref), 1);
    }

    int dec()
    {
        return __sync_sub_and_fetch(const_cast<volatile int *>(m_ref), 1);
    }

    template<class Y> AutoPtr(const AutoPtr<Y> &r)
    {
        m_ptr = r.m_ptr;
        m_ref = r.m_ref;
        if (NULL == m_ptr) return;

        inc();
    }

    T *ptr()
    {
        return m_ptr;
    }

    ~AutoPtr()
    {
        if (NULL == m_ptr) return;

        if (dec() == 0)
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
        return m_ptr == NULL ? false : true;
    }

    void swap(AutoPtr &r)
    {
        std::swap(m_ptr, r.m_ptr);
    }

    AutoPtr& operator=(T *p)
    {
        if (NULL != m_ptr)
        {
            if (dec() == 0)
            {
                delete m_ref;
                delete m_ptr;
            }
        }

        m_ptr = p;
        m_ref = NULL == m_ptr ? NULL : new int(1);

        return *this;
    }

    template<class Y> AutoPtr& operator=(const AutoPtr<Y> &r)
    {
        if (m_ptr == r.m_ptr) return *this;

        if (NULL != m_ptr)
        {
            if (dec() == 0)
            {
                delete m_ref;
                delete m_ptr;
            }
        }

        m_ptr = r.m_ptr;
        m_ref = r.m_ref;
        if (NULL == m_ptr) return *this;

        inc();

        return *this;
    }

    AutoPtr& operator=(const AutoPtr& r)
    {
        if (m_ptr == r.m_ptr) return *this;

        if (NULL != m_ptr)
        {
            if (dec() == 0)
            {
                delete m_ref;
                delete m_ptr;
            }
        }

        m_ptr = r.m_ptr;
        m_ref = r.m_ref;
        if (NULL == m_ptr) return *this;

        inc();

        return *this;
    }

    template<class Y> AutoPtr dynamicCast(const AutoPtr<Y> &r)
    {
        return AutoPtr(dynamic_cast<T *>(r._ptr));
    }

    template<class Y> AutoPtr dynamicCast(Y *p)
    {
        return AutoPtr(dynamic_cast<T *>(p));
    }

private:

    T *m_ptr;
    int *m_ref;
};

template<class T, class Y>
inline bool operator==(const AutoPtr<T> &l, const AutoPtr<Y> &r)
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
inline bool operator!=(const AutoPtr<T> &l, const AutoPtr<Y> &r)
{
    return !operator==(l, r);
}

template<class T, class Y>
inline bool operator<(const AutoPtr<T> &l, const AutoPtr<Y> &r)
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

}
#endif   /* ----- #ifndef _EAGLE_AUTOPTR_H_  ----- */

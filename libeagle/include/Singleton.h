/**
 *       Filename:  Singleton.h
 *
 *    Description:  Singleton
 *
 *        Version:  1.0
 *        Created:  04/23/2015 10:58:25 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan Chen, 228268157@qq.com
 *   Organization:  
 */

#ifndef  _SINGLETON_H_
#define  _SINGLETON_H_

#include "MutexLock.h"

extern MutexLock g_singletonLock;

template<class T> class Singleton 
{
public:
    static T &instance()
    {
        static SingletonCleaner s_cleaner;

        if (NULL != s_instance) return *s_instance;

        LockGuard guard(g_singletonLock);
        if (NULL != s_instance) return *s_instance;

        s_instance = new T();
        return *s_instance;
    }

    static void del()
    {
        if (NULL == s_instance) return;

        LockGuard guard(g_singletonLock);
        if (NULL == s_instance) return;

        delete s_instance;
        s_instance = NULL;
    }
    
private:
    class SingletonCleaner
    {
    public:
        ~SingletonCleaner()
        {
            if (NULL != s_instance) delete s_instance;
        }
    };

    static T *s_instance;
};
template <class T> T *Singleton<T>::s_instance = NULL;

#endif   /* ----- #ifndef _SINGLETON_H_  ----- */

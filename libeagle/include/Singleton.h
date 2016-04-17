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

#ifndef _EAGLE_SINGLETON_H_
#define _EAGLE_SINGLETON_H_

#include "MutexLock.h"

namespace eagle
{

static MutexLock s_singletonLock(1, 1);

template<class T> class Singleton 
{
public:
    static T &instance()
    {
        static SingletonCleaner s_cleaner;

        if (NULL != s_instance) return *s_instance;

        LockGuard<MutexLock> guard(s_singletonLock);
        if (NULL != s_instance) return *s_instance;

        s_instance = new T();

        return *s_instance;
    }

    static void del()
    {
        if (NULL == s_instance) return;

        LockGuard<MutexLock> guard(s_singletonLock);
        if (NULL == s_instance) return;

        delete s_instance;
        s_instance = NULL;
    }

    static bool isExist()
    {
        return NULL != s_instance;
    }
    
private:
    class SingletonCleaner
    {
    public:
        ~SingletonCleaner()
        {
            if (NULL != s_instance) delete s_instance;
            s_instance = NULL;
        }
    };

    static T *s_instance;
};
template <class T> T *Singleton<T>::s_instance = NULL;

}
#endif   /* ----- #ifndef _EAGLE_SINGLETON_H_  ----- */

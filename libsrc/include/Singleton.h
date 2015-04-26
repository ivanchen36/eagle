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

#include "Lock.h"

using namespace std;

class SingletonInitializer
{
public:
    SingletonInitializer()
    {
        pthread_mutex_init(&s_lock, NULL);
    }

    ~SingletonInitializer()
    {
        pthread_mutex_destroy(&s_lock);
    }

    static pthread_mutex_t s_lock;
};

template<class T> class Singleton 
{
public:
    static T &instance()
    {
        if (s_instance) return *s_instance;

        Lock lock(SingletonInitializer::s_lock);
        if (s_instance) return *s_instance;

        s_cleaner;
        s_instance = new T();
        return *s_instance;
    }
    
private:
    class SingletonCleaner
    {
    public:
       SingletonCleaner(){}; 
       ~SingletonCleaner()
       {
           if (s_instance) delete s_instance;
       }
    };

    Singleton(){};
    ~Singleton(){};

    static T *s_instance;
    static SingletonCleaner s_cleaner;
};
template <class T> T *Singleton<T>::s_instance = NULL;
template <class T> typename Singleton<T>::SingletonCleaner Singleton<T>::s_cleaner;

#endif   /* ----- #ifndef _SINGLETON_H_  ----- */

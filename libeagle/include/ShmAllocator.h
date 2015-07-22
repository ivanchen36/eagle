/**
 *       Filename:  ShmAllocator.h
 *
 *    Description:  share memory allocator
 *
 *        Version:  1.0
 *        Created:  06/12/2015 11:02:01 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan Chen, 228268157@qq.com
 *   Organization:  
 */
#include <memory>

#include "ShareMem.h"

namespace eagle
{

template <class T> class ShmAllocator : public std::allocator <T>
{
public:  
    template<class U> struct rebind
    {  
        typedef ShmAllocator<U> other;  
    };  

    ShmAllocator() {}  

    ShmAllocator(ShmAllocator<T> const &allocaltor) {}  

    template <class U> ShmAllocator(ShmAllocator<U> const &allocaltor) {}

    ShmAllocator<T> &operator=(ShmAllocator<T> const &allocaltor)
    {
        return *this;
    }  

    template<class U> ShmAllocator<T> &operator=(
            ShmAllocator<U> const& allocaltor)
    {
        return *this;
    }  

    T *allocate(size_t count)
    {
        return (T *)ShareMemI::instance().alloc(count * s_objectSize);
    }  

    void deallocate(T *ptr, size_t count)
    {
        ShareMemI::instance().free((void *)ptr);
    }  

    void construct(T *p, const T &val)
    {
        :: new ((void *)p) T(val);
    }

    void destroy(T *p)
    {
        p->~T();
    }

    static int s_objectSize;
};

template <class T> int ShmAllocator<T>::s_objectSize = sizeof(T);

}

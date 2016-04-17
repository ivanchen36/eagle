/**
 *       Filename:  ShareMem.h
 *
 *    Description:  share memory
 *
 *        Version:  1.0
 *        Created:  04/29/2015 07:54:39 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan Chen, 228268157@qq.com
 *   Organization:  
 */
#ifndef _EAGLE_SHAREMEM_H_
#define _EAGLE_SHAREMEM_H_

#include <stdint.h>
#include <unistd.h>
#include <tr1/unordered_map>

#include "AutoPtr.h"
#include "Singleton.h"

namespace eagle
{

class ShareMem
{
public:
    ~ShareMem();

    void *alloc(const int size, const int key = 0);
    void *calloc(const int size, const int key = 0);
    void free(void *ptr);

private:
    struct ShmInfo
    {
        int id;
        int pid;
    };
    typedef std::tr1::unordered_map<uintptr_t, ShmInfo> ShareMemMap;

    ShareMem(){};
    void free(void *ptr, const ShmInfo &info);

    ShareMemMap m_shmMap;

    friend class Singleton<ShareMem>;
};

typedef Singleton<ShareMem> ShareMemI;

}
#endif   /* ----- #ifndef _EAGLE_SHAREMEM_H_  ----- */

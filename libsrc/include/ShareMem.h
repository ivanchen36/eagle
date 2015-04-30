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
#ifndef  _SHAREMEM_H_
#define  _SHAREMEM_H_

#include <stdint.h>
#include <map>

#include "AutoPtr.h"
#include "Singleton.h"

class ShareMem
{
public:
    ~ShareMem();

    void *alloc(const int size, const int key = 0);
    void *calloc(const int size, const int key = 0);
    void free(void *ptr);

private:
    ShareMem(){};
    void free(void *ptr, int id);

    std::map<uintptr_t, int> m_shmMap;

    friend class Singleton<ShareMem>;
};

typedef Singleton<ShareMem> ShareMemI;

#endif   /* ----- #ifndef _SHAREMEM_H_  ----- */

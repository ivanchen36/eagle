/**
 *       Filename:  Hash.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  04/01/2016 01:09:09 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan Chen, 228268157@qq.com
 *   Organization:  
 */
#ifndef _HASH_H_
#define _HASH_H_

#include <stdint.h>
#include <stdlib.h>

namespace eagle
{

uint64_t murmurHash(const uint8_t *key, int len)
{
    uint64_t h, k;

    h = 0 ^ len;
    while (len >= 4) {
        k  = key[0];
        k |= key[1] << 8;
        k |= key[2] << 16;
        k |= key[3] << 24;

        k *= 0x5bd1e995;
        k ^= k >> 24;
        k *= 0x5bd1e995;

        h *= 0x5bd1e995;
        h ^= k;

        key += 4;
        len -= 4;
    }   

    switch (len) {
        case 3:
            h ^= key[2] << 16; 
        case 2:
            h ^= key[1] << 8;
        case 1:
            h ^= key[0];
            h *= 0x5bd1e995;
    }   

    h ^= h >> 13; 
    h *= 0x5bd1e995;
    h ^= h >> 15; 

    return abs(h);
}

template <typename T> bool pointerCompare(T l, T r)
{
    return *l < *r;
}

}
#endif   /* ----- #ifndef _HASH_H_  ----- */

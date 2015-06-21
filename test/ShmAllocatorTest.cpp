/**
 *       Filename:  ShmAllocatorTest.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/13/2015 09:03:17 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan Chen, 228268157@qq.com
 *   Organization:  
 */
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <map>
#include <vector>

#include "Log.h"
#include "EagleTime.h"
#include "ShmAllocator.h"

void doTest()
{
    ShmAllocator<int> alloc;
    std::vector<int, ShmAllocator<int> > vec(alloc);
    DEBUGLOG1("%d", vec.capacity());
    vec.push_back(1);
    sleep(5);
    DEBUGLOG1("%d", vec.capacity());
    vec.resize(20);
    sleep(5);
    vec.push_back(2);
    DEBUGLOG1("%d", vec.capacity());
    sleep(5);
    ShmAllocator< std::pair<int, int> > alloc1;
    std::less<int> less1;
    std::map<int, int, std::less<int>, ShmAllocator< std::pair<int, int> > > m(less1, alloc1);
    m[1] = 2;
    m[2] = 2;
    m[3] = 2;
    m[4] = 2;
    m[5] = 2;
    m[6] = 2;
    sleep(5);
}

/**
 * @brief main 
 */
int main ( int argc, char *argv[] )
{
    EagleTimeI::instance().autoUpdate();

    doTest();

    return EXIT_SUCCESS;
}

/**
 *       Filename:  ClassTest.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/27/2015 09:08:54 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan Chen, 228268157@qq.com
 *   Organization:  
 */
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <sys/wait.h>
#include <fcntl.h>

#include "Log.h"
#include "EagleTime.h"
#include "ClassFactory.h"
#include "Define.h"
#include "MessageHandlerFactory.h"

using namespace std;

class Test
{
public:
    void printTest()
    {
        DEBUGLOG("test");
    }
};

REGISTER_REFLECTOR("test", Test)

void test()
{
    Test *test;
    MessageHandlerFactoryI::instance().registerHandler(8080, "test");

    if (MessageHandlerFactoryI::instance().createHandler(8080, test) != EG_SUCCESS)
    {
        DEBUGLOG("createHandler err");

        return;
    }

    test->printTest();
}

/**
 * @brief main 
 */
int main ( int argc, char *argv[] )
{
    EagleTimeI::instance().autoUpdate();

    test();

    return EXIT_SUCCESS;
}

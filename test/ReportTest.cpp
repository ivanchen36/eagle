/**
 *       Filename:  CHTest.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  04/03/2016 12:53:07 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan Chen, 228268157@qq.com
 *   Organization:  
 */
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "ServerReport.h"
#include "Thread.h"

using namespace std;
using namespace eagle;

void sendReport(void *param)
{
    int isSucc = (uint64_t)param % 2;
    static const std::string testReq = "test";
    static ServerReport &report = ServerReportI::instance();

    for (int i = 0; i < 100000; ++i)
    {
        if (isSucc)
        {
            report.sendSuccReq(testReq);
        }else
        {
            report.sendFailReq(testReq, i % 10, i % 10);
        }

        if (0 == i % 100)
        {
            sleep(1);
        }
    }
}

void doTest()
{
    ServerReportI::instance().init("./report", "testserver");
    for (int i = 0; i < 100; ++i)
    {
        CallBack cb(sendReport, (void *)i);
        Thread thread(cb);
    }
    
    CallBack cb(sendReport, (void *)1);
    Thread thread(cb, 0);
}

/**
 * @brief main 
 */
int main ( int argc, char *argv[] )
{
    doTest();

    return EXIT_SUCCESS;
}

/**
 *       Filename:  AutoPtrTest.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  04/19/2015 04:04:44 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan Chen, 228268157@qq.com
 *   Organization:  
 */

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "TimeStats.h"

using namespace std;

/**
 * @brief main 
 */
int main ( int argc, char *argv[] )
{

    TimeStatsPtr ptr;
    long time = TS_STATS1(ptr, sleep, 2);
    cout << time << endl;

    ptr = new TimeStats(TimeStats::USEC);
    time = TS_STATS1(ptr, usleep, 100);
    cout << time << endl;

    ptr->disable();
    time = TS_STATS1(ptr, usleep, 1000);
    cout << time << endl;

    ptr->enable();
    time = TS_STATS1(ptr, usleep, 200);
    cout << time << endl;

    return EXIT_SUCCESS;
}

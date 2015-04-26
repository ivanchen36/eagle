/**
 *       Filename:  TimeStats.h
 *
 *    Description:  time stats
 *
 *        Version:  1.0
 *        Created:  04/19/2015 04:44:15 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan Chen, 228268157@qq.com
 *   Organization:  
 */


#ifndef _TIME_STATS_H_
#define _TIME_STATS_H_

#include <sys/time.h>
#include <stdlib.h>

#include "AutoPtr.h"

#define TS_STATS0(ts, cb)    \
(                               \
    ts->start(),            \
    (*cb)(),                    \
    ts->stop()               \
)

#define TS_STATS1(ts, cb, arg1)  \
(                                   \
    ts->start(),                \
    (*cb)(arg1),                    \
    ts->stop()                   \
)

#define TS_STATS2(ts, cb, arg1, arg2)    \
(                                           \
    ts->start(),                        \
    (*cb)(arg1, arg2),                      \
    ts->stop()                           \
)

#define TS_STATS3(ts, cb, arg1, arg2, arg3)  \
(                                               \
    ts->start(),                            \
    (*cb)(arg1, arg2, arg3),                    \
    ts->stop()                               \
)

#define TS_STATS4(ts, cb, arg1, arg2, arg3, arg4)    \
(                                                       \
    ts->start(),                                    \
    (*cb)(arg1, arg2, arg3, arg4),                      \
    ts->stop()                                       \
)


class TimeStats
{
public:
    enum Unit
    {
        USEC = 1,
        MSEC = 1000,
        SEC = 1000000,
    };

    TimeStats();
    TimeStats(Unit unit, int isEnable = 1);
    ~TimeStats(){};

    void enable();
    void disable();
    void start();
    long stop();

private:
    int m_isEnable;
    Unit m_unit;
    struct timeval m_start;
    struct timeval m_end;
};

typedef AutoPtr<TimeStats> TimeStatsPtr;

#endif

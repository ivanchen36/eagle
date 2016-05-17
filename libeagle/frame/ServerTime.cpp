#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>

#include "ServerTime.h"
#include "CallBack.h"
#include "Define.h"
#include "Timer.h"
#include "CallBack.h"

namespace
{

const char *TIME_UPDATE_TIMERTASK = "updatetime";

void updateServerTime(void *param)
{
    static eagle::ServerTime &time = eagle::ServerTimeI::instance();
    time.checkAndUpdate();
}

}

namespace eagle
{

__attribute__((constructor)) void autoUpdate()
{
    CallBack cb(updateServerTime);

    TimerI::instance().addTask(TIME_UPDATE_TIMERTASK, MIN_TIMER_INTERVAL, cb);
}

std::string ServerTime::s_timeStr = "1970-01-01 00:00:00";

ServerTime::ServerTime() : m_info(NULL)
{
    m_pid = getpid();
    m_info = (TimeInfo *)ShareMemI::instance().alloc(
            sizeof(TimeInfo), INT_MAX - SHM_KEY_TIME);
    __sync_bool_compare_and_swap(
            const_cast<volatile int *>(&(m_info->pid)), 0, m_pid);
    checkAndUpdate();
}

ServerTime::~ServerTime()
{
}

void ServerTime::autoUpdate()
{
    CallBack cb(updateServerTime);

    TimerI::instance().addTask(TIME_UPDATE_TIMERTASK, MIN_TIMER_INTERVAL, cb);
}

void ServerTime::cancelUpdate()
{
    TimerI::instance().delTask(TIME_UPDATE_TIMERTASK);
}

void ServerTime::checkAndUpdate()
{
    static int count = 0;
    static uint64_t msec = 0;
    static int &pid = m_info->pid;
    static uint64_t &curMsec = m_info->msec;
    static char (&timeStr)[TIME_STR_LEN] = m_info->timeStr;
    static int len = s_timeStr.length();

    if (m_pid == pid)
    {
        update();

        return;
    }

    if (msec != curMsec)
    {
        msec = curMsec;
        memcpy(&s_timeStr.at(0), timeStr, len);

        return;
    }

    if (++count < 4) return;

    __sync_bool_compare_and_swap(
            const_cast<volatile int *>(&pid), pid, m_pid);
    count = 0;
}

void ServerTime::update()
{
    static time_t &sec = m_info->sec;
    static uint64_t &msec = m_info->msec;
    static char (&timeStr)[TIME_STR_LEN] = m_info->timeStr;
    static struct tm &tm = m_info->tm;
    static struct timeval &tv = m_info->tv;
    static int len = s_timeStr.length();

    gettimeofday(&tv, NULL);
    msec = tv.tv_sec * 1000 + tv.tv_usec / 1000;
    if (tv.tv_sec == sec) return;

    sec = tv.tv_sec;
    localtime_r(&tv.tv_sec, &tm);
    sprintf(timeStr, "%04d-%02d-%02d %02d:%02d:%02d",
            tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, 
            tm.tm_min, tm.tm_sec);
    memcpy(&s_timeStr.at(0), timeStr, len);
}

}

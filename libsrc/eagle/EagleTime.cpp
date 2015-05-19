#include <sys/time.h>
#include <stdio.h>

#include "Log.h"
#include "EagleTime.h"
#include "CallBack.h"
#include "Timer.h"

#define UPDATE_TIME_INTERVAL 50

void updateEagleTime(void *param)
{
    EagleTimeI::instance().update();
}

EagleTime::EagleTime() : m_msec(0), m_sec(0)
{
    update();
    Log::setLogTime(m_logTime);
}

EagleTime::~EagleTime()
{
    Log::setLogTime("1970-09-28 12:00:00");
}

void EagleTime::cancelUpdate()
{
    TimerI::instance().delTask("updatetime");
}

void EagleTime::autoUpdate()
{
    CallBack cb(updateEagleTime);
    TimerI::instance().addTask("updatetime", MIN_TIMER_INTERVAL, cb);
}

void EagleTime::update()
{
    struct tm t;
    struct timeval tv;

    gettimeofday(&tv, NULL);
    m_msec = tv.tv_sec * 1000 + tv.tv_usec / 1000;
    if (m_sec == tv.tv_sec) return;

    m_sec = tv.tv_sec;
    localtime_r(&tv.tv_sec, &t);
    snprintf(m_logTime, LOGTIMELEN, "%04d-%02d-%02d %02d:%02d:%02d",
            t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, t.tm_hour, 
            t.tm_min, t.tm_sec);
}

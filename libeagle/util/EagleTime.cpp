#include <sys/time.h>
#include <stdio.h>

#include "EagleTime.h"
#include "CallBack.h"
#include "Define.h"
#include "Timer.h"

namespace eagle
{

namespace
{
const char *TIME_UPDATE_TIMERTASK = "updatetime";
EagleTime &eagleTime = EagleTimeI::instance();
}

void updateEagleTime(void *param)
{
    eagleTime.update();
}

EagleTime::EagleTime() : m_msec(0)
{
    m_tmpStr = new std::string("1970-09-28 12:00:00");
    m_timeStr = new std::string("1970-09-28 12:00:00");
    update();
}

EagleTime::~EagleTime()
{
}

void EagleTime::cancelUpdate()
{
    TimerI::instance().delTask(TIME_UPDATE_TIMERTASK);
}

void EagleTime::autoUpdate()
{
    CallBack cb(updateEagleTime);
    TimerI::instance().addTask(TIME_UPDATE_TIMERTASK, MIN_TIMER_INTERVAL, cb);
}

void EagleTime::update()
{
    std::string *tmp;

    gettimeofday(&m_tv, NULL);
    m_msec = m_tv.tv_sec * 1000 + m_tv.tv_usec / 1000;
    if (m_tv.tv_sec == m_tv.tv_sec) return;

    localtime_r(&m_tv.tv_sec, &m_tm);
    sprintf((char *)m_tmpStr->c_str(), "%04d-%02d-%02d %02d:%02d:%02d",
            m_tm.tm_year + 1900, m_tm.tm_mon + 1, m_tm.tm_mday, m_tm.tm_hour, 
            m_tm.tm_min, m_tm.tm_sec);
    tmp = m_timeStr;
    m_timeStr = m_tmpStr;
    m_tmpStr = tmp;
}

}

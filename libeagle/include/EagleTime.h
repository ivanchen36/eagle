/**
 *       Filename:  EagleTime.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/05/2015 10:18:20 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan Chen, 228268157@qq.com
 *   Organization:  
 */
#ifndef _EAGLE_EAGLETIME_H_
#define _EAGLE_EAGLETIME_H_
#include <time.h>
#include <stdint.h>
#include <string>

#include "Singleton.h"
#include "ShmAllocator.h"

namespace eagle
{

#define TIME_STR_LEN 20

class EagleTime
{
public:
    ~EagleTime();

    void cancelUpdate();
    void checkAndUpdate();

    const std::string &getTimeStr()
    {
        static time_t &curSec = m_info->sec;
        static char (&curTimeStr)[TIME_STR_LEN] = m_info->timeStr;
        static int sec = curSec;
        static std::string timeStr = curTimeStr;

        if (sec != curSec)
        {
            sec = curSec;
            timeStr = curTimeStr;
        }

        return timeStr;
    }

    const std::string getTimeStr(const char *format)
    {
        char buf[32];
        static struct tm &tm = m_info->tm;

        strftime(buf, 32, format, &tm);

        return buf;
    }

    const std::string getUTCTimeStr(const char *format)
    {
        char buf[32];
        struct tm tm;
        static time_t &sec = m_info->sec;

        gmtime_r(&sec, &tm);
        strftime(buf, 32, format, &tm);

        return buf;
    }

    const time_t &getSec()
    {
        static time_t &sec = m_info->sec;

        return sec;
    }

    const uint64_t &getMsec()
    {
        static uint64_t &msec = m_info->msec;

        return msec;
    }

    const struct tm &getTm()
    {
        static struct tm &tm = m_info->tm;

        return tm;
    }

private:
    struct TimeInfo
    {
        int pid;
        time_t sec;
        uint64_t msec;
        char timeStr[TIME_STR_LEN];
        struct tm tm;
        struct timeval tv;
    };

    EagleTime();
    void update();

    int m_pid;
    TimeInfo *m_info;

    friend class Singleton<EagleTime>;
};

typedef Singleton<EagleTime> EagleTimeI;

}
#endif   /* ----- #ifndef _EAGLE_EAGLETIME_H_  ----- */

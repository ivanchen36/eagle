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

namespace eagle
{

class EagleTime
{
public:
    ~EagleTime();

    void update();
    void autoUpdate();
    void cancelUpdate();

    const std::string &getTimeStr()
    {
        return *m_timeStr;
    }

    const std::string getTimeStr(const char *format)
    {
        char buf[32];
        strftime(buf, 32, format, &m_tm);

        return buf;
    }

    const std::string getUTCTimeStr(const char *format)
    {
        char buf[32];
        struct tm tm;

        gmtime_r(&m_tv.tv_sec, &tm);
        strftime(buf, 32, format, &tm);

        return buf;
    }

    const time_t getSec()
    {
        return m_tv.tv_sec;
    }

    const uint64_t getMsec()
    {
        return m_msec;
    }

    const struct tm &getTm()
    {
        return m_tm;
    }

private:
    EagleTime();

    uint64_t m_msec;
    struct tm m_tm;
    struct timeval m_tv;
    std::string *m_tmpStr;
    std::string *m_timeStr;

    friend class Singleton<EagleTime>;
};

typedef Singleton<EagleTime> EagleTimeI;

}
#endif   /* ----- #ifndef _EAGLE_EAGLETIME_H_  ----- */

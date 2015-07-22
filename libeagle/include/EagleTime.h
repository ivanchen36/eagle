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

#include "Singleton.h"
#include "ShareMem.h"

namespace eagle
{

#define LOGTIMELEN sizeof("1970-09-28 12:00:00")
#define ISO8601LEN sizeof("1970/09/28 12:00:00")

class EagleTime
{
public:
    ~EagleTime();

    void update();
    void autoUpdate();
    void cancelUpdate();
    void *operator new(size_t size);
    void operator delete(void* ptr);

    const char *getLogTime()
    {
        return m_logTime;
    }

    const time_t getSec()
    {
        return m_sec;
    }

    const uint64_t getMsec()
    {
        return m_msec;
    }

private:
    EagleTime();

    time_t m_sec;
    uint64_t m_msec;
    char m_logTime[LOGTIMELEN];
    char m_iso8601[ISO8601LEN];

    friend class Singleton<EagleTime>;
};

typedef Singleton<EagleTime> EagleTimeI;

}
#endif   /* ----- #ifndef _EAGLE_EAGLETIME_H_  ----- */

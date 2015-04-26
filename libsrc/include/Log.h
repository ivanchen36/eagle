/**
 *       fileName:  Log.h
 *
 *    Description:  Log
 *
 *        Version:  1.0
 *        Created:  04/26/2015 11:39:05 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan Chen, 228268157@qq.com
 *   Organization:  
 */


#ifndef _EG_LOG_H_
#define _EG_LOG_H_

#include <stdint.h>
#include <string.h>
#include <errno.h>

#include "FileEx.h"
#include "Lock.h"

class Log;
extern AutoPtr<Log> g_sysLog;

#define DEBUGLOG(format) g_sysLog->debug("[%s:%d] "format, __FILE__, __LINE__)
#define DEBUGLOG1(format, arg1) g_sysLog->debug("[%s:%d] "format, __FILE__, __LINE__, arg1)
#define DEBUGLOG2(format, arg1, arg2) g_sysLog->debug("[%s:%d] "format, __FILE__, __LINE__, arg1, arg2)
#define DEBUGLOG3(format, arg1, arg2, arg3) g_sysLog->debug("[%s:%d] "format, __FILE__, __LINE__, arg1, arg2, arg3)
#define DEBUGLOG4(format, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9) \
    g_sysLog->debug("[%s:%d] "format, __FILE__, __LINE__, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9)
#define DEBUGLOG5(format, arg1, arg2, arg3, arg4, arg5) \
    g_sysLog->debug("[%s:%d] "format, __FILE__, __LINE__, arg1, arg2, arg3, arg4, arg5)
#define DEBUGLOG6(format, arg1, arg2, arg3, arg4, arg5, arg6) \
    g_sysLog->debug("[%s:%d] "format, __FILE__, __LINE__, arg1, arg2, arg3, arg4, arg5, arg6)
#define DEBUGLOG7(format, arg1, arg2, arg3, arg4, arg5, arg6, arg7) \
    g_sysLog->debug("[%s:%d] "format, __FILE__, __LINE__, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
#define DEBUGLOG8(format, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8) \
    g_sysLog->debug("[%s:%d] "format, __FILE__, __LINE__, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)
#define DEBUGLOG9(format, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9) \
    g_sysLog->debug("[%s:%d] "format, __FILE__, __LINE__, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9)

#define INFOLOG(format) g_sysLog->info("[%s:%d] "format, __FILE__, __LINE__)
#define INFOLOG1(format, arg1) g_sysLog->info("[%s:%d] "format, __FILE__, __LINE__, arg1)
#define INFOLOG2(format, arg1, arg2) g_sysLog->info("[%s:%d] "format, __FILE__, __LINE__, arg1, arg2)
#define INFOLOG3(format, arg1, arg2, arg3) g_sysLog->info("[%s:%d] "format, __FILE__, __LINE__, arg1, arg2, arg3)
#define INFOLOG4(format, arg1, arg2, arg3, arg4) \
    g_sysLog->info("[%s:%d] "format, __FILE__, __LINE__, arg1, arg2, arg3, arg4)
#define INFOLOG5(format, arg1, arg2, arg3, arg4, arg5) \
    g_sysLog->info("[%s:%d] "format, __FILE__, __LINE__, arg1, arg2, arg3, arg4, arg5)
#define INFOLOG6(format, arg1, arg2, arg3, arg4, arg5, arg6) \
    g_sysLog->info("[%s:%d] "format, __FILE__, __LINE__, arg1, arg2, arg3, arg4, arg5, arg6)
#define INFOLOG7(format, arg1, arg2, arg3, arg4, arg5, arg6, arg7) \
    g_sysLog->info("[%s:%d] "format, __FILE__, __LINE__, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
#define INFOLOG8(format, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8) \
    g_sysLog->info("[%s:%d] "format, __FILE__, __LINE__, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)
#define INFOLOG9(format, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9) \
    g_sysLog->info("[%s:%d] "format, __FILE__, __LINE__, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9)

#define WARNLOG(format) g_sysLog->warn("[%s:%d] "format, __FILE__, __LINE__)
#define WARNLOG1(format, arg1) g_sysLog->warn("[%s:%d] "format, __FILE__, __LINE__, arg1)
#define WARNLOG2(format, arg1, arg2) wg_sysLog->arn("[%s:%d] "format, __FILE__, __LINE__, arg1, arg2)
#define WARNLOG3(format, arg1, arg2, arg3) g_sysLog->warn("[%s:%d] "format, __FILE__, __LINE__, arg1, arg2, arg3)
#define WARNLOG4(format, arg1, arg2, arg3, arg4) \
    g_sysLog->warn("[%s:%d] "format, __FILE__, __LINE__, arg1, arg2, arg3, arg4)
#define WARNLOG5(format, arg1, arg2, arg3, arg4, arg5) \
    g_sysLog->warn("[%s:%d] "format, __FILE__, __LINE__, arg1, arg2, arg3, arg4, arg5)
#define WARNLOG6(format, arg1, arg2, arg3, arg4, arg5, arg6) \
    g_sysLog->warn("[%s:%d] "format, __FILE__, __LINE__, arg1, arg2, arg3, arg4, arg5, arg6)
#define WARNLOG7(format, arg1, arg2, arg3, arg4, arg5, arg6, arg7) \
    g_sysLog->warn("[%s:%d] "format, __FILE__, __LINE__, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
#define WARNLOG8(format, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8) \
    g_sysLog->warn("[%s:%d] "format, __FILE__, __LINE__, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)
#define WARNLOG9(format, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9) \
    g_sysLog->warn("[%s:%d] "format, __FILE__, __LINE__, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9)

#define ERRORLOG(format) g_sysLog->error("[%s:%d] "format, __FILE__, __LINE__)
#define ERRORLOG1(format, arg1) g_sysLog->error("[%s:%d] "format, __FILE__, __LINE__, arg1)
#define ERRORLOG2(format, arg1, arg2) g_sysLog->error("[%s:%d] "format, __FILE__, __LINE__, arg1, arg2)
#define ERRORLOG3(format, arg1, arg2, arg3) g_sysLog->error("[%s:%d] "format, __FILE__, __LINE__, arg1, arg2, arg3)
#define ERRORLOG4(format, arg1, arg2, arg3, arg4) \
    g_sysLog->error("[%s:%d] "format, __FILE__, __LINE__, arg1, arg2, arg3, arg4)
#define ERRORLOG5(format, arg1, arg2, arg3, arg4, arg5) \
    g_sysLog->error("[%s:%d] "format, __FILE__, __LINE__, arg1, arg2, arg3, arg4, arg5)
#define ERRORLOG6(format, arg1, arg2, arg3, arg4, arg5, arg6) \
    g_sysLog->error("[%s:%d] "format, __FILE__, __LINE__, arg1, arg2, arg3, arg4, arg5, arg6)
#define ERRORLOG7(format, arg1, arg2, arg3, arg4, arg5, arg6, arg7) \
    g_sysLog->error("[%s:%d] "format, __FILE__, __LINE__, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
#define ERRORLOG8(format, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8) \
    g_sysLog->error("[%s:%d] "format, __FILE__, __LINE__, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)
#define ERRORLOG9(format, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9) \
    g_sysLog->error("[%s:%d] "format, __FILE__, __LINE__, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9)

#define DEBUG_LOG 0
#define INFO_LOG 1
#define WARN_LOG 2
#define ERROR_LOG 3

#define MAX_LINE_LEN 1024

class Log
{
public:
    Log(const char *fileName, int level);
    ~Log();

    int redirectToLog(const int fd);
    int redirectToOther(const int fd);
    void debug(const char *format, ...);
    void info(const char *format, ...);
    void warn(const char *format, ...);
    void error(const char *format, ...);

private:
    Log();
    void write(int level, const char *format, va_list args);

    int m_level; 
    FileExPtr m_file;

    friend class AutoPtr<Log>;
};

typedef AutoPtr<Log> LogPtr;
#endif

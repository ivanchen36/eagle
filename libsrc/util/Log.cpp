#include <time.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>

#include "Log.h"

LogPtr g_sysLog = new Log("./debug.log", DEBUG_LOG);

const static char *LOG_LEVEL_STR[] = {"debug", "info", "warn", "error"};

Log::Log()
{
    m_file = new FileEx("./debug", RDWR_APPEND);
    m_level = DEBUG_LOG;
}

Log::Log(const char *fileName, int level)
{
    m_file = new FileEx(fileName, RDWR_APPEND);
    m_level = level;
}

Log::~Log()
{   
}

int Log::redirectToOther(const int fd)
{
    if (dup2(fd, m_file->getFd()) == -1) return -1;

    return 0;
}

int Log::redirectToLog(const int fd)
{
    if (dup2(m_file->getFd(), fd) == -1) return -1;

    return 0;
}

void Log::write(int level, const char *format, va_list args)
{
    time_t tmp;
    struct tm timeinfo;
    int len;
    char buf[MAX_LINE_LEN];

    time (&tmp);
    localtime_r(&tmp, &timeinfo);
    snprintf(buf, MAX_LINE_LEN, "[%04d-%02d-%02d %02d:%02d:%02d %s]",
            timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday,
            timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec, LOG_LEVEL_STR[level]);
    len = strlen(buf);
    vsnprintf(buf +len, MAX_LINE_LEN - len, format, args);
    len += strlen(buf + len);
    buf[len++] = '\n';

    m_file->writeByLen((uint8_t *)buf, len);
	m_file->flush();
}

void Log::debug(const char *format, ...)
{
    if (m_level > DEBUG_LOG) return;

    va_list args;
    va_start(args, format);
    write(DEBUG_LOG, format, args);
    va_end(args);
}

void Log::info(const char *format, ...)
{
    if (m_level > INFO_LOG) return;

    va_list args;
    va_start(args, format);
    write(INFO_LOG, format, args);
    va_end(args);
}

void Log::warn(const char *format, ...)
{
    if (m_level > WARN_LOG) return;

    va_list args;
    va_start(args, format);
    write(WARN_LOG, format, args);
    va_end(args);
}

void Log::error(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    write(ERROR_LOG, format, args);
    va_end(args);
}

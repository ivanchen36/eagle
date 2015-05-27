#include <time.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>

#include "Log.h"
#include "EagleTime.h"

LogPtr g_sysLog = new Log("./debug.log", DEBUG_LOG);

namespace
{
const char *g_logTime = "1970-09-28 12:00:00";
const char *LOG_LEVEL_STR[] = {"debug", "info", "warn", "error"};

__attribute__((constructor)) void initLog()  
{ 
    ShareMemI::instance();
    EagleTimeI::instance().getLogTime();
}
}

Log::Log(const char *fileName, int level)
    : m_file(fileName, FileEx::RDWR_CREATE_APPEND)
{
    m_level = level;
}

Log::~Log()
{   
}

void Log::setLogTime(const char *logTime)
{
    g_logTime = logTime;
}

int Log::redirectToOther(const int fd)
{
    if (dup2(fd, m_file.getFd()) == -1) return -1;

    return 0;
}

int Log::redirectToLog(const int fd)
{
    if (dup2(m_file.getFd(), fd) == -1) return -1;

    return 0;
}

void Log::write(int level, const char *format, va_list args)
{
    int len;
    char buf[MAX_LINE_LEN];

    snprintf(buf, MAX_LINE_LEN, "[%s %s]", g_logTime, LOG_LEVEL_STR[level]);
    len = strlen(buf);
    vsnprintf(buf +len, MAX_LINE_LEN - len, format, args);
    len += strlen(buf + len);
    buf[len++] = '\n';

    m_file.writeByLen((uint8_t *)buf, len);
	m_file.flush();
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

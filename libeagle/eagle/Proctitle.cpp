#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "Proctitle.h"
#include "Log.h"

extern char **environ;

namespace eagle
{

Proctitle::Proctitle() : m_environ(NULL), m_last(NULL)
{
}

Proctitle::~Proctitle()
{
    if (NULL != m_environ) delete m_environ;
}

void Proctitle::init(const int argc, char *const *argv)
{
    int i;
    int len = 0;
    char * tmp;
    for (i = 0; environ[i]; ++i)
    {
        len += strlen(environ[i]) + 1; 
    }   

    m_environ = new char[len];
    tmp = m_environ;

    m_last = argv[0];
    for (i = 0; i < argc; ++i)
    {
        m_last += strlen(argv[i]) + 1;   
    }   
    for (i = 0; environ[i]; ++i)
    {
        if (m_last != environ[i]) continue;

        len = strlen(environ[i]) + 1;
        m_last += len;
        memcpy(tmp, environ[i], strlen(environ[i]) + 1); 
        environ[i] = tmp;
        tmp += len;
    }
    --m_last;
}

void Proctitle::setWorker(const int argc, char *const *argv, const char *format, ...)
{
    int len;
    va_list args;
    int size = m_last - argv[0];

    va_start(args, format);
    vsnprintf(argv[0], size, format, args);
    va_end(args);
    len = strlen(argv[0]);
    bzero(argv[0] + len, size - len);
}

void Proctitle::setMaster(const int argc, char *const *argv, const char *format, ...)
{
    int len;
    char buf[256];
    va_list args;
    char *tmp = buf;
    int size = sizeof(buf);

    for (int i = 0; i < argc && size > 1; ++i)
    {
        len = strlen(argv[i]);
        if (size < len) len = size - 1;
        *tmp++ = ' ';
        memcpy(tmp, argv[i], len);
        size -= (len - 1);
        tmp += len;
    }
    *tmp = 0;

    tmp = argv[0];
    size = m_last - tmp;
    va_start(args, format);
    vsnprintf(tmp, size, format, args);
    va_end(args);
    len = strlen(tmp);
    size -= len;
    tmp += len;
    len = strlen(buf);
    if (len > size) len = size;
    memcpy(tmp, buf, size);
    size -= len;
    tmp += len;

    if (size > 0) 
    {
        bzero(tmp, size);
    }else
    {
        *--tmp = 0;
    }
}

}

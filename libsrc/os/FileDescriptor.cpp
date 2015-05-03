#include "FileDescriptor.h"

int FileDescriptor::setFl(int flag)
{
    int flags = fcntl(m_fd, F_GETFL, 0);
    if (-1 == flags)
    {
        ERRORLOG1("F_GETFL failed, err %s", strerror(errno));

        return -1;
    }

    if (fcntl(m_fd, F_SETFL, flags | flag) == -1)
    {
        ERRORLOG1("F_SETFL %d failed, err %s", flag, strerror(errno));

        return -1;
    }

    return 0;
}

int FileDescriptor::setNonBlocking()
{
   return setFl(O_NONBLOCK); 
}

int FileDescriptor::setAsync()
{
    return setFl(O_ASYNC);
}

int FileDescriptor::setCloseOnexe()
{
    if (fcntl(m_fd, F_SETFD, FD_CLOEXEC) == -1)
    {
        ERRORLOG1("F_SETFD FD_CLOEXEC failed, err %s", strerror(errno));

        return -1;
    }

    return 0;
}

int FileDescriptor::setPid(pid_t pid)
{
    if (fcntl(m_fd, F_SETOWN, pid) == -1)
    {
        ERRORLOG1("F_SETFD failed, err %s", strerror(errno));

        return -1;
    }

    return 0;
}

int FileDescriptor::read(const char *str, const int len)
{
}

int FileDescriptor::read(const uint8_t *str, const int len)
{
}

int FileDescriptor::write(const char *str)
{

}

int FileDescriptor::write(const uint8_t *str, const int len)
{
    tmp = write(ss->fd, ib->buf + ib->r_offset, len);
    if(errno != EINTR) return;
}


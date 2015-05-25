#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "SocketEx.h"
#include "Define.h"
#include "Log.h"

Socket::Socket(const int fd) : m_fd(fd)
{
    if (-1 == m_fd) return;

    init();
}
Socket::Socket(const char *path)
{
    int ret;
    struct sockaddr_un addr;

    m_fd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (-1 == m_fd)
    {
        ERRORLOG1("socket err, %s", strerror(errno));

        return;
    }

    if (convertAddr(path, addr) != EG_SUCCESS)
    {
        close();

        return;
    }

    ret = bind(addr);
    EG_SUCCESS == ret ? init() : close();
}

Socket::Socket(const char *ip, int &port)
{
    int ret;
    struct sockaddr_in addr;

    m_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (-1 == m_fd)
    {
        ERRORLOG1("socket err, %s", strerror(errno));

        return;
    }

    convertAddr(ip, port, addr);
    ret = bind(addr);
    if (EG_SUCCESS == ret)
    {
        init();
        if (0 == port) port = ntohs(addr.sin_port);

        return;
    }

    close();
}

Socket::Socket(const char *path, const bool isServer)
{
    int ret;
    struct sockaddr_un addr;

    m_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (-1 == m_fd)
    {
        ERRORLOG1("socket err, %s", strerror(errno));

        return;
    }

    if (convertAddr(path, addr) != EG_SUCCESS)
    {
        close();

        return;
    }

    if (isServer)
    {
        ret = bind(addr);
        if (ret == EG_SUCCESS) ret = listen();
    }else
    {
        ret = connect(addr);
    }
    EG_SUCCESS == ret ? init() : close();

}

Socket::Socket(const char *ip, int &port, const bool isServer)
{
    int ret;
    struct sockaddr_in addr;

    m_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == m_fd)
    {
        ERRORLOG1("socket err, %s", strerror(errno));

        return;
    }

    convertAddr(ip, port, addr);
    if (isServer)
    {
        ret = bind(addr);
        if (ret == EG_SUCCESS)
        {
            ret = listen();
            if (0 == port) port = ntohs(addr.sin_port);
        }
    }else
    {
        ret = connect(addr);
    }
    EG_SUCCESS == ret ? init() : close();
}

Socket::~Socket()
{
    close();
}

int Socket::pair(int fd[2])
{
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, fd) != 0)
    {
        ERRORLOG1("socketpair err, %s", strerror(errno));

        return EG_FAILED;
    }

    return EG_SUCCESS;
}

void Socket::init()
{
    int flag;

    setNonBlocking();
    flag = 1;
    setsockopt(m_fd,SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));
}

void Socket::close()
{
    ::close(m_fd);
    m_fd = -1;
}

int Socket::setFl(int flag)
{
    int flags = fcntl(m_fd, F_GETFL, 0);
    if (-1 == flags)
    {
        ERRORLOG1("F_GETFL failed, err %s", strerror(errno));

        return EG_FAILED;
    }

    if (fcntl(m_fd, F_SETFL, flags | flag) == -1)
    {
        ERRORLOG2("F_SETFL %d failed, err %s", flag, strerror(errno));

        return EG_FAILED;
    }

    return EG_SUCCESS;
}

int Socket::setNonBlocking()
{
    if (-1 == m_fd) return EG_INVAL;

    return setFl(O_NONBLOCK);
}

int Socket::setAsync()
{
    if (-1 == m_fd) return EG_INVAL;

    return setFl(O_ASYNC);
}

int Socket::setCloseOnexe()
{
    if (-1 == m_fd) return EG_INVAL;

    if (fcntl(m_fd, F_SETFD, FD_CLOEXEC) == -1)
    {
        ERRORLOG1("F_SETFD FD_CLOEXEC failed, err %s", strerror(errno));

        return EG_FAILED;
    }

    return EG_SUCCESS;
}

int Socket::setPid(pid_t pid)
{
    if (-1 == m_fd) return EG_INVAL;

    if (fcntl(m_fd, F_SETOWN, pid) == -1)
    {
        ERRORLOG1("F_SETFD failed, err %s", strerror(errno));

        return EG_FAILED;
    }

    return EG_SUCCESS;
}

int Socket::convertAddr(const char *path, struct sockaddr_un &addr)
{
    addr.sun_family = AF_UNIX;
    
    strncpy(addr.sun_path, path, sizeof(addr.sun_path) - 1);
    if (strcmp(addr.sun_path, path) != 0)
    {
        ERRORLOG1("path length overflow, %s", path);

        return EG_FAILED;
    } 

    return EG_SUCCESS;
}

void Socket::convertAddr(const char *ip, const int port, struct sockaddr_in &addr)
{
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip);
}

int Socket::bind(struct sockaddr_un &addr)
{
    if (-1 == m_fd) return EG_INVAL;

    int oldUmask;
    struct stat tstat;

    if (lstat(addr.sun_path, &tstat) == 0) 
    {
        if (S_ISSOCK(tstat.st_mode))
            unlink(addr.sun_path);
    }

    oldUmask = umask(~(0777));
    if (::bind(m_fd, (struct sockaddr *)&addr, sizeof(addr)) == 0)
    {
        umask(oldUmask);

        return EG_SUCCESS;
    }

    umask(oldUmask);
    ERRORLOG1("bind err, %s", strerror(errno));

    return EG_FAILED;
}

int Socket::bind(struct sockaddr_in &addr)
{
    if (-1 == m_fd) return EG_INVAL;

    if (::bind(m_fd, (struct sockaddr *)&addr, sizeof(addr)) == 0)
        return EG_SUCCESS;

    ERRORLOG1("bind err, %s", strerror(errno));

    return EG_FAILED;
}

int Socket::listen()
{
    if (-1 == m_fd) return EG_INVAL;

    if (::listen(m_fd, EG_LISTEN_BACKLOG) == 0)
        return EG_SUCCESS;

    ERRORLOG1("listen err, %s", strerror(errno));

    return EG_FAILED;
}

int Socket::connect(const char *path)
{
    struct sockaddr_un addr;

    if (convertAddr(path, addr) == EG_FAILED) return EG_FAILED;

    return connect(addr);
}

int Socket::connect(const char *ip, const int port)
{
    struct sockaddr_in addr;

    convertAddr(ip, port, addr);

    return connect(addr);
}

int Socket::connect(struct sockaddr_un &addr)
{
    if (-1 == m_fd) return EG_INVAL;

    for (; ;)
    {
        if (::connect(m_fd, (struct sockaddr *)&addr, sizeof(addr)) == 0)
            return EG_SUCCESS;

        if (EINTR == errno) continue;

        ERRORLOG1("connect err, %s", strerror(errno));

        return EG_FAILED;
    }

    return EG_SUCCESS;
}

int Socket::connect(struct sockaddr_in &addr)
{
    if (-1 == m_fd) return EG_INVAL;

    for (; ;)
    {
        if (::connect(m_fd, (struct sockaddr *)&addr, sizeof(addr)) == 0)
            return EG_SUCCESS;

        if (EINTR == errno) continue;

        ERRORLOG1("connect err, %s", strerror(errno));

        return EG_FAILED;
    }

    return EG_SUCCESS;
}

int Socket::accept(int &fd, struct sockaddr *addr, socklen_t *addrLen)
{
    for (; ;)
    {
        fd = ::accept(m_fd, addr, addrLen);
        if (-1 != fd) return EG_SUCCESS;

        if (EINTR == errno) continue;

        if (EAGAIN == errno || ECONNABORTED == errno)
            return EG_AGAIN;

        ERRORLOG1("accept err, %s", strerror(errno));

        return EG_FAILED;
    }

    return EG_SUCCESS;
}

int Socket::accept(int &fd, struct sockaddr_in &addr)
{
    if (-1 == m_fd) return EG_INVAL;

    socklen_t len[1] = { sizeof(addr) };

    return accept(fd, (struct sockaddr *)&addr, len);
}

int Socket::accept(int &fd, struct sockaddr_un &addr)
{
    if (-1 == m_fd) return EG_INVAL;

    socklen_t len[1] = { sizeof(addr) };

    return accept(fd, (struct sockaddr *)&addr, len);
}

int Socket::send(const uint8_t *buf, int &len, 
        const int flags)
{
    if (-1 == m_fd) return EG_INVAL;

    int ret;
    for (; ;)
    {
        ret = ::send(m_fd, (const void *)buf, len, flags);
        if (ret >= 0)
        {
            len = ret;

            return EG_SUCCESS;
        }
        if (EINTR == errno) continue;

        if (EAGAIN == errno) return EG_AGAIN;

        ERRORLOG1("send err, %s", strerror(errno));

        return EG_FAILED;
    }

    return EG_SUCCESS;
}

int Socket::sendTo(const uint8_t *buf, int &len, const int flags, 
        const struct sockaddr *addr, socklen_t addrLen)
{
    int ret;
    for (; ;)
    {
        ret = ::sendto(m_fd, buf, len, flags, addr, addrLen);
        if (ret >= 0)
        {
            len = ret;

            return EG_SUCCESS;
        }
        if (EINTR == errno) continue;

        if (EAGAIN == errno) return EG_AGAIN;

        ERRORLOG1("sendto err, %s", strerror(errno));

        return EG_FAILED;
    }

    return EG_SUCCESS;
}

int Socket::sendTo(const uint8_t *buf, int &len, 
        const struct sockaddr_in &addr, const int flags)
{
    if (-1 == m_fd) return EG_INVAL;

    return sendTo(buf, len, flags, (const struct sockaddr *)&addr,
            sizeof(addr));
}

int Socket::sendTo(const uint8_t *buf, int &len, 
        const struct sockaddr_un &addr, const int flags)
{
    if (-1 == m_fd) return EG_INVAL;

    return sendTo(buf, len, flags, (const struct sockaddr *)&addr,
            sizeof(addr));
}

int Socket::sendMsg(const int fd, const uint8_t *buf, int &len,
        const int flags)
{
    if (-1 == m_fd) return EG_INVAL;

    int ret;
    struct iovec iov[1];
    struct msghdr msg;
    union {
        struct cmsghdr  cm;
        char            space[CMSG_SPACE(sizeof(int))];
    } cmsg;

    if (-1 == fd) {
        msg.msg_control = NULL;
        msg.msg_controllen = 0;

    } else {
        msg.msg_control = (caddr_t) &cmsg;
        msg.msg_controllen = sizeof(cmsg);
        cmsg.cm.cmsg_len = CMSG_LEN(sizeof(int));
        cmsg.cm.cmsg_level = SOL_SOCKET;
        cmsg.cm.cmsg_type = SCM_RIGHTS;
        memcpy(CMSG_DATA(&cmsg.cm), &fd, sizeof(int));
    }

    msg.msg_flags = 0;
    iov[0].iov_base = (char *)buf;
    iov[0].iov_len = len;
    msg.msg_name = NULL;
    msg.msg_namelen = 0;
    msg.msg_iov = iov;
    msg.msg_iovlen = 1;

    for (; ;)
    {
        ret = ::sendmsg(m_fd, &msg, flags);
        if (ret >= 0)
        {
            if(0 == ret) ERRORLOG("sendmsg() returned zero");
            len = ret;

            return EG_SUCCESS;
        }
        if (EINTR == errno) continue;

        if (EAGAIN == errno) return EG_AGAIN;

        ERRORLOG1("sendmsg err, %s", strerror(errno));

        return EG_FAILED;
    }

    return EG_SUCCESS;
}

int Socket::recv(uint8_t *buf, int &len, const int flags)
{
    if (-1 == m_fd) return EG_INVAL;

    int ret;
    for (; ;)
    {
        ret = ::recv(m_fd, (void *)buf, len, flags);
        if (ret >= 0)
        {
            len = ret;

            return EG_SUCCESS;
        }
        if (EINTR == errno) continue;

        if (EAGAIN == errno) return EG_AGAIN;

        ERRORLOG1("recv err, %s", strerror(errno));

        return EG_FAILED;
    }

    return EG_SUCCESS;
}

int Socket::recvFrom(uint8_t *buf, int &len, const int flags, 
        struct sockaddr *addr, socklen_t *addrLen)
{
    int ret;

    for (; ;)
    {
        ret = ::recvfrom(m_fd, (void *)buf, len, flags, addr, addrLen);
        if (ret >= 0)
        {
            len = ret;

            return EG_SUCCESS;
        }
        if (EINTR == errno) continue;

        if (EAGAIN == errno) return EG_AGAIN;

        ERRORLOG1("recvFrom err, %s", strerror(errno));

        return EG_FAILED;
    }

    return EG_SUCCESS;
}

int Socket::recvFrom(uint8_t *buf, int &len, 
        struct sockaddr_in &addr, const int flags)
{
    if (-1 == m_fd) return EG_INVAL;

    socklen_t addrLen[1] = { sizeof(addr) };

    return recvFrom(buf, len, flags, 
            (struct sockaddr *)&addr, addrLen);
}

int Socket::recvFrom(uint8_t *buf, int &len, 
        struct sockaddr_un &addr, const int flags)
{
    if (-1 == m_fd) return EG_INVAL;

    socklen_t addrLen[1] = { sizeof(addr) };

    return recvFrom(buf, len, flags, 
            (struct sockaddr *)&addr, addrLen);
}

int Socket::recvMsg(int &fd, uint8_t *buf, int &len, 
        const int flags)
{
    if (-1 == m_fd) return EG_INVAL;

    int ret;
    struct iovec iov[1];
    struct msghdr msg;

    union {
        struct cmsghdr  cm;
        char            space[CMSG_SPACE(sizeof(int))];
    } cmsg;

    iov[0].iov_base = (char *)buf;
    iov[0].iov_len = len;
    msg.msg_name = NULL;
    msg.msg_namelen = 0;
    msg.msg_iov = iov;
    msg.msg_iovlen = 1;
    msg.msg_control = (caddr_t) &cmsg;
    msg.msg_controllen = sizeof(cmsg);

    for (; ;)
    {
        ret = ::recvmsg(m_fd, &msg, flags);
        if (ret >= 0)
        {
            memcpy(&fd, CMSG_DATA(&cmsg.cm), sizeof(int));
            len = ret;

            return EG_SUCCESS;
        }
        if (EINTR == errno) continue;

        if (EAGAIN == errno) return EG_AGAIN;

        ERRORLOG1("recvmsg err, %s", strerror(errno));

        return EG_FAILED;
    }

    return EG_SUCCESS;
}

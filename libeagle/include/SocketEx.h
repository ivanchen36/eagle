/**
 *       Filename:  Socket.h
 *
 *    Description:  socket
 *
 *        Version:  1.0
 *        Created:  05/03/2015 08:53:31 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan Chen, 228268157@qq.com
 *   Organization:  
 */
#ifndef _EAGLE_SOCKET_H_
#define _EAGLE_SOCKET_H_
#include <stdint.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <arpa/inet.h>

#include "AutoPtr.h"

namespace eagle
{

class Socket
{
public:
	Socket(const int fd, const int isUinx = 0);
	Socket(const char *path);
	Socket(const char *ip, int &port);
	Socket(const char *path, const bool isServer);
	Socket(const char *ip, int &port, const bool isServer);
	virtual ~Socket();

    int isAvailable() {return -1 != m_fd;}
    int getFd() { return m_fd; }
	int setAsync();
	int setPid(pid_t pid);
    int setCloseOnexe();
	int setNonBlocking();
    int setCork();
    int setNodelay();
    int setReUseAddr();
    int setNotLinger();
    int setKeepAlive();
    int setNoBuf();
    int setDeferAccept();
    int convertAddr(const char *path, struct sockaddr_un &addr);
    void convertAddr(const char *ip, const int port, struct sockaddr_in &addr);
    int accept(int &fd, struct sockaddr_in &addr);
    int accept(int &fd, struct sockaddr_un &addr);
    int connect(const char *path);
    int connect(const char *ip, const int port);
    int send(const uint8_t *buf, int &len, const int flags = 0);
    int sendTo(const uint8_t *buf, int &len, const struct sockaddr_in &addr, 
            const int flags = 0);
    int sendTo(const uint8_t *buf, int &len, const struct sockaddr_un &addr, 
            const int flags = 0);
    int sendMsg(const int fd, const uint8_t *buf, int &len, const int flags = 0);
    int recv(uint8_t *buf, int &len, const int flags = 0);
    int recvFrom(uint8_t *buf, int &len, struct sockaddr_in &addr, 
            const int flags = 0);
    int recvFrom(uint8_t *buf, int &len, struct sockaddr_un &addr, 
            const int flags = 0);
    int recvMsg(int &fd, uint8_t *buf, int &len, const int flags = 0);
    static int pair(int fd[2]);

private:
    void init(const int isServer = 0, const int isUinx = 0);
    void initSndRcv(const int isUinx);
    void initAccept(const int isUinx);
    void close();
    int listen();
    int setFl(int flag);
    int connect(struct sockaddr_in &addr);
    int connect(struct sockaddr_un &addr);
    int bind(struct sockaddr_in &addr);
    int bind(struct sockaddr_un &addr);
    int accept(int &fd, struct sockaddr *addr, socklen_t *addrLen);
    int sendTo(const uint8_t *buf, int &len, const int flags, 
            const struct sockaddr *addr, socklen_t addrLen);
    int recvFrom(uint8_t *buf, int &len, const int flags, 
            struct sockaddr *addr, socklen_t *addrLen);

    int m_fd;
};

typedef AutoPtr<Socket> SocketPtr;

}
#endif   /* ----- #ifndef _EAGLE_SOCKET_H_  ----- */

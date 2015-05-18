/**
 *       Filename:  ShmTest.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  04/26/2015 02:18:19 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan Chen, 228268157@qq.com
 *   Organization:  
 */


#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <sys/wait.h>
#include <fcntl.h>

#include "Log.h"
#include "EagleTime.h"
#include "SocketEx.h"
#include "Thread.h"

using namespace std;

const char *CLIENTDIALOGARR[] = 
{
    "hello server",
    "i'm client",
    "glad to see you",
    "bye server"
};

const char *SERVERDIALOGARR[] = 
{
    "hello client",
    "i'm server",
    "mee too",
    "bye client"
};

void client(void *param)
{
    int ret;
    int isRecv;
    int tryTime = 3;
    char buf[48];
    int len;
    int line = 0;
    const char *dialog;
    int port = 17801;
    int isLocal = *(int *)param;
    SocketPtr socket;
    if (isLocal)
    {
        socket = new Socket("./test17801", 0);
    }else
    {
        socket = new Socket("127.0.0.1", port, 0);
    }

    for (; !socket->isAvailable(); --tryTime)
    {
        if (tryTime == 0)
        {
            ERRORLOG("new socket err, quit");

            return;
        }
        sleep(1);
        if (isLocal)
        {
            socket = new Socket("./test17801", 0);
        }else
        {
            socket = new Socket("127.0.0.1", port, 0);
        }
    }
    
    isRecv = 0;
    for (; ; sleep(1))
    {
        if (!isRecv)
        {
            dialog = (CLIENTDIALOGARR[line]);
            len = strlen(dialog);
            ret = socket->send((const uint8_t *)dialog, len);
            if (ret < 0)
            {
                ERRORLOG("send err, quit");

                return;
            }
            isRecv = 1;
        }else
        {
            dialog = (SERVERDIALOGARR[line]);
            len = sizeof(buf);
            ret = socket->recv((uint8_t *)buf, len);
            if (ret > 0) continue;

            if (ret < 0)
            {
                ERRORLOG("recv err, quit");

                return;
            }
            buf[len] = 0;
            DEBUGLOG1("server say : %s", buf);
            if (strcmp(buf, dialog) != 0)
            {
                ERRORLOG("protocol err, quit");

                return;
            }

            if (line == 3)
            {
                INFOLOG("client dialog finish, quit");

                return;
            }

            ++line;
            isRecv = 0;
        }
    }
}

void hanlerClient(int fd)
{
    int ret;
    int isRecv;
    char buf[48];
    int len;
    int line = 0;
    const char *dialog;
    SocketPtr socket = new Socket(fd);
    
    isRecv = 1;
    for (; ; sleep(1))
    {
        if (!isRecv)
        {
            dialog = (SERVERDIALOGARR[line]);
            len = strlen(dialog);
            ret = socket->send((const uint8_t *)dialog, len);
            if (ret < 0)
            {
                ERRORLOG("send err, quit");

                return;
            }
            if (line == 3)
            {
                INFOLOG("server dialog finish, quit");
                sleep(2);

                return;
            }

            isRecv = 1;
            ++line;
        }else
        {
            dialog = (CLIENTDIALOGARR[line]);
            len = sizeof(buf);
            ret = socket->recv((uint8_t *)buf, len);
            if (ret > 0) continue;

            if (ret < 0)
            {
                ERRORLOG("recv err, quit");

                return;
            }
            buf[len] = 0;
            DEBUGLOG1("client say : %s", buf);
            if (strcmp(buf, dialog) != 0)
            {
                ERRORLOG("protocol err, quit");

                return;
            }

            isRecv = 0;
        }
    }
}

void server(int isLocal)
{
    int ret;
    int clientFd;
    CallBack cb(client, &isLocal);
    struct sockaddr_in addr;
    int port = 17801;
    SocketPtr socket;
    if (isLocal)
    {
        socket = new Socket("./test17801", 1);
    }else
    {
        socket = new Socket("127.0.0.1", port, 1);
    }
    if (!socket->isAvailable())
    {
        ERRORLOG("new socket err, quit");

        return;
    }
    Thread clientThread(cb);
    for (; ;sleep(1))
    {
        ret = socket->accept(clientFd, addr);
        if (ret > 0) continue;
        if (ret < 0)
        {
            ERRORLOG("accept err, quit");

            return;
        }

        break;
    }

    hanlerClient(clientFd);
}

void dotest()
{
    server(1);
}

void client1(void *param)
{
    int ret;
    int isRecv;
    int tryTime = 3;
    char buf[48];
    int len;
    int line = 0;
    const char *dialog;
    int port = 17802;
    int isLocal = *(int *)param;
    SocketPtr socket;
    if (isLocal)
    {
        socket = new Socket("./test17802");
    }else
    {
        socket = new Socket("127.0.0.1", port);
    }

    if (isLocal)
    {
        socket->connect("./test17801");
    }else
    {
        socket->connect("127.0.0.1", 17801);
    }

    isRecv = 0;
    for (; ; sleep(1))
    {
        if (!isRecv)
        {
            dialog = (CLIENTDIALOGARR[line]);
            len = strlen(dialog);
            ret = socket->send((const uint8_t *)dialog, len);
            if (ret < 0)
            {
                ERRORLOG("send err, quit");

                continue;
            }
            isRecv = 1;
        }else
        {
            dialog = (SERVERDIALOGARR[line]);
            len = sizeof(buf);
            ret = socket->recv((uint8_t *)buf, len);
            if (ret > 0) continue;

            if (ret < 0)
            {
                ERRORLOG("recv err, quit");

                return;
            }
            buf[len] = 0;
            DEBUGLOG1("server say : %s", buf);
            if (strcmp(buf, dialog) != 0)
            {
                ERRORLOG("protocol err, quit");

                return;
            }

            if (line == 3)
            {
                INFOLOG("client dialog finish, quit");

                return;
            }

            ++line;
            isRecv = 0;
        }
    }
}

void hanlerClient1(SocketPtr &socket)
{
    int ret = -1;
    int isRecv;
    char buf[48];
    int len;
    int line = 0;
    const char *dialog;
    
    isRecv = 1; 
    for (; ; sleep(1))
    {
        if (!isRecv)
        {
            dialog = (SERVERDIALOGARR[line]);
            len = strlen(dialog);
            ret = socket->send((const uint8_t *)dialog, len);
            if (ret < 0)
            {
                ERRORLOG("send err, quit");

                return;
            }
            if (line == 3)
            {
                INFOLOG("server dialog finish, quit");
                sleep(2);

                return;
            }

            isRecv = 1;
            ++line;
        }else
        {
            dialog = (CLIENTDIALOGARR[line]);
            len = sizeof(buf);
            ret = socket->recv((uint8_t *)buf, len);
            if (ret > 0) continue;

            if (ret < 0)
            {
                ERRORLOG("recv err, quit");

                return;
            }
            buf[len] = 0;
            DEBUGLOG1("client say : %s", buf);
            if (strcmp(buf, dialog) != 0)
            {
                ERRORLOG("protocol err, quit");

                return;
            }

            isRecv = 0;
        }
    }
}

void server1(int isLocal)
{
    int ret = -1;
    int clientFd;
    CallBack cb(client1, &isLocal);
    int port = 17801;
    SocketPtr socket;
    if (isLocal)
    {
        socket = new Socket("./test17801");
    }else
    {
        socket = new Socket("127.0.0.1", port);
    }

    Thread clientThread(cb); 
    for (; ret == -1; sleep(1))
    {
        if (isLocal)
        {
            ret = socket->connect("./test17802");
        }else
        {
            ret = socket->connect("127.0.0.1", 17802);
        }
    }
    hanlerClient1(socket);
}

void dotest1()
{
    server1(0);
}

void client2(void *param)
{
    int ret;
    int isRecv;
    int tryTime = 3;
    char buf[48];
    int len;
    int line = 0;
    const char *dialog;
    int port = 17802;
    struct sockaddr_in addr;
    SocketPtr socket = new Socket("127.0.0.1", port);

    isRecv = 0;
    socket->convertAddr("127.0.0.1", 17801, addr);
    for (; ; sleep(1))
    {
        if (!isRecv)
        {
            dialog = (CLIENTDIALOGARR[line]);
            len = strlen(dialog);
            ret = socket->sendTo((const uint8_t *)dialog, len, addr);
            if (ret < 0)
            {
                ERRORLOG("send err, quit");

                continue;
            }
            isRecv = 1;
        }else
        {
            dialog = (SERVERDIALOGARR[line]);
            len = sizeof(buf);
            ret = socket->recvFrom((uint8_t *)buf, len, addr);
            if (ret > 0) continue;

            if (ret < 0)
            {
                ERRORLOG("recv err, quit");

                return;
            }
            buf[len] = 0;
            DEBUGLOG1("server say : %s", buf);
            if (strcmp(buf, dialog) != 0)
            {
                ERRORLOG("protocol err, quit");

                return;
            }

            if (line == 3)
            {
                INFOLOG("client dialog finish, quit");

                return;
            }

            ++line;
            isRecv = 0;
        }
    }
}

void clientLocal2(void *param)
{
    int ret;
    int isRecv;
    int tryTime = 3;
    char buf[48];
    int len;
    int line = 0;
    const char *dialog;
    struct sockaddr_un addr;
    SocketPtr socket = new Socket("./test17802");;

    isRecv = 0;
    socket->convertAddr("./test17801", addr);
    for (; ; sleep(1))
    {
        if (!isRecv)
        {
            dialog = (CLIENTDIALOGARR[line]);
            len = strlen(dialog);
            ret = socket->sendTo((const uint8_t *)dialog, len, addr);
            if (ret < 0)
            {
                ERRORLOG("send err, quit");

                continue;
            }
            isRecv = 1;
        }else
        {
            dialog = (SERVERDIALOGARR[line]);
            len = sizeof(buf);
            ret = socket->recvFrom((uint8_t *)buf, len, addr);
            if (ret > 0) continue;

            if (ret < 0)
            {
                ERRORLOG("recv err, quit");

                return;
            }
            buf[len] = 0;
            DEBUGLOG1("server say : %s", buf);
            if (strcmp(buf, dialog) != 0)
            {
                ERRORLOG("protocol err, quit");

                return;
            }

            if (line == 3)
            {
                INFOLOG("client dialog finish, quit");

                return;
            }

            ++line;
            isRecv = 0;
        }
    }
}

void hanlerClient2(SocketPtr &socket)
{
    int ret = -1;
    int isRecv;
    char buf[48];
    int len;
    int line = 0;
    const char *dialog;
    struct sockaddr_in addr;
    
    isRecv = 1; 
    for (; ; sleep(1))
    {
        if (!isRecv)
        {
            dialog = (SERVERDIALOGARR[line]);
            len = strlen(dialog);
            ret = socket->sendTo((const uint8_t *)dialog, len, addr);
            if (ret < 0)
            {
                ERRORLOG("send err, quit");

                return;
            }
            if (line == 3)
            {
                INFOLOG("server dialog finish, quit");
                sleep(2);

                return;
            }

            isRecv = 1;
            ++line;
        }else
        {
            dialog = (CLIENTDIALOGARR[line]);
            len = sizeof(buf);
            ret = socket->recvFrom((uint8_t *)buf, len, addr);
            if (ret > 0) continue;

            if (ret < 0)
            {
                ERRORLOG("recv err, quit");

                return;
            }
            buf[len] = 0;
            DEBUGLOG1("client say : %s", buf);
            if (strcmp(buf, dialog) != 0)
            {
                ERRORLOG("protocol err, quit");

                return;
            }

            isRecv = 0;
        }
    }
}

void hanlerLocalClient2(SocketPtr &socket)
{
    int ret = -1;
    int isRecv;
    char buf[48];
    int len;
    int line = 0;
    const char *dialog;
    struct sockaddr_un addr;
    
    isRecv = 1; 
    for (; ; sleep(1))
    {
        if (!isRecv)
        {
            dialog = (SERVERDIALOGARR[line]);
            len = strlen(dialog);
            ret = socket->sendTo((const uint8_t *)dialog, len, addr);
            if (ret < 0)
            {
                ERRORLOG("send err, quit");

                return;
            }
            if (line == 3)
            {
                INFOLOG("server dialog finish, quit");
                sleep(2);

                return;
            }

            isRecv = 1;
            ++line;
        }else
        {
            dialog = (CLIENTDIALOGARR[line]);
            len = sizeof(buf);
            ret = socket->recvFrom((uint8_t *)buf, len, addr);
            if (ret > 0) continue;

            if (ret < 0)
            {
                ERRORLOG("recv err, quit");

                return;
            }
            buf[len] = 0;
            DEBUGLOG1("client say : %s", buf);
            if (strcmp(buf, dialog) != 0)
            {
                ERRORLOG("protocol err, quit");

                return;
            }

            isRecv = 0;
        }
    }
}

void server2(int isLocal)
{
    int ret = -1;
    int clientFd;
    int port = 17801;
    SocketPtr socket;

    if (isLocal)
    {
        socket = new Socket("./test17801");
    }else
    {
        socket = new Socket("127.0.0.1", port);
    }

    if (!socket->isAvailable())
    {
        ERRORLOG("new socket err, quit");

        return;
    }
    if (isLocal)
    {
        CallBack cb(clientLocal2, NULL);
        Thread clientThread(cb); 
        hanlerClient2(socket);
    }else
    {
        CallBack cb(client2, NULL);
        Thread clientThread(cb); 
        hanlerLocalClient2(socket);
    }
    
}

void dotest2()
{
    server2(1);
}

void child(int fd)
{
    int ret = -1;
    int file;
    char buf[48];
    int len = sizeof(buf);
    SocketPtr socket = new Socket(fd);
    for (; ret != 0;)
    {
        ret = socket->recvMsg(file, (uint8_t *)buf, len);
        if (ret < 0)
        {
            ERRORLOG("recvMsg err quit");
        }
    }
    DEBUGLOG1("recvMsg %s", buf);

    write(file, buf, len);
}

void parent(int fd)
{
    const char *str = "it's a file\n";
    SocketPtr socket = new Socket(fd);
    int len = strlen(str);
    socket->sendMsg(STDOUT_FILENO, (const uint8_t *)str, len);
    DEBUGLOG("sendmsg");
    waitpid(-1, NULL, 0);
}

void dotest3()
{
    int fd[2];
    Socket::pair(fd);
    pid_t pid=fork();
    if(pid<0)
    {
        ERRORLOG("fork err");

        return;
    }
    else if(pid == 0)
    {
        close(fd[0]);
        child(fd[1]);
    }
    else
    {
        close(fd[1]);
        parent(fd[0]);
    }
}

/**
 * @brief main 
 */
int main ( int argc, char *argv[] )
{
    //EagleTimeI::instance().autoUpdate();
    if (g_sysLog->redirectToOther(STDOUT_FILENO))
    {
        DEBUGLOG("redirectToOther err");
    }

    dotest3();

    return EXIT_SUCCESS;
}

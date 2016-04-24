/**
 *       Filename:  ClassTest.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/27/2015 09:08:54 PM
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
#include "ServerTime.h"
#include "EpollManager.h"
#include "SelectManager.h"
#include "ReceiveHandler.h"
#include "AcceptHandler.h"
#include "MessageHandlerFactory.h"

using namespace std;
using namespace eagle;

namespace
{
    int g_helloPort = 13312;
    const char *g_helloServer = "testserver";
    const char *clientContent = "hello server";
    const int clientLen = strlen(clientContent);
    //EventManager *accpetEpoll = new EpollManager(2);
    //EventManager *receiveEpoll = new EpollManager(8);

    EventManager *accpetEpoll = new SelectManager(2);
    EventManager *receiveEpoll = new SelectManager(8);
    IoBufPoolManager &bufPoolManager = IoBufPoolManagerI::instance();
}

class HelloMessage : public MessageHandler
{
public:
    int handleMultiBuf(const char *buf, const int len, IoBuffer *&ioBuf)
    {
        if (NULL == ioBuf->next) return -1;
        
        IoBuffer *tmp = ioBuf->next;
        char *tmpBuf = (char *)ioBuf->buf + ioBuf->offset;
        int tmpLen = tmp->cur - tmp->offset;
        if (tmpLen + len < clientLen) return -1;
        
        ioBuf = tmp;
        tmpLen = clientLen - len;
        ioBuf->offset = ioBuf->offset + tmpLen;
        if (strncmp(buf, clientContent, len) == 0
                && strncmp(tmpBuf, clientContent + len, tmpLen) == 0)
        {
            response();
        }

        return 0;
    }

    void response()
    {

        DEBUGLOG("response");
        send(clientContent, clientLen);
    }

    IoBuffer *handle(IoBuffer *ioBuf)
    {
        char *tmp;
        int len;
        
        DEBUGLOG("handle");
        for (; ;)
        {
            tmp = (char *)ioBuf->buf + ioBuf->offset;
            len = ioBuf->cur - ioBuf->offset;
            if (clientLen > len && handleMultiBuf(tmp, len, ioBuf) != 0) break;

            ioBuf->offset = ioBuf->offset + len;
            if (strncmp(tmp, clientContent, clientLen) == 0)
            {
                response();
            }

            if (ioBuf->offset == ioBuf->cur) ioBuf = ioBuf->next;

            if (NULL == ioBuf) break;
        }

        return ioBuf;
    }
};

REGISTER_REFLECTOR(g_helloServer, HelloMessage);

void startClient(EventManager *clientEpoll)
{
    Socket *socket = new Socket("127.0.0.1", g_helloPort, 0);

    if (!socket->isAvailable())
    {
        ERRORLOG("bind socket err");

        return;
    }
    clientEpoll->registerEvent(READ, new ReceiveHandler(
                clientEpoll, socket, new HelloMessage(), 
                bufPoolManager.getBufPool()));
    int len = clientLen;
    socket->send((const uint8_t *)clientContent, len);
}

void startClient1(void *param)
{
    char buf[48];
    int len;
    int ret;
    SocketPtr socket;
    int *isStop = (int *)param;

    for (int i = 0; !*isStop && i < 100000; ++i)
    {
        socket = new Socket("127.0.0.1", g_helloPort, 0);

        if (!socket->isAvailable())
        {
            ERRORLOG("connect socket err");

            return;
        }

        len = clientLen;
        for (; !*isStop; )
        {
            ret = socket->send((const uint8_t *)clientContent, len);
            if (ret == -1) ERRORLOG("send err");

            if (ret != 1) break;

            sched_yield();
        }
        for (; !*isStop; )
        {
            len = sizeof(buf);
            ret = socket->recv((uint8_t *)buf, len);
            if (ret == -1) ERRORLOG("recv err");

            if (ret != 1) break;

            sched_yield();
        }
        if (0 == ret)
        {
            if (strncmp(buf, clientContent, clientLen) != 0)
            {
                ERRORLOG("proto err");
            }
        }
    }
}

void startServer(void *param)
{
    if (NULL == param)
    {
        accpetEpoll->loop();
    }else
    {
        receiveEpoll->loop();
    }
}

void test()
{
    int testPort = 13312;
    int isStop = 0;
    Socket *socket = new Socket("127.0.0.1", g_helloPort, 1);
    if (!socket->isAvailable())
    {
        ERRORLOG("bind socket err");

        return;
    }

    EventHandler *accepter = new AcceptHandler(receiveEpoll, socket, g_helloPort, 1);
    MessageHandlerFactoryI::instance().registerHandler(g_helloPort, g_helloServer);
    accpetEpoll->registerEvent(READ, accepter);

    CallBack cb(startServer, (void *)NULL);
    Thread thread(cb);
    CallBack cb1(startServer, (void *)1);
    Thread thread1(cb1);


    sleep(1);
    for (int i = 0; i < 50; ++i)
    {
        CallBack cb(startClient1, (void *)&isStop);
        Thread thread(cb);
    }

    for (int i = 0; i < 1000; ++i)
    {
        sleep(1);
    }
    DEBUGLOG("stop test");
    isStop = 1;
    sleep(1);

    delete accpetEpoll;
    delete receiveEpoll;
}

/**
 * @brief main 
 */
int main ( int argc, char *argv[] )
{
    test();

    return EXIT_SUCCESS;
}

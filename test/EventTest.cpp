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
#include "EagleTime.h"
#include "EpollManager.h"
#include "SelectManager.h"
#include "ReceiveHandler.h"
#include "AcceptHandler.h"
#include "MessageHandlerFactory.h"

using namespace std;

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
        m_eventHandler->write((const uint8_t *)clientContent, clientLen);
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
                clientEpoll, socket, new HelloMessage()));
    int len = clientLen;
    socket->send((const uint8_t *)clientContent, len);
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
    Socket *socket = new Socket("127.0.0.1", g_helloPort, 1);
    if (!socket->isAvailable())
    {
        ERRORLOG("bind socket err");

        return;
    }

    EventHandler *accepter = new AcceptHandler(receiveEpoll, socket, g_helloPort);
    MessageHandlerFactoryI::instance().registerHandler(g_helloPort, g_helloServer);
    accpetEpoll->registerEvent(READ, accepter);

    CallBack cb(startServer, (void *)NULL);
    Thread thread(cb);
    CallBack cb1(startServer, (void *)1);
    Thread thread1(cb1);


    sleep(1);
    for (int i = 0; i < 10; ++i)
    {
        startClient(receiveEpoll);
    }

    for (int i = 0; i < 100; ++i)
    {
        sleep(1);
    }

    delete accpetEpoll;
    delete receiveEpoll;
}

/**
 * @brief main 
 */
int main ( int argc, char *argv[] )
{
    EagleTimeI::instance().autoUpdate();

    test();

    return EXIT_SUCCESS;
}

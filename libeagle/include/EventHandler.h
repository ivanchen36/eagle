/**
 *       Filename:  EventHandler.h
 *
 *    Description:  fd event handler
 *
 *        Version:  1.0
 *        Created:  05/18/2015 11:57:32 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan Chen, 228268157@qq.com
 *   Organization:  
 */
#ifndef _EAGLE_EVENTHANDLER_H_
#define _EAGLE_EVENTHANDLER_H_

#include "AutoPtr.h"
#include "AutoPtr1.h"
#include "Define.h"
#include "SocketEx.h"
#include "IoBuffer.h"
#include "Log.h"
#include "ClassFactory.h"

namespace eagle
{

class EventManager;

enum Event
{
    NONE = 0X00,
    READ = 0X01,
    WRITE = 0X02,
    RDWR = 0X03
};

class EventHandler : public Reference
{
public:
    EventHandler(EventManager *const manager, const int fd);
    EventHandler(EventManager *const manager, Socket *socket);
    virtual ~EventHandler();

    void activateRead()
    {
        m_event |= READ;
    }

    void activateWrite()
    {
        m_event |= WRITE;
    }

    void inactivateRead()
    {
        m_event &= ~READ;
    }

    void inactivateWrite()
    {
        m_event &= ~WRITE;
    }

    int hasRead()
    {
        return m_event & READ;
    }

    int hasWrite()
    {
        return m_event & WRITE;
    }

    int getFd()
    {
        return m_socket->getFd();
    }

    int &getRegisterEvent()
    {
        return m_registerEvent;
    }

    void clearRegisterEvent();
    virtual int read() = 0;
    virtual int write()
    {
        ERRORLOG("need override");

        return EG_FAILED;
    }

    virtual int write(const uint8_t *buf, int size) 
    {
        ERRORLOG("need override");

        return EG_FAILED;
    }

protected:    
    int m_event;
    int m_registerEvent;
    Socket *m_socket;
    EventManager *const m_manager;
};

typedef AutoPtr1<EventHandler> EventHandlerPtr;

class MessageHandler
{
public:
    virtual ~MessageHandler(){}
    virtual IoBuffer *handle(IoBuffer *ioBuf) = 0;

    void setEventHandler(EventHandler *eventHandler)
    {
       m_eventHandler = eventHandler;
    }

    void send(const char *buf)
    {
        m_eventHandler->write((const uint8_t *)buf, strlen(buf));
    }

    void send(const char *buf, const int len)
    {
        m_eventHandler->write((const uint8_t *)buf, len);
    }

    void send(const uint8_t *buf, const int len)
    {
        m_eventHandler->write(buf, len);
    }

    void close()
    {
        m_eventHandler->clearRegisterEvent();
    }

private:
    EventHandler *m_eventHandler;
};

}
#endif   /* ----- #ifndef _EAGLE_EVENTHANDLER_H_  ----- */

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
#ifndef  _EVENTHANDLER_H_
#define  _EVENTHANDLER_H_

#include <sched.h>

#include "AutoPtr.h"
#include "AutoPtr1.h"
#include "Define.h"
#include "SocketEx.h"
#include "Log.h"

class EventManager;

enum Event
{
    NONE = 0X00,
    READ = 0X01,
    WRITE = 0X02,
    RDWR = 0X03
};

struct IoBuffer
{
    IoBuffer *next;
    uint16_t offset;
    uint16_t cur;
    uint8_t buf[1];
    const static uint16_t size;

    IoBuffer()
    {
        cur = 0;
        offset = 0;
        next = NULL;
    }

    void reset()
    {
        cur = 0;
        offset = 0;
        next = NULL;
    }

    void *operator new(size_t s)
    {
        return (void *)new char [EG_IO_BUF_SIZE];
    }

    void operator delete(void* ptr)
    {
        delete [] (char *)ptr;
    }
};

union IoBufferUnion
{
    struct BufStruct
    {
        BufStruct *next;
        uint16_t offset;
        uint16_t cur;
        uint8_t buf[1];
    } ioBuf;
    char buf[EG_IO_BUF_SIZE];
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
    void releaseBuf();

    int m_event;
    int m_registerEvent;
    Socket *m_socket;
    IoBuffer *m_readBuf;
    IoBuffer *m_writeBuf;
    EventManager *const m_manager;
};

typedef AutoPtr1<EventHandler> EventHandlerPtr;

class MessageHandler
{
public:
    virtual IoBuffer *handle(IoBuffer *ioBuf) = 0;

    void setEventHandler(EventHandler *eventHandler)
    {
       m_eventHandler = eventHandler;
    }

protected:
    EventHandler *m_eventHandler;
};
#endif   /* ----- #ifndef _EVENTHANDLER_H_  ----- */

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

#include "AutoPtr1.h"
#include "Define.h"
#include "SocketEx.h"

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
    uint16_t cur;
    uint8_t buf[1];
    const static uint16_t size;

    void *operator new(size_t s)
    {
        return (void *)new char [EG_IO_BUF_SIZE];
    }

    void operator delete(void* ptr)
    {
        delete [] (char *)ptr;
    }
};

class EventHandler : public Reference
{
public:
    EventHandler(const EventManager *manager, const int fd);
    EventHandler(const EventManager *manager, SocketPtr &socket);

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
        return m_event &= READ;
    }

    int hasWrite()
    {
        return m_event &= WRITE;
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
    virtual int write() = 0;

protected:    
    int m_event;
    int m_registerEvent;
    SocketPtr m_socket;
    const EventManager *m_manager;
};

typedef AutoPtr1<EventHandler> EventHandlerPtr;
#endif   /* ----- #ifndef _EVENTHANDLER_H_  ----- */

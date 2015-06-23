/**
 *       Filename:  ReceiveHandler.h
 *
 *    Description:  receive handler
 *
 *        Version:  1.0
 *        Created:  05/26/2015 12:55:47 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan Chen, 228268157@qq.com
 *   Organization:  
 */
#ifndef  _RECEIVEHANDLER_H_
#define  _RECEIVEHANDLER_H_

#include "EventHandler.h"

class ReceiveHandler : public EventHandler
{
public:
    ReceiveHandler(EventManager *const manager, const int fd, 
            MessageHandler *handler, Pool<IoBufferUnion> *bufPool);
    ReceiveHandler(EventManager *const manager, Socket *socket, 
            MessageHandler *handler, Pool<IoBufferUnion> *bufPool);
    virtual ~ReceiveHandler();
   
    virtual int read();
    virtual int write();   
    int write(const uint8_t *buf, int size);

private:
    void waitWrite(const uint8_t *buf, int size);

    IoBuffer *getBuf()
    {
        IoBuffer *tmp = (IoBuffer *)m_bufPool->pop();

        tmp->reset();
        return tmp;
    }

    void releaseBuf(IoBuffer *buf)
    {
        return m_bufPool->push((IoBufferUnion *)buf);
    }

    void readLock()
    {
        for(; ;)
        {
            if (__sync_bool_compare_and_swap(
                        const_cast<volatile char *>(&m_readMutex), '\0', '\1')) return;

            sched_yield();
        }
    }

    void readUnlock()
    {
        __sync_bool_compare_and_swap(
                const_cast<volatile char *>(&m_readMutex), '\1', '\0');

    }

    void writeLock()
    {
        for(; ;)
        {
            if (__sync_bool_compare_and_swap(
                        const_cast<volatile char *>(&m_writeMutex), '\0', '\1')) return;

            sched_yield();
        }
    }

    void writeUnlock()
    {
        __sync_bool_compare_and_swap(
                    const_cast<volatile char *>(&m_writeMutex), '\1', '\0');

    }

    char m_readMutex;
    char m_writeMutex;
    IoBuffer *m_readBuf;
    IoBuffer *m_writeBuf;
    MessageHandler *m_messageHandler;
    Pool<IoBufferUnion> *m_bufPool;
};
#endif   /* ----- #ifndef _RECEIVEHANDLER_H_  ----- */

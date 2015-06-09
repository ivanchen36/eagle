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
            MessageHandler *handler);
    ReceiveHandler(EventManager *const manager, Socket *socket, 
            MessageHandler *handler);
    virtual ~ReceiveHandler();
   
    virtual int read();
    virtual int write();   
    int write(const uint8_t *buf, int size);

private:
    void waitWrite(const uint8_t *buf, int size);

    void readLock();
    void readUnlock();
    void writeLock();
    void writeUnlock();

    char m_readMutex;
    char m_writeMutex;
    MessageHandler *m_messageHandler;
};
#endif   /* ----- #ifndef _RECEIVEHANDLER_H_  ----- */

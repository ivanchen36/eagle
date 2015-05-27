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
    ReceiveHandler(EventManager *const manager, const int fd);
    ReceiveHandler(EventManager *const manager, SocketPtr &socket);
    ~ReceiveHandler();
   
    virtual int read();
    virtual int write();   
private:
};
#endif   /* ----- #ifndef _RECEIVEHANDLER_H_  ----- */

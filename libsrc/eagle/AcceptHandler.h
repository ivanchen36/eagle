/**
 *       Filename:  AcceptHandler.h
 *
 *    Description:  accept handler
 *
 *        Version:  1.0
 *        Created:  05/25/2015 11:47:43 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan Chen, 228268157@qq.com
 *   Organization:  
 */
#ifndef  _ACCEPTHANDLER_H_
#define  _ACCEPTHANDLER_H_

#include "EventHandler.h"

class AcceptHandler : public EventHandler
{
public:
    AcceptHandler(EventManager *const manager, Socket *socket, 
            const int port);
    ~AcceptHandler();
   
    virtual int read();
    virtual int write();

private:
    const int m_port;
};
#endif   /* ----- #ifndef _ACCEPTHANDLER_H_  ----- */

/**
 *       Filename:  NodeHandler.h
 *
 *    Description:  node handler
 *
 *        Version:  1.0
 *        Created:  06/15/2015 09:12:31 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan Chen, 228268157@qq.com
 *   Organization:  
 */
#ifndef  _NODEHANDLER_H_
#define  _NODEHANDLER_H_
#include "AutoPtr.h"
#include "EventHandler.h"

class NodeHandler : public MessageHandler
{
#if 0
public:
    NodeHandler();
    ~NodeHandler();
    
    virtual IoBuffer *handle(IoBuffer *ioBuf) = 0;
    virtual void close() = 0;
private:
#endif
};

typedef AutoPtr<NodeHandler> NodeHandlerPtr;
#endif   /* ----- #ifndef _NODEHANDLER_H_  ----- */
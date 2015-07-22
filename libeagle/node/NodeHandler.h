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
#ifndef _EAGLE_NODEHANDLER_H_
#define _EAGLE_NODEHANDLER_H_
#include "AutoPtr.h"
#include "EventHandler.h"

namespace eagle
{

class NodeHandler : public MessageHandler
{
public:
    NodeHandler() {}
    virtual ~NodeHandler() {}

    virtual void run() = 0;
    virtual IoBuffer *handle(IoBuffer *ioBuf) = 0;
};

typedef AutoPtr<NodeHandler> NodeHandlerPtr;

}
#endif   /* ----- #ifndef _EAGLE_NODEHANDLER_H_  ----- */

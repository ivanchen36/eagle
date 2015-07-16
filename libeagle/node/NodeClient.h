/**
 *       Filename:  NodeClient.h
 *
 *    Description:  node client
 *
 *        Version:  1.0
 *        Created:  06/12/2015 08:24:59 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan Chen, 228268157@qq.com
 *   Organization:  
 */
#ifndef  _NODECLIENT_H_
#define  _NODECLIENT_H_

#include "NodeHandler.h"

class NodeClient : public NodeHandler
{
public:
    NodeClient();
    ~NodeClient();

    void run();
    IoBuffer *handle(IoBuffer *ioBuf);
};
#endif   /* ----- #ifndef _NODECLIENT_H_  ----- */

/**
 *       Filename:  NodeServer.h
 *
 *    Description:  Node Server
 *
 *        Version:  1.0
 *        Created:  06/12/2015 08:27:58 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan Chen, 228268157@qq.com
 *   Organization:  
 */
#ifndef  _NODESERVER_H_
#define  _NODESERVER_H_

#include "Partner.h"
#include "NodeHandler.h"

class NodeServer : public NodeHandler
{
public:
    NodeServer();
    virtual ~NodeServer();
    
    void run();
    IoBuffer *handle(IoBuffer *ioBuf);

private:
    Partner *m_partner;
};

#endif   /* ----- #ifndef _NODESERVER_H_  ----- */

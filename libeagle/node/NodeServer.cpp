#include "Thread.h"
#include "NodeServer.h"

namespace
{
void startNodeServer(void *param)
{
    NodeServer *nodeServer = (NodeServer *)param;
    nodeServer->send("");
}
}

REGISTER_REFLECTOR(NODE_SERVER_NAME, NodeServer)

NodeServer::NodeServer()
{
}

NodeServer::~NodeServer()
{
}

void NodeServer::run()
{
    CallBack cb(startNodeServer, this);
    Thread thread(cb);
}

IoBuffer *NodeServer::handle(IoBuffer *ioBuf)
{
    return ioBuf;
}

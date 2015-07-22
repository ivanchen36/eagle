#include "Thread.h"
#include "NodeClient.h"

namespace eagle
{

namespace
{
void startNodeClient(void *param)
{
    NodeClient *nodeclient = (NodeClient *)param;
    nodeclient->send("");
}
}

NodeClient::NodeClient()
{
}

NodeClient::~NodeClient()
{
}

void NodeClient::run()
{
    CallBack cb(startNodeClient, this);
    Thread thread(cb);
}

IoBuffer *NodeClient::handle(IoBuffer *ioBuf)
{
    return ioBuf;
}

}

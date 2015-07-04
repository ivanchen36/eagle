#include "EagleNode.h"
#include "ChildSigManager.h"
#include "SelectManager.h"

namespace
{
void quitEagleNode(void *param)
{
    EventManager *manager = (EventManager *)param;
    manager->stopLoop();
}
}

EagleNode::EagleNode()
{
    m_eventManager = new SelectManager(1); 
    ChildSigManagerI::instance().init(quitEagleNode);
}

EagleNode::~EagleNode()
{
}

void EagleNode::setNodeAddr(const char *ip, const int port)
{
    m_nodeAddr.ip = ip;
    m_nodeAddr.port = port;
}

void EagleNode::addNodeAddr(const char *ip, const int port)
{
    std::vector<NodeAddress>::iterator iter;
    for (iter = m_otherNodeAddr.begin(); iter != m_otherNodeAddr.end(); ++iter) 
    {
        if (iter->ip == ip && iter->port == port)
            return;
    }
    m_otherNodeAddr.push_back(NodeAddress(ip, port));
}

void EagleNode::delNodeAddr(const char *ip, const int port)
{
    std::vector<NodeAddress>::iterator iter;
    for (iter = m_otherNodeAddr.begin(); iter != m_otherNodeAddr.end(); ++iter) 
    {
        if (iter->ip == ip && iter->port == port)
        {
            m_otherNodeAddr.erase(iter);

            return;
        }
    }
}

void EagleNode::addServer(std::map<std::string, int> &serverMap)
{
    m_serverMap.swap(serverMap);
}

void EagleNode::run()
{
#if 0
    childSigManager.init(notifyQuitCb);
#endif
}

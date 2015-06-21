#include "EagleNode.h"

void EagleNode::setNodeAddr(const char *ip, const short port)
{
    m_nodeAddr.ip = ip;
    m_nodeAddr.port = port;
}

void EagleNode::addNodeAddr(const char *ip, const short port)
{
    std::vector<NodeAddress>::iterator iter;
    for (iter = m_otherNodeAddr.begin(); iter != m_otherNodeAddr.end(); ++iter) 
    {
        if (iter->ip == ip && iter->port == port)
            return;
    }
    m_otherNodeAddr.push_back(NodeAddress(ip, port));
}

void EagleNode::delNodeAddr(const char *ip, const short port)
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

void EagleNode::addServer(const char *name, const short port)
{
    m_serverMap[name] = port;
}

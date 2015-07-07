#include "EagleNode.h"
#include "NodeServer.h"
#include "NodeClient.h"
#include "ChildSigManager.h"
#include "SelectManager.h"
#include "PropertiesParser.h"

namespace
{
void quitEagleNode(void *param)
{
    EventManager *manager = (EventManager *)param;
    manager->stopLoop();
}
}

EagleNode::EagleNode() : m_handler(NULL), m_eventManager(new SelectManager(1))
{
    CallBack cb(quitEagleNode, m_eventManager);
    ChildSigManagerI::instance().init(cb);
}

EagleNode::~EagleNode()
{
    if (NULL != m_eventManager) delete m_eventManager;
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
    Socket *socket;

    PropertiesParser::parseNodeProperties();
    socket = new Socket(m_nodeAddr.ip.c_str(), m_nodeAddr.port, 1);
    if (!socket->isAvailable())
    {
        delete socket;
        socket = new Socket(m_nodeAddr.ip.c_str(), m_nodeAddr.port, 0);
        if (socket->isAvailable())
        {
            m_handler = new NodeClient();
        }
    }else
    {
        m_handler = new NodeServer();
    }
    m_eventManager->loop();
}

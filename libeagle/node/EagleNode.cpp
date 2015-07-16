#include "EagleNode.h"
#include "NodeServer.h"
#include "NodeClient.h"
#include "ChildSigManager.h"
#include "SelectManager.h"
#include "AcceptHandler.h"
#include "ReceiveHandler.h"
#include "PropertiesParser.h"

namespace
{
void quitEagleNode(void *param)
{
    EventManager *manager = (EventManager *)param;
    manager->stopLoop();
}
}

EagleNode::EagleNode() : m_eventManager(new SelectManager(1))
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
    m_addr.ip = ip;
    m_addr.port = port;
}

void EagleNode::addNodeAddr(const char *ip, const int port)
{
    std::vector<NodeAddress>::iterator iter;
    for (iter = m_nodeAddr.begin(); iter != m_nodeAddr.end(); ++iter) 
    {
        if (iter->ip == ip && iter->port == port)
            return;
    }
    m_nodeAddr.push_back(NodeAddress(ip, port));
}

void EagleNode::delNodeAddr(const char *ip, const int port)
{
    std::vector<NodeAddress>::iterator iter;
    for (iter = m_nodeAddr.begin(); iter != m_nodeAddr.end(); ++iter) 
    {
        if (iter->ip == ip && iter->port == port)
        {
            m_nodeAddr.erase(iter);

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
    NodeHandler *handler;

    PropertiesParser::parseNodeProperties();
    socket = new Socket(m_addr.ip.c_str(), m_addr.port, 1);
    if (!socket->isAvailable())
    {
        delete socket;
        socket = new Socket(m_addr.ip.c_str(), m_addr.port, 0);
        if (socket->isAvailable())
        {
            handler = new NodeClient();
            m_eventManager->registerEvent(READ, new ReceiveHandler(
            m_eventManager, socket, handler,
                IoBufPoolManagerI::instance().getBufPool()));
        }
    }else
    {
        handler = new NodeServer();
        MessageHandlerFactoryI::instance().registerHandler(
                m_addr.port, NODE_SERVER_NAME);
        m_eventManager->registerEvent(READ, new AcceptHandler(
            m_eventManager, socket, m_addr.port, NODE_SERVER_INDEX));
    }
    if (NULL == handler)
    {
        ERRORLOG2("bind or connect %s:%d err, quit", 
                m_addr.ip.c_str(), m_addr.port);

        return;
    }

    handler->run();
    m_eventManager->loop();
}

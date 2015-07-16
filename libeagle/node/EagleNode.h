/**
 *       Filename:  EagleNode.h
 *
 *    Description:  eagle node
 *
 *        Version:  1.0
 *        Created:  06/13/2015 05:22:20 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan Chen, 228268157@qq.com
 *   Organization:  
 */
#ifndef  _EAGLENODE_H_
#define  _EAGLENODE_H_
#include <string>
#include <vector>
#include <map>

#include "Singleton.h"
#include "EventManager.h"

struct NodeAddress
{
    std::string ip;
    int port;

    NodeAddress() {}

    NodeAddress(const std::string & i, const int p)
    {
        ip = i;
        port = p;
    }

    NodeAddress(const NodeAddress &addr)
    {
        ip = addr.ip;
        port = addr.port;
    }

    NodeAddress& operator=(const NodeAddress &addr)
    {
        ip = addr.ip;
        port = addr.port;

        return *this;
    }

    bool operator==(const NodeAddress &addr)
    {
        if (addr.ip == ip && addr.port == port) return true;

        return false;
    }
};

class EagleNode 
{
public:
    ~EagleNode();

    void setNodeAddr(const char *ip, const int port);
    void addNodeAddr(const char *ip, const int port);
    void delNodeAddr(const char *ip, const int port);
    void addServer(std::map<std::string, int> &serverMap);
    EventHandler *getNodeHanlder(const std::string &node);
    void run();

private:
    EagleNode();

    EventManager *m_eventManager;
    NodeAddress m_addr;
    std::map<std::string, int> m_serverMap;
    std::vector<NodeAddress> m_nodeAddr;
    std::map<std::string, EventHandler *> m_nodeHanlder;

    friend class Singleton<EagleNode>;
};

typedef Singleton<EagleNode> EagleNodeI;
#endif   /* ----- #ifndef _EAGLENODE_H_  ----- */

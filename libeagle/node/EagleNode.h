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
#include "NodeHandler.h"

struct NodeAddress
{
    std::string ip;
    short port;

    NodeAddress() {}

    NodeAddress(const std::string & i, const short p)
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
    ~EagleNode() {}

    void setNodeAddr(const char *ip, const short port);
    void addNodeAddr(const char *ip, const short port);
    void delNodeAddr(const char *ip, const short port);
    void addServer(const char *name, const short port);

private:
    EagleNode() {}

    NodeHandler *m_handler;
    NodeAddress m_nodeAddr;
    std::map<std::string, short> m_serverMap;
    std::vector<NodeAddress> m_otherNodeAddr;

    friend class Singleton<EagleNode>;
};

typedef Singleton<EagleNode> EagleNodeI;
#endif   /* ----- #ifndef _EAGLENODE_H_  ----- */

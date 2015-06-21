/**
 *       Filename:  NodeHandler.h
 *
 *    Description:  node handler
 *
 *        Version:  1.0
 *        Created:  06/15/2015 09:12:31 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan Chen, 228268157@qq.com
 *   Organization:  
 */
#ifndef  _NODEHANDLER_H_
#define  _NODEHANDLER_H_
#include "AutoPtr.h"

class NodeHandler 
{
public:
    NodeHandler();
    ~NodeHandler();
    
private:
};

typedef AutoPtr<NodeHandler> NodeHandlerPtr;
#endif   /* ----- #ifndef _NODEHANDLER_H_  ----- */

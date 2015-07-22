/**
 *       Filename:  MessageHandlerFactory.h
 *
 *    Description:  message handler facetory
 *
 *        Version:  1.0
 *        Created:  05/27/2015 04:59:48 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan Chen, 228268157@qq.com
 *   Organization:  
 */
#ifndef _EAGLE_MESSAGEHANDLERFACTORY_H_
#define _EAGLE_MESSAGEHANDLERFACTORY_H_

#include "ClassFactory.h"
#include "Define.h"

namespace eagle
{

class MessageHandlerFactory 
{
public:
    void registerHandler(const int port, const std::string &name);

    template <class T> 
    int createHandler(const int port, T *&instance)
    {
        std::map<int, createClassFunc>::const_iterator iter;

        iter = m_funcMap.find(port);
        if (m_funcMap.find(port) != m_funcMap.end())
        {
            instance = (T *)(*iter->second)();

            return EG_SUCCESS;
        }

        instance = NULL;

        return EG_FAILED;
    }
    
private:
    MessageHandlerFactory(){};

    std::map<int, createClassFunc> m_funcMap;

    friend class Singleton<MessageHandlerFactory>;
};

typedef Singleton<MessageHandlerFactory> MessageHandlerFactoryI;

}
#endif   /* ----- #ifndef _EAGLE_MESSAGEHANDLERFACTORY_H_  ----- */

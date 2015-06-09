/**
 *       Filename:  ClassFactory.h
 *
 *    Description:  class factory
 *
 *        Version:  1.0
 *        Created:  05/26/2015 11:22:24 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan Chen, 228268157@qq.com
 *   Organization:  
 */
#ifndef  _CLASSFACTORY_H_
#define  _CLASSFACTORY_H_
#include <map>
#include <string>

#include "Define.h"
#include "Singleton.h"

typedef void *(*createClassFunc)();

class ClassFactory 
{
public:
    template <class T> 
    int createClass(const std::string &name, T *&instance)
    {
        std::map<std::string, createClassFunc>::const_iterator iter;

        iter = m_funcMap.find(name);
        if (m_funcMap.find(name) != m_funcMap.end())
        {
            instance = (T *)(*iter->second)();

            return EG_SUCCESS;
        }

        instance = NULL;

        return EG_FAILED;
    }

    void registerClass(const std::string &name, createClassFunc func);
    createClassFunc getCreateFunc(const std::string &name);
    
private:
    ClassFactory(){};

    std::map<std::string, createClassFunc> m_funcMap;

    friend class Singleton<ClassFactory>;
};

typedef Singleton<ClassFactory> ClassFactoryI;

#define REGISTER_REFLECTOR(name, classname) \
void *create##classname() \
{ \
    return new classname(); \
} \
__attribute__((constructor)) void reflector##classname()\
{ \
    ClassFactoryI::instance().registerClass(name, create##classname); \
} \

#endif   /* ----- #ifndef _CLASSFACTORY_H_  ----- */

#include "ClassFactory.h"

void ClassFactory::registerClass(const std::string &name, createClassFunc func)
{
    if (m_funcMap.find(name) != m_funcMap.end()) return;

    m_funcMap[name] = func;
}

createClassFunc ClassFactory::getCreateFunc(const std::string &name)
{
    std::map<std::string, createClassFunc>::const_iterator iter;

    iter = m_funcMap.find(name);
    if (m_funcMap.find(name) == m_funcMap.end()) return NULL;

    return iter->second;
}

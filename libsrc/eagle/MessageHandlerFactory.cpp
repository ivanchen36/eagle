#include "MessageHandlerFactory.h"
#include "ClassFactory.h"

void MessageHandlerFactory::registerHandler(const int port, const std::string &name)
{
    createClassFunc func =  ClassFactoryI::instance().getCreateFunc(name);
    if (NULL == func) return;

    m_funcMap[port] = func;
}

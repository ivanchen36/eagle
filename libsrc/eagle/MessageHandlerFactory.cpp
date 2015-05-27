#include "MessageHandlerFactory.h"
#include "ClassFactory.h"
#include "Log.h"

void MessageHandlerFactory::registerHandler(const int port, const std::string &name)
{
    createClassFunc func =  ClassFactoryI::instance().getCreateFunc(name);
    if (NULL == func)
    {
        ERRORLOG1("can't find create message handler, name %s", name.c_str());

        return;
    }

    m_funcMap[port] = func;
}

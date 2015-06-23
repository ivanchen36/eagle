#include "MessageHandlerFactory.h"
#include "ClassFactory.h"
#include "Log.h"

namespace
{
ClassFactory &classFactory = ClassFactoryI::instance();
}

void MessageHandlerFactory::registerHandler(const int port, const std::string &name)
{
    createClassFunc func =  classFactory.getCreateFunc(name);
    if (NULL == func)
    {
        ERRORLOG1("can't find create message handler, name %s", name.c_str());

        return;
    }

    m_funcMap[port] = func;
}

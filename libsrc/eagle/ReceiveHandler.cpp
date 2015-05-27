#include "ReceiveHandler.h"
#include "EventManager.h"
#include "Log.h"

ReceiveHandler::ReceiveHandler(EventManager *const manager, const int fd)
    :EventHandler(manager, fd)
{
}

ReceiveHandler::ReceiveHandler(EventManager *const manager, SocketPtr &socket)
    :EventHandler(manager, socket)
{
}

ReceiveHandler::~ReceiveHandler()
{
}

int ReceiveHandler::read()
{
    int fd;
    int ret = EG_SUCCESS;
    struct sockaddr_in addr;

    for (; ;)
    {
        ret = m_socket->accept(fd, addr);
        if (EG_SUCCESS != ret) break;
        //m_manager->registerEvent(READ, new EventHandler(m_manager, fd));
    }

    if (ret == EG_FAILED) return EG_FAILED;

    return EG_SUCCESS;
}

int ReceiveHandler::write()
{
    ERRORLOG("accept handler no wrtie");

    return EG_SUCCESS;
}

#include "AcceptHandler.h"
#include "ReceiveHandler.h"
#include "EventManager.h"
#include "MessageHandlerFactory.h"
#include "Log.h"

AcceptHandler::AcceptHandler(EventManager *const manager, Socket *socket,
        const int port) : EventHandler(manager, socket), m_port(port)
{
}

AcceptHandler::~AcceptHandler()
{
}

int AcceptHandler::read()
{
    int fd;
    int ret = EG_SUCCESS;
    struct sockaddr_in addr;
    MessageHandler *message;

    for (; ;)
    {
        ret = m_socket->accept(fd, addr);
        if (EG_SUCCESS != ret) break;
        if (MessageHandlerFactoryI::instance().createHandler(
                    m_port, message) != EG_SUCCESS) continue;

        m_manager->registerEvent(READ, new ReceiveHandler(
                    m_manager, fd, message));
    }

    if (EG_FAILED == ret) return EG_FAILED;

    return EG_SUCCESS;
}

int AcceptHandler::write()
{
    ERRORLOG("accept handler no wrtie");

    return EG_SUCCESS;
}

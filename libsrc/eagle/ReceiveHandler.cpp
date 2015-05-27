#include "ReceiveHandler.h"
#include "EventManager.h"
#include "Log.h"

ReceiveHandler::ReceiveHandler(EventManager *const manager, const int fd, 
        MessageHandler *handler)
    :EventHandler(manager, fd), m_handler(handler)
{
}

ReceiveHandler::ReceiveHandler(EventManager *const manager, SocketPtr &socket,
        MessageHandler *handler) 
    : EventHandler(manager, socket), m_handler(handler)
{
}

ReceiveHandler::~ReceiveHandler()
{
    if (NULL != m_handler) delete m_handler;
}

int ReceiveHandler::read()
{
    return EG_SUCCESS;
}

int ReceiveHandler::write()
{
    return EG_SUCCESS;
}

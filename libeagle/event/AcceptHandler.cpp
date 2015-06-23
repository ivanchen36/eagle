#include "AcceptHandler.h"
#include "ReceiveHandler.h"
#include "EventManager.h"
#include "MessageHandlerFactory.h"
#include "ShareMem.h"
#include "Eagle.h"
#include "Log.h"

namespace
{
ShareMem &shareMem = ShareMemI::instance();
IoBufPoolManager &bufPoolManager = IoBufPoolManagerI::instance();
MessageHandlerFactory &messageHandlerFactory = MessageHandlerFactoryI::instance();
}

AcceptHandler::AcceptHandler(EventManager *const manager, Socket *socket,
        const int port) : EventHandler(manager, socket), m_port(port)
{
    m_lock = (char *)shareMem.alloc(1, port);
    if (NULL == m_lock)
    {
        ERRORLOG("alloc accept lock err");
    }else
    {
        *m_lock = 0;
    }
}

AcceptHandler::~AcceptHandler()
{
    if (NULL != m_lock) shareMem.free(m_lock);
}

int AcceptHandler::read()
{
    if (m_manager->isOverLoad())
        return EG_SUCCESS;

    if (tryLock() == EG_FAILED)
        return EG_SUCCESS;

    int fd;
    int ret = EG_SUCCESS;
    struct sockaddr_in addr;
    MessageHandler *message;

    for (; ;)
    {
        ret = m_socket->accept(fd, addr);
        if (EG_SUCCESS != ret) break;
        if (messageHandlerFactory.createHandler(
                    m_port, message) != EG_SUCCESS) continue;

        m_manager->registerEvent(READ, new ReceiveHandler(
                    m_manager, fd, message, bufPoolManager.getBufPool()));
    }

    unlock();
    if (EG_FAILED == ret) return EG_FAILED;

    return EG_SUCCESS;
}

int AcceptHandler::write()
{
    ERRORLOG("accept handler no wrtie");

    return EG_SUCCESS;
}

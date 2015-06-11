#include "AcceptHandler.h"
#include "ReceiveHandler.h"
#include "EventManager.h"
#include "MessageHandlerFactory.h"
#include "ShareMem.h"
#include "Log.h"

AcceptHandler::AcceptHandler(EventManager *const manager, Socket *socket,
        const int port) : EventHandler(manager, socket), m_port(port)
{
    m_lock = (char *)ShareMemI::instance().alloc(1, port);
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
    if (NULL != m_lock) ShareMemI::instance().free(m_lock);
}

int AcceptHandler::read()
{
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
        if (MessageHandlerFactoryI::instance().createHandler(
                    m_port, message) != EG_SUCCESS) continue;

        m_manager->registerEvent(READ, new ReceiveHandler(
                    m_manager, fd, message));
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

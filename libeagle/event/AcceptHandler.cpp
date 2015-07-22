#include "AcceptHandler.h"
#include "ReceiveHandler.h"
#include "EventManager.h"
#include "ShareMem.h"
#include "Eagle.h"
#include "Log.h"

namespace eagle
{

namespace
{
ShareMem &shareMem = ShareMemI::instance();
}

AcceptHandler::AcceptHandler(EventManager *const manager, Socket *socket,
        const int port, const int index) 
    : EventHandler(manager, socket), m_index(index), m_port(port),
    m_messageHandlerFactory(MessageHandlerFactoryI::instance()), 
    m_bufPoolManager(IoBufPoolManagerI::instance())
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
        if (m_messageHandlerFactory.createHandler(
                    m_port, message) != EG_SUCCESS) continue;

        m_manager->registerEvent(READ, new ReceiveHandler(
                    m_manager, fd, message, m_bufPoolManager.getBufPool()));
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

}

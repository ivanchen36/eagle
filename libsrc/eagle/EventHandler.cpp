#include "EventHandler.h"
#include "EventManager.h"

const uint16_t IoBuffer::size = EG_IO_BUF_SIZE - offsetof(IoBuffer, buf);

EventHandler::EventHandler(EventManager *const manager, const int fd)
    : m_event(NONE), m_registerEvent(NONE), m_socket(new Socket(fd)), 
    m_manager(manager)
{
}

EventHandler::EventHandler(EventManager *const manager, SocketPtr &socket)
    : m_event(NONE), m_registerEvent(NONE), m_socket(socket), 
    m_manager(manager)
{
}

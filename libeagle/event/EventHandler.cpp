#include "EventHandler.h"
#include "EventManager.h"

const uint16_t IoBuffer::size = EG_IO_BUF_SIZE - offsetof(IoBuffer, buf);

EventHandler::EventHandler(EventManager *const manager, const int fd)
    : m_event(NONE), m_registerEvent(NONE), m_socket(new Socket(fd)), 
    m_manager(manager)
{
}

EventHandler::EventHandler(EventManager *const manager, Socket *socket)
    : m_event(NONE), m_registerEvent(NONE), m_socket(socket), 
    m_manager(manager)
{
}

EventHandler::~EventHandler()
{
    if (m_socket) delete m_socket;
}

void EventHandler::clearRegisterEvent()
{
    m_manager->unregisterEvent(m_registerEvent, this);
}

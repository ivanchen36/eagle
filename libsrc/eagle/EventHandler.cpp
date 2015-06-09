#include "EventHandler.h"
#include "EventManager.h"

const uint16_t IoBuffer::size = EG_IO_BUF_SIZE - offsetof(IoBuffer, buf);

EventHandler::EventHandler(EventManager *const manager, const int fd)
    : m_event(NONE), m_registerEvent(NONE), m_socket(new Socket(fd)), 
    m_readBuf(NULL), m_writeBuf(NULL), m_manager(manager)
{
}

EventHandler::EventHandler(EventManager *const manager, Socket *socket)
    : m_event(NONE), m_registerEvent(NONE), m_socket(socket), 
    m_readBuf(NULL), m_writeBuf(NULL), m_manager(manager)
{
}

EventHandler::~EventHandler()
{
    releaseBuf();
    if (m_socket) delete m_socket;
}

void EventHandler::releaseBuf()
{
    IoBuffer *ib;

    for (ib = m_readBuf; NULL != m_readBuf; ib = m_readBuf)
    {
        m_readBuf = m_readBuf->next;
        m_manager->releaseBuf(ib); 
    }

    for (ib = m_writeBuf; NULL != m_writeBuf; ib = m_writeBuf)
    {
        m_writeBuf = m_writeBuf->next;
        m_manager->releaseBuf(ib); 
    }
}

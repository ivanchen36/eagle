#include "ReceiveHandler.h"
#include "EventManager.h"
#include "Log.h"

ReceiveHandler::ReceiveHandler(EventManager *const manager, const int fd, 
        MessageHandler *handler)
    :EventHandler(manager, fd), m_readMutex('\0'), m_writeMutex('\0'),
    m_messageHandler(handler) 

{
    m_messageHandler->setEventHandler(this);
}

ReceiveHandler::ReceiveHandler(EventManager *const manager, Socket *socket,
        MessageHandler *handler) 
    : EventHandler(manager, socket), m_readMutex('\0'), m_writeMutex('\0'),
    m_messageHandler(handler)
{
    m_messageHandler->setEventHandler(this);
}

ReceiveHandler::~ReceiveHandler()
{
    if (NULL != m_messageHandler) delete m_messageHandler;
}

void ReceiveHandler::readLock()
{
    for(; ;)
    {
        if (__sync_bool_compare_and_swap(
                    const_cast<volatile char *>(&m_readMutex), '\0', '\1')) return;

        sched_yield();
    }
}

void ReceiveHandler::readUnlock()
{
    for(; ;)
    {
        if (__sync_bool_compare_and_swap(
                    const_cast<volatile char *>(&m_readMutex), '\1', '\0')) return;

        sched_yield();
    }
}

void ReceiveHandler::writeLock()
{
    for(; ;)
    {
        if (__sync_bool_compare_and_swap(
                    const_cast<volatile char *>(&m_writeMutex), '\0', '\1')) return;

        sched_yield();
    }
}

void ReceiveHandler::writeUnlock()
{
    for(; ;)
    {
        if (__sync_bool_compare_and_swap(
                    const_cast<volatile char *>(&m_writeMutex), '\1', '\0')) return;

        sched_yield();
    }
}

int ReceiveHandler::read()
{
    int ret;
    int recv;
    int readSuccess = 0;
    IoBuffer *ib;
    IoBuffer *tmp;

    readLock();
    if (m_readBuf == NULL)
    {
        m_readBuf = m_manager->getBuf();
        ib = m_readBuf;
        recv = ib->size;
    }else
    {
        ib = m_readBuf;
        while (NULL != ib->next) ib = ib->next;
        recv = ib->size - ib->cur;
    }

    for (; ;)
    {
        ret = m_socket->recv(ib->buf + ib->cur, recv);
        if (EG_FAILED == ret) 
        {
            m_manager->unregisterEvent(m_registerEvent, this);

            readUnlock();
            return EG_FAILED;
        }

        if (EG_AGAIN == ret)
        {
            if (readSuccess) break;

            readUnlock();
            return EG_SUCCESS;
        }

        readSuccess = 1;
        ib->cur += recv;
        if (ib->size == ib->cur)
        {
            tmp = m_manager->getBuf();
            ib->next = tmp;
            ib = tmp;
            recv = ib->size;
        }else
        {
            recv = ib->size - ib->cur;
        }
    }

    ib = m_messageHandler->handle(m_readBuf);
    for (; ib != m_readBuf; )
    {
        tmp = m_readBuf;
        m_readBuf = m_readBuf->next;
        m_manager->releaseBuf(tmp); 
    }
    readUnlock();

    return EG_SUCCESS;
}

int ReceiveHandler::write()
{
    int ret;
    int send;
    IoBuffer *ib;

    if (m_writeBuf == NULL) return EG_SUCCESS;

    writeLock();
    for (ib = m_writeBuf; NULL != m_writeBuf; ib = m_writeBuf)
    {
        for (; ;)
        {
            send = ib->cur - ib->offset;
            ret = m_socket->send(ib->buf + ib->offset, send);
            if (EG_FAILED == ret) 
            {
                m_manager->unregisterEvent(m_registerEvent, this);

                writeUnlock();
                return EG_FAILED;
            }

            if (EG_AGAIN == ret)
            {
                writeUnlock();
                return EG_SUCCESS;
            }

            ib->offset += send;
            if (ib->offset == ib->cur) break;
        }

        m_writeBuf = m_writeBuf->next;
        m_manager->releaseBuf(ib);
    }

    m_manager->unregisterEvent(WRITE, this);
    writeUnlock();

    return EG_SUCCESS;
}

int ReceiveHandler::write(const uint8_t *buf, int size)
{
    int ret;
    int len = 0;
    int send = size;

    for (; len < size;)
    {
        ret = m_socket->send(buf + len, send);
        if (EG_FAILED == ret) 
        {
            m_manager->unregisterEvent(m_registerEvent, this);

            return EG_FAILED;
        }

        if (EG_AGAIN == ret)
        {
            waitWrite(buf + len, size - len);
            if (!(m_registerEvent &= WRITE))
            {
                m_manager->registerEvent(WRITE, this);
            }

            return EG_SUCCESS;
        }

        len += send;
    }

    return EG_SUCCESS;
}

void ReceiveHandler::waitWrite(const uint8_t *buf, int size)
{
    int len;
    IoBuffer *ib;
    IoBuffer *tmp;

    writeLock();
    if (m_writeBuf == NULL)
    {
        m_writeBuf = m_manager->getBuf();
        ib = m_writeBuf;
        len = ib->size;
    }else
    {
        ib = m_writeBuf;
        while (NULL != ib->next) 
        {
            ib = ib->next;
        }
        len = ib->size - ib->cur;
    }

    for (; ;)
    {
        len = len < size ? len : size;
        size = size -len;
        memcpy(ib->buf + ib->cur, buf, len);
        ib->cur += len;
        if (0 == size) break;

        tmp = m_manager->getBuf();
        ib->next = tmp;
        ib = tmp;
        len = ib->size;
    }
    writeUnlock();
}

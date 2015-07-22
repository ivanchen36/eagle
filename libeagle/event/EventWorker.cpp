#include "EventWorker.h"

namespace eagle
{

EventWorker::EventWorker(const int maxEventSize) 
    : Task("event worker"), m_status(FREE), m_queue(maxEventSize)
{
    m_thread = new TaskThread(this);
}

EventWorker::~EventWorker()
{
    if (m_thread) delete m_thread;
}

int EventWorker::excute()
{
    if (handleEvent() == EG_SUCCESS)
        return EG_SUCCESS;

    sched_yield();
    if (handleEvent() == EG_SUCCESS)
        return EG_SUCCESS;

    m_status = FREE;
    sched_yield();
    if (handleEvent() == EG_SUCCESS)
    {
        m_status = NORMAL;
        return EG_SUCCESS;
    }

    return EG_AGAIN;
}

int EventWorker::handleEvent()
{
    if (m_queue.empty())
        return EG_FAILED;

    EventHandler *handler;

    m_queue.pop(handler);
    if (handler->hasRead()) 
    {
        handler->read();
        handler->inactivateRead();
    }
    if (handler->hasWrite())
    {
        handler->write();
        handler->inactivateWrite();
    }
    if (handler->dec() == 0) delete handler;

    return EG_SUCCESS;
}

int EventWorker::addEvent(EventHandler *handler)
{
    m_queue.push(handler);
    handler->inc();

    if (m_queue.full())
    {
        m_status = BUSY;

        return EG_SUCCESS;
    }else if (FREE == m_status)
    {
        m_status = NORMAL;
        m_thread->start();
    }

    return EG_SUCCESS;
}

}

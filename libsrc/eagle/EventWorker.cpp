#include "EventWorker.h"

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
        handler->inactivateRead();
        handler->read();
    }
    if (handler->hasWrite())
    {
        handler->inactivateWrite();
        handler->write();
    }
    if (handler->dec()) delete handler;

    return EG_SUCCESS;
}


int EventWorker::addEvent(EventHandler *handler)
{
    if (m_queue.full())
    {
        m_status = BUSY;

        return EG_FAILED;
    }

    m_queue.push(handler);
    handler->inc();

    if (m_queue.full())
    {
        m_status = BUSY;
    }else if (FREE == m_status)
    {
        m_thread->start();
        m_status = NORMAL;
    }

    return EG_SUCCESS;
}

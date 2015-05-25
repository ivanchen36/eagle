/**
 *       Filename:  SelectManager.h
 *
 *    Description:  select event manager
 *
 *        Version:  1.0
 *        Created:  05/24/2015 06:16:29 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan Chen, 228268157@qq.com
 *   Organization:  
 */
#ifndef  _SELECTMANAGER_H_
#define  _SELECTMANAGER_H_

#include <tr1/unordered_map>

#include "EventManager.h"

class SelectManager : public EventManager
{
public:
    SelectManager(const int workerNum);
    ~SelectManager();

    virtual void loop();
    virtual int registerEvent(int event, EventHandlerPtr &handler);
    virtual int unregisterEvent(int event, EventHandlerPtr &handler);

private:
    typedef std::tr1::unordered_map<int, EventHandler *> EventMap;

    void initFdSet();
    void handleFdSet();
    void flushSelectEvent();

    int m_maxFd;
    fd_set m_readSet;
    fd_set m_writeSet;
    EventMap m_eventMap;
};
#endif   /* ----- #ifndef _SELECTMANAGER_H_  ----- */

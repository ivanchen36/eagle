#include "ChildSigManager.h"
#include "Log.h"

ChildSigManager::ChildSigManager()
{
    m_handleMap[SIGQUIT] = (SaHandle)&ChildSigManager::sigQuit;  
}

ChildSigManager::~ChildSigManager()
{
}

void ChildSigManager::init(NotifyQuitFunc func)
{
    m_notifyQuitFunc = func;
    SignalManager::init();
}

ChildSigManager::Type ChildSigManager::getType()
{
    return CHILD;
}

void ChildSigManager::sigQuit()
{
    DEBUGLOG("child quit");
    (*m_notifyQuitFunc)();
}

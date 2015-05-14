#include "ChildSigManager.h"
#include "Log.h"

ChildSigManager::ChildSigManager()
{
    m_handleMap[SIGQUIT] = (SaHandle)&ChildSigManager::sigQuit;  
}

ChildSigManager::~ChildSigManager()
{
}

void ChildSigManager::init(const CallBack &notifyQuitCb)
{
    m_notifyQuitCb = notifyQuitCb;
    SignalManager::init();
}

ChildSigManager::Type ChildSigManager::getType()
{
    return CHILD;
}

void ChildSigManager::sigQuit()
{
    m_notifyQuitCb.excute();
}

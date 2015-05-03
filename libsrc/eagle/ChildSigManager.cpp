#include "ChildSigManager.h"

ChildSigManager::ChildSigManager()
{
    m_handleMap[SIGQUIT] = (SaHandle)&ChildSigManager::sigQuit;  
}

ChildSigManager::~ChildSigManager()
{
}

void ChildSigManager::init(WaitQuitFunc waitQuit)
{
    m_waitQuitFunc = waitQuit;
    SignalManager::init();
}

ChildSigManager::Type ChildSigManager::getType()
{
    return CHILD;
}

void ChildSigManager::sigQuit()
{
    (*m_waitQuitFunc)();
}

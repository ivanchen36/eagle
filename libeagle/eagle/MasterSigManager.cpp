#include "MasterSigManager.h"
#include "ProcessManager.h"
#include "Log.h"

MasterSigManager::MasterSigManager()
{
    m_handleMap[SIGQUIT] = (SaHandle)&MasterSigManager::sigQuit;  
    m_handleMap[SIGCHLD] = (SaHandle)&MasterSigManager::sigChld;  
}

MasterSigManager::~MasterSigManager()
{

}

MasterSigManager::Type MasterSigManager::getType()
{
    return MASTER;
}

void MasterSigManager::sigQuit()
{
    ProcessManagerI::instance().quit();
}

void MasterSigManager::sigChld()
{
    ProcessManagerI::instance().check();
}

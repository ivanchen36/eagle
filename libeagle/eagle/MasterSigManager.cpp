#include "MasterSigManager.h"
#include "ProcessManager.h"
#include "Log.h"

namespace
{
ProcessManager &processManager = ProcessManagerI::instance();
}

MasterSigManager::MasterSigManager()
{
    m_handleMap[SIGQUIT] = (SaHandle)&MasterSigManager::sigQuit;  
    m_handleMap[SIGTERM] = (SaHandle)&MasterSigManager::sigQuit;  
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
    processManager.quit();
}

void MasterSigManager::sigChld()
{
    processManager.check();
}

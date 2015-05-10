#include "MasterSigManager.h"
#include "ProcessManager.h"
#include "Log.h"

extern int g_isReceiveSigQuit;

MasterSigManager::MasterSigManager()
{
    m_handleMap[SIGQUIT] = (SaHandle)&MasterSigManager::sigQuit;  
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
    DEBUGLOG("master quit");
    ProcessManagerI::instance().quitAll();
    g_isReceiveSigQuit = 1;
}

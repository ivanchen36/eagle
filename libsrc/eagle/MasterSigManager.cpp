#include "MasterSigManager.h"
#include "ProcessManager.h"
#include "Log.h"

extern int g_isQuit;

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
    ProcessManagerI::instance().quitAllChild();
    g_isQuit = 1;
}

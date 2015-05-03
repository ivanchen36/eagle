#include "MasterSigManager.h"

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

}

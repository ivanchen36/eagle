#include "SignalManager.h"
#include "MasterSigManager.h"
#include "ChildSigManager.h"
#include "Log.h"

namespace
{
    void masterSaHandler(int sig)
    {
        MasterSigManagerI::instance().handleSig(sig); 
    }

    void childSaHandler(int sig)
    {
        ChildSigManagerI::instance().handleSig(sig); 
    }
}

void SignalManager::init()
{
    struct sigaction sa;
    void (*saHandler)(int);
    SaHandleMap::const_iterator it;

    saHandler = getType() == MASTER ?
        masterSaHandler : childSaHandler;
    for (it = m_handleMap.begin(); it != m_handleMap.end();)
    {
        bzero(&sa, sizeof(struct sigaction)); 
        sa.sa_handler = saHandler;
        sigemptyset(&sa.sa_mask); 
        if (sigaction(it->first, &sa, NULL) == -1) 
        {
            ERRORLOG2("sigaction %d err, %s", 
                    it->first, strerror(errno));

            continue;
        }
    }
}

void SignalManager::handleSig(const int sig)
{
    SaHandleMap::const_iterator it;
    if ((it = m_handleMap.find(sig)) != m_handleMap.end())
    {
        (*it->second)();

        return;
    }
    ERRORLOG1("can't find signal %d handle", sig);
}

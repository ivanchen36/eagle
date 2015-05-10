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
    void (*sigHandleFunc)(int);
    SaHandleMap::const_iterator it;

    sigHandleFunc = getType() == MASTER ?
        masterSaHandler : childSaHandler;
    for (it = m_handleMap.begin(); it != m_handleMap.end(); ++it)
    {
        if (it->first == SIGALRM) continue; /* only timer use */

        bzero(&sa, sizeof(struct sigaction)); 
        sa.sa_handler = sigHandleFunc;
        sigemptyset(&sa.sa_mask); 
        if (sigaction(it->first, &sa, NULL) == -1) 
        {
            ERRORLOG2("sigaction %d err, %s", 
                    it->first, strerror(errno));
        }
    }
}

void SignalManager::handleSig(const int sig)
{
    SaHandleMap::const_iterator it;
    if ((it = m_handleMap.find(sig)) != m_handleMap.end())
    {
        (this->*(it->second))();

        return;
    }
    ERRORLOG1("can't find signal %d handle", sig);
}

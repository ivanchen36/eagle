#include "SignalManager.h"
#include "MasterSigManager.h"
#include "ChildSigManager.h"
#include "Log.h"

namespace
{

MasterSigManager &masterSigManager = MasterSigManagerI::instance();
ChildSigManager &childSigManager = ChildSigManagerI::instance();

void masterSaHandler(int sig)
{
    masterSigManager.handleSig(sig); 
}

void childSaHandler(int sig)
{
    childSigManager.handleSig(sig); 
}
}

int SignalManager::block()
{
    sigset_t set;

    sigemptyset(&set);
    for (SaHandleMap::const_iterator it = m_handleMap.begin(); 
            it != m_handleMap.end(); ++it)
    {
        sigaddset(&set, it->first);
    }

    if (sigprocmask(SIG_BLOCK, &set, NULL) != 0)
    {
        ERRORLOG1("sigprocmask err, %d", strerror(errno));

        return -1;
    }

    return 0;
}

int SignalManager::unBlock()
{
    sigset_t set;

    sigemptyset(&set);
    for (SaHandleMap::const_iterator it = m_handleMap.begin(); 
            it != m_handleMap.end(); ++it)
    {
        sigaddset(&set, it->first);
    }

    if (sigprocmask(SIG_UNBLOCK, &set, NULL) != 0)
    {
        ERRORLOG1("sigprocmask err, %d", strerror(errno));

        return -1;
    }

    return 0;
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

void SignalManager::clean()
{
    SaHandleMap::const_iterator it;

    for (it = m_handleMap.begin(); it != m_handleMap.end(); ++it)
    {
        signal(it->first, SIG_DFL);
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

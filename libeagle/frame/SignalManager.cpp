#include "Define.h"
#include "SignalManager.h"
#include "MasterSigManager.h"
#include "ChildSigManager.h"
#include "Log.h"

namespace eagle
{

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

int SignalManager::block()
{
    sigset_t set;

    sigemptyset(&set);
    for (;;)
    {
        sigsuspend(&set);
        if (EINTR == errno) return EG_SUCCESS;

        ERRORLOG1("sigsuspend err, %s", strerror(errno));

        return EG_FAILED;
    }

    return EG_SUCCESS;
}

int SignalManager::setBlock()
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

        return EG_FAILED;
    }

    return EG_SUCCESS;
}

int SignalManager::setUnBlock()
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

        return EG_FAILED;
    }

    return EG_SUCCESS;
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
    INFOLOG2("%d receive signal %d", getpid(), sig);

    SaHandleMap::const_iterator it;
    if ((it = m_handleMap.find(sig)) != m_handleMap.end())
    {
        (this->*(it->second))();

        return;
    }
    ERRORLOG1("can't find signal %d handle", sig);
}

}

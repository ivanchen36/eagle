/**
 *       Filename:  SignalManager.h
 *
 *    Description:  signal manager
 *
 *        Version:  1.0
 *        Created:  05/03/2015 08:42:40 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan Chen, 228268157@qq.com
 *   Organization:  
 */
#ifndef  _SIGNALMANAGER_H_
#define  _SIGNALMANAGER_H_
#include <signal.h>
#include <map>

class SignalManager
{
public:
    enum Type
    {
        MASTER,
        CHILD,
        UNKNOWN,
    };

    virtual ~SignalManager(){}

    int block();
    void init();
    void clean();
    int unBlock();
    void handleSig(const int sig);
    virtual Type getType() = 0;

protected:
    typedef void (SignalManager::*SaHandle)();
    typedef std::map<int, SaHandle> SaHandleMap;

    SaHandleMap m_handleMap;
};

#endif   /* ----- #ifndef _SIGNALMANAGER_H_  ----- */

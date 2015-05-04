/**
 *       Filename:  ChildSigManager.h
 *
 *    Description:  child process signal manager
 *
 *        Version:  1.0
 *        Created:  05/03/2015 11:34:11 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan Chen, 228268157@qq.com
 *   Organization:  
 */
#ifndef  _CHILDSIGMANAGER_H_
#define  _CHILDSIGMANAGER_H_
#include "Define.h"
#include "Singleton.h"
#include "SignalManager.h"

class ChildSigManager : public SignalManager
{
public:
    ChildSigManager();
    ~ChildSigManager();

    void init(NotifyQuitFunc func);
    Type getType();
    void sigQuit();

private:
    NotifyQuitFunc m_notifyQuitFunc;
};

typedef Singleton<ChildSigManager> ChildSigManagerI;
#endif   /* ----- #ifndef _CHILDSIGMANAGER_H_  ----- */

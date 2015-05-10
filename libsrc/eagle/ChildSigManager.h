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

#include "CallBack.h"
#include "Singleton.h"
#include "SignalManager.h"

class ChildSigManager : public SignalManager
{
public:
    ChildSigManager();
    ~ChildSigManager();

    void init(const CallBack &notifyQuitCb);
    Type getType();
    void sigQuit();

private:
    CallBack m_notifyQuitCb;
};

typedef Singleton<ChildSigManager> ChildSigManagerI;
#endif   /* ----- #ifndef _CHILDSIGMANAGER_H_  ----- */

/**
 *       Filename:  MasterSigManager.h
 *
 *    Description:  master process signal manager
 *
 *        Version:  1.0
 *        Created:  05/03/2015 11:33:45 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan Chen, 228268157@qq.com
 *   Organization:  
 */
#ifndef  _MASTERSIGMANAGER_H_
#define  _MASTERSIGMANAGER_H_
#include "Singleton.h"
#include "SignalManager.h"

class MasterSigManager : public SignalManager
{
public:
    MasterSigManager();
    ~MasterSigManager();

    Type getType();
    void sigQuit();
};

typedef Singleton<MasterSigManager> MasterSigManagerI;
#endif   /* ----- #ifndef _MASTERSIGMANAGER_H_  ----- */

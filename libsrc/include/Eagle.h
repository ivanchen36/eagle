/**
 *       Filename:  Eagle.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  04/22/2015 08:45:15 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan Chen, 228268157@qq.com
 *   Organization:  
 */
#ifndef  _EAGLE_H_
#define  _EAGLE_H_

#include "CallBack.h"
#include "Singleton.h"
#include "Singleton.h"

#define EAGLE_INIT(cb) if (0 != EagleI::instance().init(cb)) return 0

class Eagle
{
public:
    int init(const CallBack &notifyQuitCb);

private:
    void childInit(const CallBack &notifyQuitCb);
    void masterInit();
    void masterClean();
    int masterCycle();
    int spawnChildProcess();

    friend class Singleton<Eagle>;
};

typedef Singleton<Eagle> EagleI;
#endif   /* ----- #ifndef _EAGLE_H_  ----- */

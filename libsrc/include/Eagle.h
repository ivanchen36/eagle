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

#include "Define.h"
#include "Singleton.h"

class Eagle
{
public:
    void init(NotifyQuitFunc func);
    void destroy();

private:
    Eagle();
    ~Eagle();

    friend class Singleton<Eagle>;
};
typedef Singleton<Eagle> EagleI;

#endif   /* ----- #ifndef _EAGLE_H_  ----- */

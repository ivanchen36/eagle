/**
 *       Filename:  Follower.h
 *
 *    Description:  follower
 *
 *        Version:  1.0
 *        Created:  06/12/2015 08:34:55 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan Chen, 228268157@qq.com
 *   Organization:  
 */
#ifndef _EAGLE_FOLLOWER_H_
#define _EAGLE_FOLLOWER_H_

#include "Partner.h"

namespace eagle
{

class Follower : public Partner
{
public:
    Follower();
    ~Follower();
    
    void campaign();
private:
};

}
#endif   /* ----- #ifndef _EAGLE_FOLLOWER_H_  ----- */

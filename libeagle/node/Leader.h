/**
 *       Filename:  Leader.h
 *
 *    Description:  leader
 *
 *        Version:  1.0
 *        Created:  06/12/2015 08:35:35 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan Chen, 228268157@qq.com
 *   Organization:  
 */
#ifndef  _LEADER_H_
#define  _LEADER_H_

#include "Partner.h"

class Leader : public Partner
{
public:
    Leader();
    ~Leader();
    
private:
    PropertiesManager *m_propertiesManager;
};
#endif   /* ----- #ifndef _LEADER_H_  ----- */

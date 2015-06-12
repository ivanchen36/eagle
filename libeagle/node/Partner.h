/**
 *       Filename:  Partner.h
 *
 *    Description:  partner
 *
 *        Version:  1.0
 *        Created:  06/12/2015 08:29:08 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan Chen, 228268157@qq.com
 *   Organization:  
 */
#ifndef  _PARTNER_H_
#define  _PARTNER_H_

#include "NodeServer.h"

class Partner 
{
public:
    Partner();
    ~Partner();
    
private:
    NodeServer *m_node;
};
#endif   /* ----- #ifndef _PARTNER_H_  ----- */

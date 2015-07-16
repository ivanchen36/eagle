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

class Partner 
{
public:
    Partner();
    ~Partner();

    int commitPropose(uint8_t *buf, const int len);
    void acceptPropose();
};

#endif   /* ----- #ifndef _PARTNER_H_  ----- */

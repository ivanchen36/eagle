/**
 *       Filename:  OsApi.h
 *
 *    Description:  os api wrapper
 *
 *        Version:  1.0
 *        Created:  04/19/2015 01:38:54 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan Chen, 228268157@qq.com
 *   Organization:  
 */

#ifndef  _OSAPI_H_
#define  _OSAPI_H_

class OsApi 
{
public:
    static void atomicAdd(int &var, const int val);
    static void atomicSub(int &var, const int val);
};

#endif   /* ----- #ifndef _OSAPI_H_  ----- */

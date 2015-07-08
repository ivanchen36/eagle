/**
 *       Filename:  CallBack.h
 *
 *    Description:  call back
 *
 *        Version:  1.0
 *        Created:  05/09/2015 02:35:51 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan Chen, 228268157@qq.com
 *   Organization:  
 */
#ifndef  _CALLBACK_H_
#define  _CALLBACK_H_
#include "Log.h"
#include "AutoPtr.h"

typedef void (*CallBackFunc)(void *param);
class CallBack
{
public:
    CallBack(){}

    CallBack(const CallBack &cb) 
        : m_cbParam(cb.m_cbParam), m_cbFunc(cb.m_cbFunc) {}

    CallBack(CallBackFunc func, void *param = NULL) 
        : m_cbParam(param), m_cbFunc(func){}

    void excute()
    {
        (*m_cbFunc)(m_cbParam);
    }

    CallBackFunc getCbFunc()
    {
        return m_cbFunc;
    }

    void *getCbParam()
    {
        return m_cbParam;
    }

private:
    void *m_cbParam;
    CallBackFunc m_cbFunc;
};

typedef AutoPtr<CallBack> CallBackPtr;
#endif   /* ----- #ifndef _CALLBACK_H_  ----- */

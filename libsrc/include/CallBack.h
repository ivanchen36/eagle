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

typedef void (*CallBackFunc)(void *param);
class CallBack
{
public:
    CallBack() : m_cbFunc(0){}
    CallBack(CallBackFunc func, void *param = 0) 
        : m_cbParam(param), m_cbFunc(func){}

    int excute()
    {
        if (0 == m_cbFunc) return -1;
        (*m_cbFunc)(m_cbParam);

        return 0;
    }

private:
    void *m_cbParam;
    CallBackFunc m_cbFunc;
};

#endif   /* ----- #ifndef _CALLBACK_H_  ----- */

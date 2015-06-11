/**
 *       Filename:  AcceptHandler.h
 *
 *    Description:  accept handler
 *
 *        Version:  1.0
 *        Created:  05/25/2015 11:47:43 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan Chen, 228268157@qq.com
 *   Organization:  
 */
#ifndef  _ACCEPTHANDLER_H_
#define  _ACCEPTHANDLER_H_

#include "EventHandler.h"

class AcceptHandler : public EventHandler
{
public:
    AcceptHandler(EventManager *const manager, Socket *socket, 
            const int port);
    virtual ~AcceptHandler();
   
    int tryLock()
    {
        if (__sync_bool_compare_and_swap(
                    const_cast<volatile char *>(m_lock), '\0', '\1')) 
            return EG_SUCCESS;

        return EG_FAILED;
    }

    void unlock()
    {
        __sync_bool_compare_and_swap(
                const_cast<volatile char *>(m_lock), '\1', '\0');

    }

    virtual int read();
    virtual int write();

private:
    char *m_lock;
    const int m_port;
};
#endif   /* ----- #ifndef _ACCEPTHANDLER_H_  ----- */

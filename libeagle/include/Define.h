/**
 *       Filename:  Define.h
 *
 *    Description:  macro and data type define
 *
 *        Version:  1.0
 *        Created:  05/01/2015 09:04:06 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan Chen, 228268157@qq.com
 *   Organization:  
 */
#ifndef  _DEFINE_H_
#define  _DEFINE_H_

#define ALIGN(d, a) ((((d) - 1) / (a) + 1) * (a))
#define MUL_TWO_ALIGN(d, a) (((d) + ((a) - 1)) & ~((a) - 1))

#define MIN_TIMER_INTERVAL 50

#define EG_MAX_EVENTHANDLER 1024
#define EG_EPOLL_MAX_EVENT 512
#define EG_LISTEN_BACKLOG 1024

#ifdef __x86_64__
#define EG_IO_BUF_SIZE 1016
#elif __i386__
#define EG_IO_BUF_SIZE 1020
#endif

#define EG_DEFER_ACCEPT_TIMEOUT 5

#define EG_AGAIN 1
#define EG_SUCCESS 0
#define EG_FAILED -1
#define EG_INVAL -2

#endif   /* ----- #ifndef _DEFINE_H_  ----- */
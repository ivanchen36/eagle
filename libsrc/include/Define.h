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

#endif   /* ----- #ifndef _DEFINE_H_  ----- */

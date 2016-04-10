/**
 *       Filename:  Shell.h
 *
 *    Description:  shell
 *
 *        Version:  1.0
 *        Created:  04/10/2016 06:45:44 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan Chen, 228268157@qq.com
 *   Organization:  
 */
#ifndef _SHELL_H_
#define _SHELL_H_

#include <string>

namespace ealge
{

class Shell 
{
public:
    static int execute(const std::string &cmd, std::string &rs);
};

}
#endif   /* ----- #ifndef _SHELL_H_  ----- */

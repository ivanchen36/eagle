/**
 *       Filename:  StrUtil.h
 *
 *    Description:  string util
 *
 *        Version:  1.0
 *        Created:  06/11/2015 07:11:28 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan Chen, 228268157@qq.com
 *   Organization:  
 */
#ifndef _EAGLE_STRUTIL_H_
#define _EAGLE_STRUTIL_H_

#include <string>
#include <vector>
#include <map>

namespace eagle
{

class StrUtil 
{
public:
    static void copy(char *&dest, const char *src);
    static void copy(char *&dest, const char *src, const int len);
    static void split(const char *sep, const char *str, 
            std::vector<std::string> &vec);
    static void split(const char *sep, char *str, 
            std::vector<char *> &vec);
    static void split(const char *sep1, const char *sep2, 
            const char *str, std::map<std::string, std::string> &map);
    static void split(const char *sep1, const char *sep2, 
            char *str, std::map<char *, char *> &map);
};

}
#endif   /* ----- #ifndef _EAGLE_STRUTIL_H_  ----- */

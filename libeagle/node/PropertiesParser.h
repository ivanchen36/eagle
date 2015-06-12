/**
 *       Filename:  PropertiesParser.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/11/2015 03:43:17 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan Chen, 228268157@qq.com
 *   Organization:  
 */
#ifndef  _PROPERTIESPARSER_H_
#define  _PROPERTIESPARSER_H_

#include "Properties.h"

class PropertiesParser 
{
public:
    static int initProperties(const char *fileName, Properties &properties);
    static void formatToString(Properties &properties, 
            std::string &str); 
    static void formatToProperties(const std::string &str, 
            Properties &properties); 
};
#endif   /* ----- #ifndef _PROPERTIESPARSER_H_  ----- */

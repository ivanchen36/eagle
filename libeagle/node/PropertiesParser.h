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

#include "tinyxml2.h"
#include "Properties.h"

class PropertiesParser 
{
public:
    static int parseNodeProperties();
    static int parseProProperties(std::string &ip, 
            std::map<std::string, int> &serverMap);
    static int parseProperties(Properties &properties);
    static void formatToString(Properties &properties, 
            std::string &str); 
    static void formatToProperties(const std::string &str, 
            Properties &properties); 
private:
    static int handleNodeTag(tinyxml2::XMLElement *root);
    static int handleEagleTag(tinyxml2::XMLElement *root);
    static int handleProgramTag(tinyxml2::XMLElement *root, 
            std::map<std::string, int> &serverMap);
    static int handleServerTag(tinyxml2::XMLElement *root, 
            std::map<std::string, int> &serverMap);
    static int handlePropertyTag(tinyxml2::XMLElement *root, Properties &properties);
};
#endif   /* ----- #ifndef _PROPERTIESPARSER_H_  ----- */

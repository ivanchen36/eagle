/**
 *       Filename:  PropertiesTest.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/12/2015 11:04:37 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan Chen, 228268157@qq.com
 *   Organization:  
 */
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "Log.h"
#include "EagleTime.h"
#include "PropertiesParser.h"

void doTest()
{
    Properties properties;
    std::string str = "1test1=123&2test2=fdsfsd&2test3=sfsd&1test4=1234&2test5=sfsd&1test6=12342&2test7=sfsd&1test8=1234";
    PropertiesParser::formatToProperties(str, properties);
    int test1 = properties.getInt("test1");
    std::string test2 = properties.getString("test2");
    std::string test3 = properties.getString("test3");
    int test4 = properties.getInt("test4");
    DEBUGLOG4("%d %s %s %d", test1, test2.c_str(), test3.c_str(), test4);
    str.clear();
    PropertiesParser::formatToString(properties, str);
    DEBUGLOG1("%s", str.c_str());
}

/**
 * @brief main 
 */
int main ( int argc, char *argv[] )
{
    if (g_sysLog->redirectToOther(STDOUT_FILENO))
    {
        DEBUGLOG("redirectToOther err");
    }
    EagleTimeI::instance().autoUpdate();

    doTest();

    return EXIT_SUCCESS;
}

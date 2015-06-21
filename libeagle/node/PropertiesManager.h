/**
 *       Filename:  PropertiesManager.h
 *
 *    Description:  properties manager
 *
 *        Version:  1.0
 *        Created:  06/15/2015 04:10:53 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan Chen, 228268157@qq.com
 *   Organization:  
 */
#ifndef  _PROPERTIESMANAGER_H_
#define  _PROPERTIESMANAGER_H_

#include <map>
#include <string>

class PropertiesManager
{
public:
    int hasVerProperties(const std::string &program, 
            const std::string &ver)
    {
        if (std::find(m_verList.begin(), m_verList.end(), program + ver) 
                != m_verList.end()) return 1;

        return 0;
    }

private:
    std::vector<std::string> m_verList;
    std::map<std::string, std::map<std::string, int> *> m_intMap;
    std::map<std::string, std::map<std::string, std::string> *> m_stringMap;
};

typedef AutoPtr<PropertiesManager> PropertiesManagerPtr;
#endif   /* ----- #ifndef _PROPERTIESMANAGER_H_  ----- */

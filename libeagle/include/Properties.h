/**
 *       Filename:  Properties.h
 *
 *    Description:  Properties
 *
 *        Version:  1.0
 *        Created:  06/10/2015 07:40:28 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan Chen, 228268157@qq.com
 *   Organization:  
 */
#ifndef  _PROPERTIES_H_
#define  _PROPERTIES_H_
#include <map>
#include <vector>
#include <string>
#include <algorithm>

#include "AutoPtr.h"

class PropertiesParser;

class Properties
{
public:
    std::string &getString(const std::string &name)
    {
        return m_stringMap[name];
    }

    int &getInt(const std::string &name)
    {
        return m_intMap[name];
    }

private:
    void setProperties(const std::string &name, const std::string &val)
    {
        m_stringMap[name] = val;
    }

    void setProperties(const std::string &name, const int val)
    {
        m_intMap[name] = val;
    }

    std::map<std::string, int> m_intMap;
    std::map<std::string, std::string> m_stringMap;

    friend class PropertiesParser;
};

typedef AutoPtr<Properties> PropertiesPtr;

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
#endif   /* ----- #ifndef _PROPERTIES_H_  ----- */

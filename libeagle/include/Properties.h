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
#ifndef _EAGLE_PROPERTIES_H_
#define _EAGLE_PROPERTIES_H_
#include <map>
#include <vector>
#include <string>
#include <algorithm>

#include "AutoPtr.h"

namespace eagle
{

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

    void setProperties(const std::string &name, const std::string &val)
    {
        m_stringMap[name] = val;
    }

    void setProperties(const std::string &name, const int val)
    {
        m_intMap[name] = val;
    }

private:
    std::map<std::string, int> m_intMap;
    std::map<std::string, std::string> m_stringMap;

    friend class PropertiesParser;
};

typedef AutoPtr<Properties> PropertiesPtr;

}
#endif   /* ----- #ifndef _EAGLE_PROPERTIES_H_  ----- */

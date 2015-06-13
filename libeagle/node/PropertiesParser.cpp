#include "PropertiesParser.h"
#include "StrUtil.h"
#include "Define.h"
#include "Log.h"
#include "ShmAllocator.h"
#include "tinyxml2.h"

namespace
{
    const char *TAG_EAGLE = "eagle";
#if 0
    const char *TAG_BROTHER = "brother";
    const char *TAG_INCLUDE = "include";
    const char *TAG_PROGRAM = "eagle";
    const char *TAG_PROPERTY = "eagle";
    const char *TAG_BIND = "bind";
    const char *TAG_CONNECT = "connect";

    const char *ATTR_IP = "ip";
    const char *ATTR_PORT = "port";
    const char *ATTR_FILE = "file";
    const char *ATTR_NAME = "name";
    const char *ATTR_VER = "ver";
    const char *ATTR_PROCESS = "process";
    const char *ATTR_SERVER = "server";
    const char *ATTR_TYPE = "type";
    const char *ATTR_VALUE = "value";

    const char *VAL_AUTO = "auto";
    const char *VAL_STRING = "string";
    const char *VAL_INT = "int";
#endif

    const char TYPE_INT = '1';
    const char TYPE_STRING = '2';
}

int PropertiesParser::initProperties(const char *fileName, Properties &properties)
{
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLElement *root;

    doc.LoadFile(fileName);
    root = doc.RootElement();
    if (NULL == root)
    {
        ERRORLOG1("parse file %s failed.", fileName);

        return EG_FAILED;
    }

    if (root->Name() != TAG_EAGLE)
    {
        ERRORLOG("not found eagle tag.");

        return EG_FAILED;
    }

    return EG_SUCCESS;
}

void PropertiesParser::formatToString(Properties &properties, 
        std::string &str)
{
    char buf[16];
    std::map<std::string, int>::const_iterator iter1
        = properties.m_intMap.begin();
    std::map<std::string, int>::const_iterator end1 
        = properties.m_intMap.end();
    std::map<std::string, std::string>::const_iterator iter2
        = properties.m_stringMap.begin();
    std::map<std::string, std::string>::const_iterator 
        end2 = properties.m_stringMap.end();

    str.reserve(512);
    if (iter1 != end1)
    {
        str.append(1, TYPE_INT);
        str.append(iter1->first);
        str.append(1, '=');
        snprintf(buf, sizeof(buf), "%d", iter1->second);
        str.append(buf);
        ++iter1;
    }

    for (; iter1 != end1; ++iter1)
    {
        str.append(1, '&');
        str.append(1, TYPE_INT);
        str.append(iter1->first);
        str.append(1, '=');
        snprintf(buf, sizeof(buf), "%d", iter1->second);
        str.append(buf);
    }

    if (str.empty() && iter2 != end2)
    {
        str.append(1, TYPE_STRING);
        str.append(iter2->first);
        str.append(1, '=');
        str.append(iter2->second);
        ++iter2;
    }

    for (; iter2 != end2; ++iter2)
    {
        str.append(1, '&');
        str.append(1, TYPE_STRING);
        str.append(iter2->first);
        str.append(1, '=');
        str.append(iter2->second);
    }
}

void PropertiesParser::formatToProperties(const std::string &str, 
        Properties &properties)
{
    std::map<char *, char *> varMap;
    std::map<char *, char *>::iterator iter;
    std::map<char *, char *>::iterator end;
    char *name;
    char *val;
    char *buf = const_cast<char *>(str.c_str());

    StrUtil::split("&", "=", buf, varMap);
    for (iter = varMap.begin(), end = varMap.end(); iter != end; ++iter)
    {
        name = iter->first;
        val = iter->second;
        if (*name == TYPE_INT)
        {
            properties.setProperties(name + 1, atoi(val));
        }else
        {
            properties.setProperties(name + 1, val);
        }
    }
}

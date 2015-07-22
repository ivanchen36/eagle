#include "PropertiesParser.h"
#include "StrUtil.h"
#include "Define.h"
#include "Log.h"
#include "ShmAllocator.h"
#include "Eagle.h"
#include "EagleNode.h"

namespace eagle
{

namespace
{
const char *TAG_EAGLE = "eagle";
const char *TAG_NODE = "node";
const char *TAG_PROGRAM = "program";
const char *TAG_PROPERTY = "property";
const char *TAG_SERVER = "server";

const char *ATTR_IP = "ip";
const char *ATTR_PORT = "port";
const char *ATTR_NAME = "name";
const char *ATTR_VER = "ver";
const char *ATTR_TYPE = "type";
const char *ATTR_VALUE = "value";
const char *ATTR_PROCESS = "process";
const char *ATTR_LOG = "log";

const char *VAL_AUTO = "auto";
const char *VAL_STRING = "string";
const char *VAL_INT = "int";
const char *VAL_DEBUG = "debug";
const char *VAL_INFO = "info";
const char *VAL_WARN = "warn";
const char *VAL_ERROR = "error";

const char TYPE_INT = '1';
const char TYPE_STRING = '2';

Eagle &eagle = EagleI::instance();
Program &program = eagle.getProgram();
}

int PropertiesParser::handleEagleTag(tinyxml2::XMLElement *root)
{
    const char *ip;
    int port;

    ip = root->Attribute(ATTR_IP);
    if (NULL == ip)
    {
        ERRORLOG("not found attribute ip.");

        return EG_FAILED;
    }

    if (root->QueryIntAttribute(ATTR_PORT, &port) != tinyxml2::XML_NO_ERROR)
    {
        ERRORLOG("not found attribute port.");

        return EG_FAILED;
    }

    EagleNodeI::instance().setNodeAddr(ip, port);

    return EG_SUCCESS;
}

int PropertiesParser::handleNodeTag(tinyxml2::XMLElement *root)
{
    const char *ip;
    int port;

    ip = root->Attribute(ATTR_IP);
    if (NULL == ip)
    {
        ERRORLOG("not found attribute ip.");

        return EG_FAILED;
    }

    if (root->QueryIntAttribute(ATTR_PORT, &port) != tinyxml2::XML_NO_ERROR)
    {
        ERRORLOG("not found attribute port.");

        return EG_FAILED;
    }

    EagleNodeI::instance().addNodeAddr(ip, port);

    return EG_SUCCESS;
}

int PropertiesParser::handleProgramTag(tinyxml2::XMLElement *root, 
        std::map<std::string, int> &serverMap)
{
    int process;
    tinyxml2::XMLElement *tmp;
    const char *name = root->Attribute(ATTR_NAME);
    const char *ver = root->Attribute(ATTR_VER);
    const char *log = root->Attribute(ATTR_LOG);

    if (NULL == name || NULL == ver || 
            root->QueryIntAttribute(ATTR_PROCESS, &process) != tinyxml2::XML_NO_ERROR)
    {
        ERRORLOG("not found attribute name、process or ver.");

        return EG_FAILED;
    } 

    if (strcmp(name, program.name) != 0
            || strcmp(ver,program.ver) != 0)
        return EG_AGAIN;

    if (NULL == log || strcmp(VAL_DEBUG, log) == 0)
    {
        program.logLevel = DEBUG_LOG;
    }else if (strcmp(VAL_INFO, log) == 0)
    {
        program.logLevel = INFO_LOG;
    }else if (strcmp(VAL_WARN, log) == 0)
    {
        program.logLevel = WARN_LOG;
    }else if (strcmp(VAL_ERROR, log) == 0)
    {
        program.logLevel = ERROR_LOG;
    }
    program.processNum = process;

    for (tmp = root->FirstChildElement(TAG_SERVER); NULL != tmp; 
            tmp = tmp->NextSiblingElement(TAG_SERVER))
    {
        handleServerTag(tmp, serverMap);
    }

    return EG_SUCCESS;
}

int PropertiesParser::handleServerTag(tinyxml2::XMLElement *root, 
        std::map<std::string, int> &serverMap)
{
    int num;
    const char *name = root->Attribute(ATTR_NAME);
    const char *port = root->Attribute(ATTR_PORT);

    if (NULL == name || NULL == port)
    {
        ERRORLOG("not found attribute name or ver.");

        return EG_FAILED;
    }

    if (strcmp(port, VAL_AUTO) == 0)
    {
        serverMap[name] = 0;

        return EG_SUCCESS;
    }

    num = atoi(port);
    if (0 == num)
    {
        ERRORLOG1("invalid port %s.", port);

        return EG_FAILED;
    }

    serverMap[name] = num;

    return EG_SUCCESS;
}

int PropertiesParser::handlePropertyTag(tinyxml2::XMLElement *root, Properties &properties)
{
    const char *name = root->Attribute(ATTR_NAME);
    const char *type = root->Attribute(ATTR_TYPE);
    const char *val = root->Attribute(ATTR_VALUE);

    if (NULL == name || NULL == type || NULL == val)
    {
        ERRORLOG("not found attribute name、type or value.");

        return EG_FAILED;
    }

    if (strcmp(type, VAL_INT) == 0)
    {
        properties.setProperties(name, atoi(val));
    }else if (strcmp(type, VAL_STRING) == 0)
    {
        properties.setProperties(name, val);
    }else
    {
        ERRORLOG("invalid property type.");

        return EG_FAILED;
    }
 
    return EG_SUCCESS;
}

int PropertiesParser::parseProProperties(std::string &ip, 
        std::map<std::string, int> &serverMap)
{
    int ret = EG_FAILED;
    const char *attr;
    char buf[MAX_FILENAME_LEN];
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLElement *root;
    tinyxml2::XMLElement *tmp;

    snprintf(buf, MAX_FILENAME_LEN, "%s/%s/%s.xml", 
            program.prefix, CONF_DIR, ENTRY_CONF_FILE);
    doc.LoadFile(buf);

    root = doc.FirstChildElement(TAG_EAGLE);
    if (NULL == root)
    {
        ERRORLOG1("parse file %s failed, not find eagle tag.", buf);

        return EG_FAILED;
    }

    attr = root->Attribute(ATTR_IP);
    if (NULL == attr)
    {
        ERRORLOG("not found attribute ip.");

        return EG_FAILED;
    }

    ip = attr;
    for (tmp = root->FirstChildElement(TAG_PROGRAM); NULL != tmp; 
            tmp = tmp->NextSiblingElement(TAG_PROGRAM))
    {
        if (handleProgramTag(tmp, serverMap) == EG_SUCCESS)
        {
            ret = EG_SUCCESS;
        }
    }

    if (EG_FAILED == ret)
    {
        ERRORLOG("parse program config failed.");
    }

    return ret;
}

int PropertiesParser::parseNodeProperties()
{
    char buf[MAX_FILENAME_LEN];
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLElement *root;
    tinyxml2::XMLElement *tmp;

    snprintf(buf, MAX_FILENAME_LEN, "%s/%s/%s.xml", 
            program.prefix, CONF_DIR, ENTRY_CONF_FILE);
    doc.LoadFile(buf);

    root = doc.FirstChildElement(TAG_EAGLE);
    if (NULL == root)
    {
        ERRORLOG1("parse file %s failed, not find eagle tag.", buf);

        return EG_FAILED;
    }

    if (handleEagleTag(root) != EG_SUCCESS) return EG_FAILED;

    for (tmp = root->FirstChildElement(TAG_NODE); NULL != tmp; 
            tmp = tmp->NextSiblingElement(TAG_NODE))
    {
        handleNodeTag(tmp);
    }

    return EG_SUCCESS;
}

int PropertiesParser::parseProperties(Properties &properties)
{
    const char *attr;
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLElement *root;
    tinyxml2::XMLElement *tmp;
    char buf[MAX_FILENAME_LEN];
    const char *programName = program.name;
    const char *programVer = program.ver;

    snprintf(buf, MAX_FILENAME_LEN, "%s/%s/%s.xml", program.prefix, 
            CONF_DIR, programName);
    if (!File::isExist(buf))
    {
        INFOLOG1("proProperties file %s isn't exist.", buf);

        return EG_SUCCESS;
    }

    doc.LoadFile(buf);
    root = doc.FirstChildElement(programName);
    if (NULL == root)
    {
        ERRORLOG2("parse file %s failed, not find %s tag.", buf,
                programName);

        return EG_FAILED;
    }

    for (; NULL != root; root = root->NextSiblingElement(programName))
    {
        attr = root->Attribute(ATTR_VER);
        if (NULL == attr || strcmp(attr, programVer) != 0) continue;

        for (tmp = root->FirstChildElement(TAG_PROPERTY); NULL != tmp; 
                tmp = tmp->NextSiblingElement(TAG_PROPERTY))
        {
            handlePropertyTag(tmp, properties);
        }

        return EG_SUCCESS;
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

}

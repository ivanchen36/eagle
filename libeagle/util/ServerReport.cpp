#include "ServerReport.h"
#include "StrUtil.h"
#include "Shell.h"
#include "EagleTime.h"

namespace
{
const char *LEVEL3_NODE = "%s.%s.%s";
const char *LEVEL6_SS_NODE = "%s.%s.%s.%s.%s.%s";
const char *LEVEL6_SI_NODE = "%s.%s.%s.%s.%s.%d";
const char *LEVEL6_II_NODE = "%s.%s.%s.%d.%s.%d";
const char *LEVEL6_IS_NODE = "%s.%s.%s.%d.%s.%s";

const char *SEP = ".";
const char *NAME = "name";
const char *STATS = "stats";
const char *HOUR = "hour";
const char *SUCCESS = "success";
const char *DATE = "date";
const char *FAIL = "fail";
const char *ERRCODE = "errcode";
const char *KEY = "key";
}

namespace eagle
{

void ServerReport::Stats::loadXmlFile()
{
    char buf[512];
    std::string prefix;
    tinyxml2::XMLElement *tmp;

    prefix.reserve(128);
    sprintf(buf, "%s/%s-%04d%02d%02d.xml", m_savePath.c_str(), 
            m_serverName.c_str(), m_tm.tm_year + 1900, 
            m_tm.tm_mon + 1, m_tm.tm_mday);
    if (access(buf, F_OK) != 0) return;

    m_docPtr->LoadFile(buf);
    tmp = m_docPtr->FirstChildElement(m_serverName.c_str());
    if (NULL == tmp) return;

    for (tmp = tmp->FirstChildElement();
            tmp != NULL; tmp = tmp->NextSiblingElement())
    {
        handleXmlTag(prefix, tmp);
    }
}

void ServerReport::Stats::handleXmlTag(std::string &prefix,
        tinyxml2::XMLElement *el)
{
    if (!strcmp(STATS, el->Name())
            && m_tm.tm_hour != atoi(el->Attribute(HOUR))) 
        return;

    const char *text;
    int len = prefix.length();

    prefix.append(el->Name());
    if (0 == strcmp(ERRCODE, el->Name()) || 0 == strcmp(KEY, el->Name()))
    {
        prefix.append(SEP);
        prefix.append(el->Attribute(NAME));
    }
    text = el->GetText();
    if (NULL != text)
    {
        m_intMap[prefix] = atoi(text);
    }else
    {
        prefix.append(SEP);
        for (el = el->FirstChildElement();
                el != NULL; el = el->NextSiblingElement())
        {
            handleXmlTag(prefix, el);
        }
    }
    prefix.resize(len);
}

void ServerReport::Stats::saveXmlTag(tinyxml2::XMLElement *el, 
        std::string key, int val)
{
    int len;
    char *str;
    std::vector<char *> list;
    tinyxml2::XMLElement *tmp;

    StrUtil::split(SEP, (char *)&key.at(0), list);
    len = list.size();
    for (int i = 0; i < len; ++i)
    {
        tmp = el;
        str = list[i];
        el = tmp->FirstChildElement(str);
        if ((0 == strcmp(ERRCODE, str) || 0 == strcmp(KEY, str)) && len > i + 1) ++i;
        while (el != NULL)
        {
            if (0 == strcmp(STATS, str))
            {
                if (m_tm.tm_hour == atoi(el->Attribute(HOUR))) break;

                el = el->NextSiblingElement();

                continue;
            }

            if (0 == strcmp(ERRCODE, str) || 0 == strcmp(KEY, str))
            {
                if (0 == strcmp(list[i], el->Attribute(NAME))) break;

                el = el->NextSiblingElement();

                continue;
            }

            break;
        }

        if (NULL != el) continue;

        el = m_docPtr->NewElement(str);
        if (0 == strcmp(SUCCESS, str) || 0 == strcmp(FAIL, str))
        {
            tmp->InsertFirstChild(el);
        }else
        {
            tmp->InsertEndChild(el);
        }
        if (0 == strcmp(STATS, str))
        {
            el->SetAttribute(HOUR, m_tm.tm_hour);
            continue;
        }

        if (0 == strcmp(ERRCODE, str) || 0 == strcmp(KEY, str))
        {
            el->SetAttribute(NAME, list[i]);
        }
    }
    el->SetText(val);
}

void ServerReport::Stats::saveXmlFile()
{
    char buf[512];
    tinyxml2::XMLElement *tmp;
    std::map<std::string, int>::iterator iter;

    sprintf(buf, "%04d-%02d-%02d", m_tm.tm_year + 1900, 
            m_tm.tm_mon + 1, m_tm.tm_mday);
    tmp = m_docPtr->FirstChildElement(m_serverName.c_str());
    if (NULL != tmp && 0 != strcmp(tmp->Attribute(DATE), buf))
    {
        m_docPtr = new tinyxml2::XMLDocument();
        tmp = NULL;
    }
    if (NULL == tmp)
    {
        tmp = m_docPtr->NewElement(m_serverName.c_str());
        tmp->SetAttribute(DATE, buf);
        m_docPtr->InsertEndChild(tmp);
    }

    for (iter = m_intMap.begin(); iter != m_intMap.end(); 
            ++iter)
    {
        saveXmlTag(tmp, iter->first, iter->second);
    }

    sprintf(buf, "%s/%s-%04d%02d%02d.xml", m_savePath.c_str(), 
            m_serverName.c_str(), m_tm.tm_year + 1900, 
            m_tm.tm_mon + 1, m_tm.tm_mday);
    m_docPtr->SaveFile(buf);
}

int ServerReport::createSavePathDir()
{
    char buf[256];
    std::string rs;

    snprintf(buf, sizeof(buf), "mkdir -p %s 2>&1", m_savePath.c_str());
    Shell::execute(buf, rs);
    if (rs.empty()) return 0;

    return -1;
}

int ServerReport::init(const std::string &reportPath, const std::string &serverName)
{
    static eagle::EagleTime &eagleTime = eagle::EagleTimeI::instance();

    int t = eagleTime.getSec();
    int tmp = reportPath.length() -1;

    m_tm = eagleTime.getTm();
    m_nextHour = ((t + SECONDS_PER_EIGHT_HOUR - 1) / SECONDS_PER_HOUR + 1
            ) * SECONDS_PER_HOUR - SECONDS_PER_EIGHT_HOUR;
    m_nextFiveMinute = ((t + SECONDS_PER_EIGHT_HOUR - 1) / SECONDS_PER_FIVE_MINUITE + 1
            ) * SECONDS_PER_FIVE_MINUITE - SECONDS_PER_EIGHT_HOUR;
    m_savePath = reportPath;
    m_serverName = serverName;
    if ('/' == m_savePath.at(tmp))
    {
        m_savePath.resize(tmp);
    }

    if (access(m_savePath.c_str(), F_OK) != 0 && createSavePathDir()) return -1;

    m_doc = new tinyxml2::XMLDocument();
    m_hourStats = new Stats(m_doc, m_tm, m_savePath, m_serverName);
    m_hourStats->loadXmlFile();

    return 0;
}

void ServerReport::destroy()
{
    m_hourStats = NULL;
}

void ServerReport::checkAndSaveFile()
{
    static eagle::EagleTime &eagleTime = eagle::EagleTimeI::instance();

    int tmp = eagleTime.getSec();

    if (tmp < m_nextFiveMinute) return;

    if (m_lock.tryLock() != EG_SUCCESS) return;

    if (tmp < m_nextFiveMinute) 
    {
        m_lock.unLock();

        return;
    }

    m_nextFiveMinute = ((tmp + SECONDS_PER_EIGHT_HOUR - 1) / SECONDS_PER_FIVE_MINUITE + 1
            ) * SECONDS_PER_FIVE_MINUITE - SECONDS_PER_EIGHT_HOUR;
    if (tmp > m_nextHour)
    {
        m_tm = eagleTime.getTm();
        m_nextHour = ((tmp + SECONDS_PER_EIGHT_HOUR - 1) / SECONDS_PER_HOUR + 1
                ) * SECONDS_PER_HOUR - SECONDS_PER_EIGHT_HOUR;
        m_hourStats = new Stats(m_doc, m_tm, m_savePath, m_serverName);
    }else
    {
        m_hourStats->saveXmlFile();
    }

    m_lock.unLock();
}

void ServerReport::sendSuccReq(const std::string &reqName)
{
    char buf[128];

    checkAndSaveFile();

    StatsPtr stats = m_hourStats;
    std::map<std::string, int> &intMap = stats->getMap();
    sprintf(buf, LEVEL3_NODE, reqName.c_str(), STATS, SUCCESS);
    std::string str(buf);
    int &val = intMap[str];
    __sync_add_and_fetch(const_cast<volatile int *>(&val), 1);
}

void ServerReport::sendFailReq(const std::string &reqName, const std::string &errCode, 
        const std::string &errKey)
{
    char buf[512];

    checkAndSaveFile();

    StatsPtr stats = m_hourStats;
    std::map<std::string, int> &intMap = stats->getMap();
    sprintf(buf, LEVEL3_NODE, reqName.c_str(), STATS, FAIL);
    int &val1 = intMap[buf];
    __sync_add_and_fetch(const_cast<volatile int *>(&val1), 1);

    snprintf(buf, 128, LEVEL6_SS_NODE, reqName.c_str(), STATS, ERRCODE, 
            errCode.c_str(), KEY, errKey.c_str());
    int &val2 = intMap[buf];
    __sync_add_and_fetch(const_cast<volatile int *>(&val2), 1);
}

void ServerReport::sendFailReq(const std::string &reqName, const std::string &errCode, 
        const int errKey)
{
    char buf[512];

    checkAndSaveFile();

    StatsPtr stats = m_hourStats;
    std::map<std::string, int> &intMap = stats->getMap();
    sprintf(buf, LEVEL3_NODE, reqName.c_str(), STATS, FAIL);
    int &val1 = intMap[buf];
    __sync_add_and_fetch(const_cast<volatile int *>(&val1), 1);

    snprintf(buf, 128, LEVEL6_SI_NODE, reqName.c_str(), STATS, ERRCODE, 
            errCode.c_str(), KEY, errKey);
    int &val2 = intMap[buf];
    __sync_add_and_fetch(const_cast<volatile int *>(&val2), 1);
}

void ServerReport::sendFailReq(const std::string &reqName, const int errCode, 
        const int errKey)
{
    char buf[512];

    checkAndSaveFile();

    StatsPtr stats = m_hourStats;
    std::map<std::string, int> &intMap = stats->getMap();
    sprintf(buf, LEVEL3_NODE, reqName.c_str(), STATS, FAIL);
    int &val1 = intMap[buf];
    __sync_add_and_fetch(const_cast<volatile int *>(&val1), 1);

    snprintf(buf, 128, LEVEL6_II_NODE, reqName.c_str(), STATS, ERRCODE, 
            errCode, KEY, errKey);
    int &val2 = intMap[buf];
    __sync_add_and_fetch(const_cast<volatile int *>(&val2), 1);
}

void ServerReport::sendFailReq(const std::string &reqName, const int errCode, 
        const std::string &errKey)
{
    char buf[512];
    checkAndSaveFile();

    StatsPtr stats = m_hourStats;
    std::map<std::string, int> &intMap = stats->getMap();
    sprintf(buf, LEVEL3_NODE, reqName.c_str(), STATS, FAIL);
    int &val1 = intMap[buf];
    __sync_add_and_fetch(const_cast<volatile int *>(&val1), 1);

    snprintf(buf, 128, LEVEL6_IS_NODE, reqName.c_str(), STATS, ERRCODE, 
            errCode, KEY, errKey.c_str());
    int &val2 = intMap[buf];
    __sync_add_and_fetch(const_cast<volatile int *>(&val2), 1);
}

}

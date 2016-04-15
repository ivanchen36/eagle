/**
 *       Filename:  ServerReport.h
 *
 *    Description:  server report
 *
 *        Version:  1.0
 *        Created:  04/09/2016 11:30:09 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan Chen, 228268157@qq.com
 *   Organization:  
 */
#ifndef _SERVERREPORT_H_
#define _SERVERREPORT_H_

#include <string>
#include <map>
#include <vector>

#include "Singleton.h"
#include "tinyxml2.h"

namespace eagle
{

#define SECONDS_PER_FIVE_MINUITE 300
#define SECONDS_PER_HOUR 3600
#define SECONDS_PER_DAY 86400
#define SECONDS_PER_EIGHT_HOUR 28800

class ServerReport 
{
public:
    ~ServerReport()
    {
        destroy();
    }

    int init(const std::string &reportPath, const std::string &serverName);
    void destroy();
    void sendSuccReq(const std::string &reqName);
    void sendFailReq(const std::string &reqName, const std::string &errCode, 
            const std::string &errKey);
    void sendFailReq(const std::string &reqName, const std::string &errCode, 
            const int errKey);
    void sendFailReq(const std::string &reqName, const int errCode, 
            const int errKey);
    void sendFailReq(const std::string &reqName, const int errCode, 
            const std::string &errKey);
        
private:
    typedef AutoPtr<tinyxml2::XMLDocument> XMLDocumentPtr;

    class Stats
    {
    public:
        Stats(XMLDocumentPtr &doc, struct tm &t,
                std::string &savePath, std::string &serverName)
            : m_tm(t), m_savePath(savePath), 
            m_serverName(serverName), m_docPtr(doc)
        {
            loadXmlFile();
        }

        ~Stats()
        {
            saveXmlFile();
        }
        void loadXmlFile();
        void saveXmlFile();

        std::map<std::string, int> &getMap()
        {
            return m_intMap;
        }
        
    private:
        void handleXmlTag(std::string &prefix,
                tinyxml2::XMLElement *el);
        void saveXmlTag(tinyxml2::XMLElement *el, 
                const std::string &key, int val);

        const struct tm m_tm;
        const std::string &m_savePath;
        const std::string &m_serverName;
        std::map<std::string, int> m_intMap;
        XMLDocumentPtr m_docPtr;
    };
    typedef AutoPtr<Stats> StatsPtr;

    ServerReport() :  m_mutex('\0')
    {
    } 

    int createSavePathDir();
    void checkAndSaveFile();

    char m_mutex;
    int m_nextHour;
    int m_nextFiveMinute;
    struct tm m_tm;
    XMLDocumentPtr m_doc;
    std::string m_savePath;
    std::string m_serverName;
    StatsPtr m_hourStats;

   friend class Singleton<ServerReport>;
};

typedef Singleton<ServerReport> ServerReportI;

}
#endif   /* ----- #ifndef _SERVERREPORT_H_  ----- */

/**
 *       Filename:  Eagle.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  04/22/2015 08:45:15 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan Chen, 228268157@qq.com
 *   Organization:  
 */
#ifndef _EAGLE_EAGLE_H_
#define _EAGLE_EAGLE_H_
#include <string>

#include "CallBack.h"
#include "ShareMem.h"
#include "Singleton.h"
#include "Timer.h"
#include "StrUtil.h"
#include "Properties.h"
#include "EventHandler.h"
#include "EventManager.h"

namespace eagle
{

#define EAGLE_MAIN(ver) \
int main ( int argc, char *argv[] ) \
{ \
    TimerI::del(); \
    ServerTimeI::del(); \
    ShareMemI::del(); \
    if (fork() > 0) return 0;\
    ServerTimeI::instance().autoUpdate(); \
    if (EG_SUCCESS != EagleI::instance().init(argc, argv, ver)) \
    return 0; \
 \
    EagleI::instance().runWorker(); \
    EagleI::instance().destroy(); \
    INFOLOG1("process %d quit", getpid()); \
    return 0; \
}

struct Program
{
    char *name;
    char *ver;
    char *prefix;
    int logLevel;
    int processNum;

    Program()
    {
        name = NULL;
        ver = NULL;
        prefix = NULL;
        logLevel = DEBUG_LOG;
        processNum = 0;
    }

    ~Program()
    {
        if (NULL != name) delete []name;
        if (NULL != ver) delete []ver;
        if (NULL != prefix) delete []prefix;
    }
};

struct Server
{
    char *name;
    Socket *socket;
    int port;

    Server(const char *n, const int p, Socket *s)
    {
        StrUtil::copy(name, n);
        socket = s;
        port = p;
    }

    ~Server()
    {
        if (NULL != name) delete []name;
        if (NULL != socket) delete socket;
    }
};

class Eagle
{
public:
    ~Eagle();

    const int &getIndex()
    {
        return m_index;
    }

    void setIndex(const int index)
    {
        m_index = index;
    }

    Program &getProgram()
    {
        return m_program;
    } 

    int init(const int argc, char *const *argv, const char *ver);
    void destroy();
    void runWorker(const int isMasterThread = 1);
    void stopWorker();

private:
    Eagle();

    int readPid();
    int writePid();
    int delPid();
    int checkDir();
    void initLog();
    void printUsage();
    void printVer();
    void printPrompt(const char *option);
    void nodeInit();
    void initMaster();
    void initWorker();
    void cleanMaster();
    int masterCycle();
    int initProcess();
    int sendSignal(const char *signal);
    int parseOptions(const int argc, char *const *argv);
    int initServers(std::string &ip, std::map<std::string, int> &serverMap);

    int m_index;
    Server **m_servers;
    Properties *m_properties;
    EventManager *m_acceptManager;
    EventManager *m_receiveManager;
    Program m_program;

    friend class Singleton<Eagle>;
};

typedef Singleton<Eagle> EagleI;

}
#endif   /* ----- #ifndef _EAGLE_EAGLE_H_  ----- */

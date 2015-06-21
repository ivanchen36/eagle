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
#ifndef  _EAGLE_H_
#define  _EAGLE_H_
#include <string>

#include "CallBack.h"
#include "Singleton.h"
#include "Log.h"
#include "Properties.h"

#define EAGLE_INIT(cb, ver) \
    if (0 != EagleI::instance().init(argc, argv, cb, ver)) return 0

struct Program
{
    char *name;
    char *ver;
    char *prefix;
    int logLevel;
    int process;

    Program()
    {
        name = NULL;
        ver = NULL;
        prefix = NULL;
        logLevel = DEBUG_LOG;
    }

    ~Program()
    {
        if (NULL != name) delete name;
        if (NULL != ver) delete ver;
        if (NULL != prefix) delete prefix;
    }
};

struct EagleAttr
{
    static int index;
    static int status;
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

    int init(const int argc, char *const *argv, const CallBack &notifyQuitCb, 
            const char *ver);

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
    void childInit(const CallBack &notifyQuitCb);
    void masterInit();
    void masterClean();
    int masterCycle();
    int spawnChildProcess();
    int parseOptions(const int argc, char *const *argv);
    int sendSignal(const char *signal);

    int m_index;
    Program m_program;
    Properties *m_properties;

    friend class Singleton<Eagle>;
};

typedef Singleton<Eagle> EagleI;
#endif   /* ----- #ifndef _EAGLE_H_  ----- */

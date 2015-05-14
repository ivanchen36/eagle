/**
 *       Filename:  ProcessManager.h
 *
 *    Description:  process manager
 *
 *        Version:  1.0
 *        Created:  05/04/2015 01:16:22 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan Chen, 228268157@qq.com
 *   Organization:  
 */
#ifndef  _PROCESSMANAGER_H_
#define  _PROCESSMANAGER_H_
#include <vector>

#include "Singleton.h"

class ProcessManager
{
public:
    enum Status
    {
        NORMAL,
        SPAWN,
        QUIT,
    };

    ProcessManager();
    ~ProcessManager();

    Status getStatus()
    {
        if (0 == m_processNum) return QUIT;

        if (m_processList.size() == m_processNum)
            return NORMAL;

        return SPAWN;
    }

    void check();
    void quit();
    int stop(const int pid);
    void waitQuit();
    int spawn(int &processNum);
    int reSpawn();

private:
    int m_processNum;
    MutexLock m_lock;
    std::vector<pid_t> m_processList;
};

typedef Singleton<ProcessManager> ProcessManagerI;
#endif   /* ----- #ifndef _PROCESSMANAGER_H_  ----- */

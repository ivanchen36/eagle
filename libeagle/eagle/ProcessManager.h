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
        QUIT
    };

    struct ProcessNode
    {
        int index;
        int pid;
        ProcessNode *next;

        ProcessNode(int i, int p, ProcessNode *n)
            : index(i), pid(p), next(n){};
    };

    ProcessManager();
    ~ProcessManager();

    Status getStatus()
    {
        if (0 == m_processNum) return QUIT;

        if (m_aliveNum >= m_processNum)
            return NORMAL;

        return SPAWN;
    }

    void check();
    void quit(int processNum = 0);
    int stop(const int pid);
    void waitQuit();
    int spawn(int &processNum);
    int reSpawn();

private:
    void deleteProcess(const int pid);

    int m_aliveNum;
    int m_processNum;
    MutexLock m_lock;
    ProcessNode *m_processHead;
};

typedef Singleton<ProcessManager> ProcessManagerI;
#endif   /* ----- #ifndef _PROCESSMANAGER_H_  ----- */

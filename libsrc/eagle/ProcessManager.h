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
    ProcessManager();
    ~ProcessManager();

    int spawnProcess(const int processNum);
    int quitChild(const int pid);
    void quitAllChild();

private:
    std::vector<pid_t> childList;
};

typedef Singleton<ProcessManager> ProcessManagerI;
#endif   /* ----- #ifndef _PROCESSMANAGER_H_  ----- */

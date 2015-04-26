#include "Eagle.h"

Eagle::Eagle()
{

}

Eagle::~Eagle()
{

}

int Eagle::init()
{
#if 0
    sem_t sem;
    if (sem_init(&_sem, 0, 0))
    {
        return -1;
    }
    if (sem_post(&_sem))
    {
        
    }
    pid_t pid = fork();
    if(pid<0)
    {
        fprintf(stderr,"err");
    }
    else if(pid == 0)
    {
        cout << "child" << endl;
        cout << "exit" << endl;
    }
    else
    {
        m_childPid = pid;
    }
#endif
    return 0;
}

void Eagle::destroy()
{
}

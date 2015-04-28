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
    pid_t pid = fork();
    if(pid<0)
    {
    }
    else if(pid == 0)
    {
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

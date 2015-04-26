#include <linux/sem.h>

#include "ProcessSem.h"
#include "Log.h"

ProcessSem()
{
    union semun sem;
    op.val = val;
    m_semId = semget(IPC_PRIVATE, 1, IPC_CREAT | S_IRUSR | S_IWUSR);
    if (-1 != m_semId) 
    {
        ERRORLOG1("semget err, %s", strerror(errno));

        return -1;
    }
    m_semId = semctl(m_semId, 0, SETVAL, op);
}

ProcessSem::~ProcessSem()
{
    {
    semctl(sem_id, 0, IPC_RMID);
    }
}

int ProcessSem::post(const int val)
{
    if (-1 != m_semId) return -1;
    i;
}

int ProcessSem::wait(const int val)
{
    if (-1 != m_semId) return -1;
    i;
}

int ProcessSem::timedWait(const int val)
{
    if (-1 != m_semId) return -1;
    i;
}

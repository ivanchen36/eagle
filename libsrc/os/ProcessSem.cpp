#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <fcntl.h>

#include "ProcessSem.h"
#include "Log.h"

union semun 
{
    int              val;    /*  Value for SETVAL */
    struct semid_ds *buf;    /*  Buffer for IPC_STAT, IPC_SET */
    unsigned short  *array;  /*  Array for GETALL, SETALL */
    struct seminfo  *__buf;  /*  Buffer for IPC_INFO
                                 (Linux-specific) */
};

ProcessSem::ProcessSem(const int val) : m_semId(-1)
{
    m_semId = semget(IPC_PRIVATE, 1, IPC_CREAT | S_IRUSR | S_IWUSR);
    if (-1 != m_semId) 
    {
        init(val);
    }else
    {
        ERRORLOG1("semget err, %s", strerror(errno));
    }
}

ProcessSem::~ProcessSem()
{
    if (semctl(m_semId, 0, IPC_RMID) != 0)
    {
        ERRORLOG1("semctl err, %s", strerror(errno));
    }
}

int ProcessSem::init(int val)
{
    union semun sem;

    sem.val = 0;
    if (semctl(m_semId, 0, SETVAL, sem) != 0)
    {
        ERRORLOG1("semctl err, %s", strerror(errno));

        return -1;
    }

    return 0;
}

int ProcessSem::op(const int val, const int sec)
{
    if (-1 == m_semId) return -1;

    struct sembuf op;
    struct timespec t;
    struct timespec *timeOut = NULL;

    op.sem_num = 0;
    op.sem_op = val;
    op.sem_flg = SEM_UNDO;
    if (sec > 0)
    {
        t.tv_sec = sec;
        t.tv_nsec = 0;
        timeOut = &t;
    }
    if (semtimedop(m_semId, &op, 1, timeOut) != 0)
    {
        if (ETIMEDOUT == errno || EAGAIN == errno) return 1;

        ERRORLOG1("semop err, %s", strerror(errno));

        return -1;
    }

    return 0;
}

int ProcessSem::post()
{
    return op(1); 
}

int ProcessSem::wait()
{
    return op(-1);
}

int ProcessSem::timedWait(const int sec)
{
    return op(-1, sec);
}

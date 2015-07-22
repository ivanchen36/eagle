#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <fcntl.h>

#include "ProcessSem.h"
#include "Log.h"
#include "ShareMem.h"

namespace eagle
{

namespace
{
ShareMem &shareMem = ShareMemI::instance();
}

union semun 
{
    int              val;    /*  Value for SETVAL */
    struct semid_ds *buf;    /*  Buffer for IPC_STAT, IPC_SET */
    unsigned short  *array;  /*  Array for GETALL, SETALL */
    struct seminfo  *__buf;  /*  Buffer for IPC_INFO
                                 (Linux-specific) */
};

ProcessSem::ProcessSem(const int preceesNum, const int val) 
    : m_semId(-1), m_ref(NULL)
{
    m_semId = semget(IPC_PRIVATE, 1, IPC_CREAT | S_IRUSR | S_IWUSR);
    if (-1 == m_semId) 
    {
        ERRORLOG1("semget err, %s", strerror(errno));

        return;
    }

    init(val);
    if (preceesNum > 1)
    {
        m_ref = (int *)shareMem.alloc(sizeof(int));
        *m_ref = preceesNum;
    }
}

ProcessSem::~ProcessSem()
{
    int isDelete = 1;

    if (NULL != m_ref)
    {
        if (__sync_sub_and_fetch(const_cast<volatile int *>(m_ref), 1) != 0)
        {
            isDelete = 0;
        }else
        {
            shareMem.free(m_ref);
        }
    }
    if (!isDelete) return;

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

        return EG_FAILED;
    }

    return EG_SUCCESS;
}

int ProcessSem::op(const int val, const int sec)
{
    if (-1 == m_semId) return EG_INVAL;

    struct sembuf buf;
    struct timespec t;
    struct timespec *timeOut = NULL;

    buf.sem_num = 0;
    buf.sem_op = val;
    buf.sem_flg = SEM_UNDO;
    if (sec > 0)
    {
        t.tv_sec = sec;
        t.tv_nsec = 0;
        timeOut = &t;
    }
    for (; ;)
    {
        if (semtimedop(m_semId, &buf, 1, timeOut) == 0) return EG_SUCCESS;

        if (EINTR == errno) continue;

        if (EAGAIN == errno || ERANGE == errno) return EG_AGAIN;

        ERRORLOG1("semop err, %s", strerror(errno));

        return EG_FAILED;
    }

    return EG_SUCCESS;
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

}

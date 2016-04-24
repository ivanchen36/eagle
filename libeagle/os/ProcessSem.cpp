#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <fcntl.h>
#include <unistd.h>

#include "ProcessSem.h"
#include "Log.h"
#include "ShareMem.h"

namespace eagle
{

union semun 
{
    int              val;    /*  Value for SETVAL */
    struct semid_ds *buf;    /*  Buffer for IPC_STAT, IPC_SET */
    unsigned short  *array;  /*  Array for GETALL, SETALL */
    struct seminfo  *__buf;  /*  Buffer for IPC_INFO
                                 (Linux-specific) */
};

ProcessSem::ProcessSem(const int key, const int val) 
    : m_semId(-1), m_ref(NULL)
{
    ShareMem &shareMem = ShareMemI::instance();

    m_semId = semget(key, 1, IPC_CREAT | S_IRUSR | S_IWUSR);
    if (-1 == m_semId) 
    {
        ERRORLOG1("semget err, %s", strerror(errno));

        return;
    }

    m_ref = (char *)shareMem.alloc(1, key);
    if (shareMem.getNattch((void *)m_ref) == 1)init(val);
}

ProcessSem::~ProcessSem()
{
    int ret;
    ShareMem &shareMem = ShareMemI::instance();

    if (-1 == m_semId) return;

    ret = shareMem.free(m_ref);
    if (0 == ret && semctl(m_semId, 0, IPC_RMID) != 0) 
    {
        ERRORLOG1("semctl err, %s", strerror(errno));
    }
}

int ProcessSem::init(int val)
{
    union semun sem;

    sem.val = val;
    if (semctl(m_semId, 0, SETVAL, sem) != 0)
    {
        ERRORLOG1("semctl err, %s", strerror(errno));

        return EG_FAILED;
    }

    return EG_SUCCESS;
}

int ProcessSem::getVal()
{
    int ret;

    ret = semctl(m_semId, 0, GETVAL, 0);
    if (ret == -1)
    {
        ERRORLOG1("semctl err, %s", strerror(errno));

        return EG_FAILED;
    }

    return ret;
}

int ProcessSem::op(const int val, const int msec)
{
    if (-1 == m_semId) return EG_INVAL;

    struct sembuf buf;
    struct timespec t;
    struct timespec *timeOut = NULL;

    buf.sem_num = 0;
    buf.sem_op = val;
    buf.sem_flg = 0;
    if (msec > 0)
    {
        t.tv_sec = msec / 1000;
        t.tv_nsec = (msec % 1000) * 1000 * 1000;
        timeOut = &t;
    }
    for (; ;)
    {
        if (semtimedop(m_semId, &buf, 1, timeOut) == 0) break;

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

int ProcessSem::timedWait(const int msec)
{
    return op(-1, msec);
}

}

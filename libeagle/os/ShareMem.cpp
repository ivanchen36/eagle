#include <sys/shm.h>

#include "Log.h"
#include "Define.h"
#include "ShareMem.h"

namespace eagle
{

ShareMem::~ShareMem()
{
    ShareMemMap::iterator it;

    for (it = m_shmMap.begin(); it != m_shmMap.end();)
    {
        free((void *)it->first, it->second);
        m_shmMap.erase(it++);
    }
}

void *ShareMem::calloc(const int size, const int key)
{
    void *ptr = alloc(size, key);
    if (NULL != ptr) memset(ptr, 0, size);

    return ptr;
}

void *ShareMem::alloc(const int size, const int key)
{
    int id;
    int isNew = 1;
    void *ptr;

    id = shmget(key, size, (SHM_R | SHM_W | IPC_CREAT | IPC_EXCL));
    if (key != 0 && -1 == id) 
    {
        isNew = 0;
        id = shmget(key, size, (SHM_R | SHM_W));
    }

    if (-1 == id)
    {
        ERRORLOG2("shmget %d err, %s!", size, strerror(errno));

        return NULL;
    }

    ptr = shmat(id, NULL, 0);
    if ((void *)-1 == ptr)
    {
        ERRORLOG1("shmat err, %s!", strerror(errno));

        return NULL;
    }

    if (isNew) 
    {
        memset(ptr, 0, size);
    }
    m_shmMap[(uintptr_t)ptr] = id;

    return ptr;
}

int ShareMem::getNattch(void *ptr)
{
    struct shmid_ds shmds;
    ShareMemMap::iterator iter = m_shmMap.find((uintptr_t)ptr);

    if (iter == m_shmMap.end()) return EG_FAILED;

    if (shmctl(iter->second, IPC_STAT, &shmds) == -1)
    {
        ERRORLOG2("shmctl IPC_STAT  %d err, %s!", iter->second, strerror(errno));

        return EG_FAILED;
    }

    return shmds.shm_nattch;
}

int ShareMem::free(void *ptr, const int id)
{
    struct shmid_ds shmds;

    if (shmdt(ptr) == -1) 
    {
        ERRORLOG1("shmdt err, %s!", strerror(errno));

        return EG_FAILED;
    }

    if (shmctl(id, IPC_STAT, &shmds) == -1)
    {
        ERRORLOG2("shmctl IPC_STAT %d err, %s!", id, strerror(errno));

        return EG_FAILED;
    }

    if (0 == shmds.shm_nattch && shmctl(id, IPC_RMID, NULL) == -1) 
        ERRORLOG2("shmctl IPC_RMID %d err, %s!", id, strerror(errno));

    return shmds.shm_nattch;
}

int ShareMem::free(void *ptr)
{
    int ret = EG_FAILED;
    if (NULL == ptr) return ret;

    ShareMemMap::iterator iter = m_shmMap.find((uintptr_t)ptr);
    if (iter == m_shmMap.end()) return ret;

    ret = free(ptr, iter->second);
    m_shmMap.erase(iter);

    return ret;
}

}

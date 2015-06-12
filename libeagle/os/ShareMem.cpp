#include <sys/shm.h>

#include "Log.h"
#include "ShareMem.h"

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
    if (0 == key)
    {
        if (shmctl(id, IPC_RMID, NULL) == -1)
        {
            ERRORLOG1("shmctl IPC_RMID err, %s!", strerror(errno));
        }
        id = -1;
    }
    if ((void *)-1 == ptr)
    {
        ERRORLOG1("shmat err, %s!", strerror(errno));

        return NULL;
    }

    m_shmMap[(uintptr_t)ptr] = isNew ? id : -1;

    return ptr;
}

void ShareMem::free(void *ptr, int id)
{
    if (shmdt(ptr) == -1) ERRORLOG1("shmdt err, %s!", strerror(errno));

    if (-1 != id && shmctl(id, IPC_RMID, NULL) == -1) 
        ERRORLOG2("shmctl IPC_RMID  %d err, %s!", id, strerror(errno));
}

void ShareMem::free(void *ptr)
{
    if (NULL == ptr) return;

    ShareMemMap::iterator iter = m_shmMap.find((uintptr_t)ptr);
    if (iter == m_shmMap.end()) return;

    free(ptr, iter->second);
    m_shmMap.erase(iter);
}
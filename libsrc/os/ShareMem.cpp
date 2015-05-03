#include <sys/shm.h>

#include "Log.h"
#include "ShareMem.h"

ShareMem::~ShareMem()
{
    std::map<intptr_t, int>::iterator it;

    for (it = m_shmMap.begin(); it != m_shmMap.end();)
    {
        free((void *)it->first, it->second);
        m_shmMap.erase(it++);
    }
}

void *ShareMem::calloc(const int size, const int key)
{
    void *ptr = alloc(size, key);
    if (!ptr) memset(ptr, 0, size);

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
    if (-1 == (intptr_t)ptr)
    {
        ERRORLOG1("shmat err, %s!", strerror(errno));

        return NULL;
    }

    m_shmMap[(intptr_t)ptr] = isNew ? id : -1;

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
    if (!ptr) return;

    int id = m_shmMap[(intptr_t)ptr];

    m_shmMap.erase((intptr_t)ptr);
    free(ptr, id);
}

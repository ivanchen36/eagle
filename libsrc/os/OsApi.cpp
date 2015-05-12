#include "OsApi.h"

void OsApi::atomicAdd(int &var, const int val)
{
    __sync_fetch_and_add(const_cast<volatile int *>(&var), val);
}

void OsApi::atomicSub(int &var, const int val)
{
    __sync_fetch_and_sub(const_cast<volatile int *>(&var), val);
}

int OsApi::atomicCompare(int &var, const int val)
{
    if (__sync_bool_compare_and_swap(const_cast<volatile int *>(&var), val, val))
        return 0;

    return -1;
}

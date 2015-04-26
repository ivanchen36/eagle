#include "OsApi.h"

void OsApi::atomicAdd(int &var, const int val)
{
    __sync_fetch_and_add(const_cast<volatile int *>(&var), val);
}

void OsApi::atomicSub(int &var, const int val)
{
    __sync_fetch_and_sub(const_cast<volatile int *>(&var), val);
}

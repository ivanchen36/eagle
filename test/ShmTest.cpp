/**
 *       Filename:  ShmTest.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  04/26/2015 02:18:19 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan Chen, 228268157@qq.com
 *   Organization:  
 */


#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <sys/wait.h>
#include <fcntl.h>

#include "Log.h"
#include "EagleTime.h"
#include "ShareMem.h"

using namespace std;


void test(int isFree)
{
    int key = 1;
    void *ptr = NULL;
    std::vector<uintptr_t> shmList(10);
    for (int i = 0; i < 10; ++i)
    {
        ptr = ShareMemI::instance().alloc(1024, key++);
        if (!ptr)
        {
            DEBUGLOG("alloc fail");
            continue;
        }
        shmList[i] = (uintptr_t)ptr; 
        DEBUGLOG1("alloc shm addr %u", (uintptr_t)ptr);
    }

    sleep(5);
    if (!isFree) return;

    for (int i = 0; i < 10; ++i)
    {
        ptr = (void *)shmList[i];
        ShareMemI::instance().free(ptr);
        DEBUGLOG1("free shm addr %u", (uintptr_t)ptr);
    }
}

const char *CHILDDIALOGARR[] = 
{
    "hello parent",
    "i'm child",
    "glad to see you",
    "bye parent"
};

const char *PARENTDIALOGARR[] = 
{
    "hello child",
    "i'm parent",
    "mee too",
    "bye child"
};

void child(char *str)
{
    for (int i = 0; i < 5; ++i)
    {
        if (0 == i)
        {
            strcpy(str, CHILDDIALOGARR[i]);

            continue;
        }
        while (strcmp(str, PARENTDIALOGARR[i - 1]))
        {
            sleep(1);
        }
        DEBUGLOG1("parent say : %s", str);
        if (4 == i) return;

        strcpy(str, CHILDDIALOGARR[i]);
    }
}

void parent(char *str)
{
    for (int i = 0; i < 4; ++i)
    {
        while (strcmp(str, CHILDDIALOGARR[i]))
        {
            sleep(1);
        }
        DEBUGLOG1("child say : %s", str);
        strcpy(str, PARENTDIALOGARR[i]);
    }
}

void test1()
{
    int ret;
    char *str;
    int dialogKey = 888;

    pid_t pid=fork();
    if(pid<0)
    {
        ERRORLOG("fork err");
    }
    else if(pid == 0)
    {
        str = (char *)ShareMemI::instance().alloc(48, dialogKey);
        str[0] = 0;
        child(str);
        ShareMemI::instance().free(str);
    }
    else
    {
        str = (char *)ShareMemI::instance().alloc(48, dialogKey);
        str[0] = 0;
        parent(str);
        ShareMemI::instance().free(str);
        waitpid(-1, NULL, 0);
    }
}

/**
 * @brief main 
 */
int main ( int argc, char *argv[] )
{
    EagleTimeI::instance().autoUpdate();

    test1();

    return EXIT_SUCCESS;
}

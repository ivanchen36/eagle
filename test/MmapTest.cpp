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

#include "Log.h"
#include "MmapFile.h"

using namespace std;
using namespace eagle;

void test()
{
    char str[12] = "zdsf23";
    MmapFilePtr mmFile = new MmapFile("mmtest", 1);
    
    mmFile->write(str, 0);
    mmFile->flush();
    sleep(11);
    mmFile->read(str, 5, 0);
    DEBUGLOG1("%s", str);
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

void child(MmapFilePtr &mmFile)
{
    char str[48];
    for (int i = 0; i < 5; ++i)
    {
        if (!i)
        {
            mmFile->write(CHILDDIALOGARR[i], 0);
            continue;
        }
        do
        {
            sleep(1);
            mmFile->read(str, strlen(PARENTDIALOGARR[i - 1]), 0);
        }while (strcmp(str, PARENTDIALOGARR[i - 1]));
        DEBUGLOG1("parent say : %s", str);
        if (4 == i) return;

        mmFile->write(CHILDDIALOGARR[i], 0);
    }
}

void parent(MmapFilePtr &mmFile)
{
    char str[48];
    for (int i = 0; i < 4; ++i)
    {
        do
        {
            sleep(1);
            mmFile->read(str, strlen(CHILDDIALOGARR[i]), 0);
        }while (strcmp(str, CHILDDIALOGARR[i]));
        DEBUGLOG1("child say : %s", str);
        mmFile->write(PARENTDIALOGARR[i], 0);
    }
}

void test1()
{
    pid_t pid=fork();
    if(pid<0)
    {
        ERRORLOG("fork err");
    }
    else if(pid == 0)
    {
        MmapFilePtr mmFile = new MmapFile("dialog", 1);
        child(mmFile);
        exit(3);
    }
    else
    {
        MmapFilePtr mmFile = new MmapFile("dialog", 1);
        parent(mmFile);
        waitpid(-1, NULL, 0);
    }
}

/**
 * @brief main 
 */
int main ( int argc, char *argv[] )
{
    test1();

    return EXIT_SUCCESS;
}

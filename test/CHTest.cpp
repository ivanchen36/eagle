/**
 *       Filename:  CHTest.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  04/03/2016 12:53:07 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan Chen, 228268157@qq.com
 *   Organization:  
 */
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "ConsistentHashing.h"

using namespace std;
using namespace eagle;

class CHTest : public ConsistentHashing<int>::Operator
{
public:
    virtual void dispatch(int &num)
    {
        cout << "dispatch " << num <<endl;
    }
};

typedef AutoPtr<CHTest> CHTestPtr;

void doTest1()
{
    int num = 1;
    ConsistentHashing<int> ch;
    AutoPtr<ConsistentHashing<int>::Operator> test = new CHTest();

    ch.regist(1);
    ch.regist(2, test);
    ch.regist(3);
    ch.regist(4);
    ch.regist(5);
    ch.push(num, num);
    ++num;
    ch.push("test", num);
    ch.unregist(1);
    ch.unregist(2);
    ch.unregist(3);
    ch.unregist(4);
    ch.unregist(5);
    ch.regist(1);
    ch.regist(2, test);
    ch.regist(3);
    ch.regist(4);
    ch.regist(5);
    ++num;
    ch.push(num, num);
    ch.unregist(1);
    ch.unregist(2);
    ch.unregist(3);
    ch.unregist(4);
    ch.unregist(5);
}

/**
 * @brief main 
 */
int main ( int argc, char *argv[] )
{
    doTest1();

    return EXIT_SUCCESS;
}

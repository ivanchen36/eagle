/**
 *       Filename:  AutoPtrTest.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  04/19/2015 04:04:44 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan Chen, 228268157@qq.com
 *   Organization:  
 */
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <tr1/unordered_map>

#include "AutoPtr.h"

using namespace std;

class AutoPtrTest
{
public: 
    AutoPtrTest(const char *name)
    {
        m_name = name;
        cout << "constructor" << " " << m_name << endl;
    }

    AutoPtrTest(const AutoPtrTest &test)
    {
        m_name = test.m_name;
        cout << "constructor" << " " << m_name << endl;
    }

    virtual ~AutoPtrTest()
    {
        cout << "destructor" << " " << m_name << endl;
    }

    void print()
    {
        cout << "print"  << " " << m_name << endl;
    }
 
    AutoPtrTest()
    {
        m_name = "test";
        cout << "constructor" << " " << m_name << endl;
    }

protected:
    string m_name;
};

class AutoPtrTestChild : public AutoPtrTest
{
public:
    AutoPtrTestChild(const char *name)
    {
        cout << "child constructor" << endl;
    }

    ~AutoPtrTestChild()
    {
        cout << "child destructor" << endl;
    }
};

typedef AutoPtr<AutoPtrTest> APTestPtr;

void doTest()
{
    APTestPtr child = new AutoPtrTestChild("");
    APTestPtr ptr = new AutoPtrTest("test1");
    APTestPtr ptr1 = ptr;
    ptr->print();
    ptr1->print();
    ptr = new AutoPtrTest("test2");
    ptr->print();
}

void doTest1()
{
    tr1::unordered_map<int, APTestPtr> map;
    APTestPtr child = new AutoPtrTestChild("");
    map[1] = child;
}

void doTest2()
{
    tr1::unordered_map<int, APTestPtr> map;
    APTestPtr child = new AutoPtrTestChild("");
    map[1] = child;
    map.erase(1);
}

/**
 * @brief main 
 */
int main ( int argc, char *argv[] )
{
    doTest();

    return EXIT_SUCCESS;
}

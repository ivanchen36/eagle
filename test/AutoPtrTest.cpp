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
#include <vector>
#include <tr1/unordered_map>

#include "AutoPtr.h"
#include "AutoPtr1.h"

using namespace std;
using namespace eagle;

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
    APTestPtr testPtr;
    if (!testPtr)
    {
        cout << "null ptr" << endl;
    }
    if (NULL == testPtr)
    {
        cout << "null ptr" << endl;
    }
    APTestPtr child = new AutoPtrTestChild("");
    APTestPtr ptr = new AutoPtrTest("test1");
    APTestPtr ptr1 = ptr;
    testPtr = ptr1;
    if (child && ptr && ptr1)
    {
        ptr->print();
        ptr1->print();
        ptr = new AutoPtrTest("test2");
        ptr->print();
        testPtr->print();
    }
}

void doTest1()
{
    vector<APTestPtr> vec;
    tr1::unordered_map<int, APTestPtr> map;
    APTestPtr child = new AutoPtrTestChild("");

    map[1] = child;
    vec.push_back(new AutoPtrTestChild(""));
}

void doTest2()
{
    vector<APTestPtr> vec;
    tr1::unordered_map<int, APTestPtr> map;
    APTestPtr child = new AutoPtrTestChild("");

    for (int i = 0; i < 10; ++i)
    {
        cout << "i  " << i << endl;
        map[i] = child;
        map.erase(i);
        vec.push_back(new AutoPtrTestChild(""));
        vec.clear();
    }
}

class AutoPtr1Test : public Reference
{
public: 
    AutoPtr1Test(const char *name)
    {
        m_name = name;
        cout << "constructor" << " " << m_name << endl;
    }

    AutoPtr1Test(const AutoPtr1Test &test)
    {
        m_name = test.m_name;
        cout << "constructor" << " " << m_name << endl;
    }

    virtual ~AutoPtr1Test()
    {
        cout << "destructor" << " " << m_name << endl;
    }

    void print()
    {
        cout << "print"  << " " << m_name << endl;
    }
 
    AutoPtr1Test()
    {
        m_name = "test";
        cout << "constructor" << " " << m_name << endl;
    }

protected:
    string m_name;
};

class AutoPtr1TestChild : public AutoPtr1Test
{
public:
    AutoPtr1TestChild(const char *name)
    {
        cout << "child constructor" << endl;
    }

    ~AutoPtr1TestChild()
    {
        cout << "child destructor" << endl;
    }
};

typedef AutoPtr1<AutoPtr1Test> APTest1Ptr;

void doTest3()
{
    APTest1Ptr testPtr;
    if (!testPtr)
    {
        cout << "null ptr" << endl;
    }
    if (NULL == testPtr)
    {
        cout << "null ptr" << endl;
    }
    APTest1Ptr child = new AutoPtr1TestChild("");
    APTest1Ptr ptr = new AutoPtr1Test("test1");
    APTest1Ptr ptr1 = ptr;
    testPtr = ptr1;
    if (child && ptr && ptr1)
    {
        ptr->print();
        ptr1->print();
        ptr = new AutoPtr1Test("test2");
        ptr->print();
        testPtr->print();
    }
}

void test4(void **ptr)
{
    APTest1Ptr child = new AutoPtr1TestChild("");
    *ptr = (void *)child.ptr();
    child->inc();
}

void doTest4()
{
    AutoPtr1Test *ptr;
    test4((void **)&ptr);
    cout << "leave test4" << endl;
    if (ptr->dec() == 0) delete ptr;
}

/**
 * @brief main 
 */
int main ( int argc, char *argv[] )
{
    doTest4();

    return EXIT_SUCCESS;
}

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

    ~AutoPtrTest()
    {
        cout << "destructor" << " " << m_name << endl;
    }

    void print()
    {
        cout << "print"  << " " << m_name << endl;
    }

private:
    AutoPtrTest()
    {
        m_name = "test";
        cout << "constructor" << " " << m_name << endl;
    }

    friend class AutoPtr<AutoPtrTest>;

    string m_name;
};

typedef AutoPtr<AutoPtrTest> APTestPtr;

/**
 * @brief main 
 */
int main ( int argc, char *argv[] )
{
    APTestPtr ptr = new AutoPtrTest("test1");
    APTestPtr ptr1 = ptr;
    ptr->print();
    ptr1->print();
    ptr = new AutoPtrTest("test2");
    ptr->print();

    return EXIT_SUCCESS;
}

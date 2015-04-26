/**
 *       Filename:  SingleTest.cpp
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

#include "Singleton.h"

using namespace std;

class SingleTest
{
public: 
    void test()
    {
        cout << m_name << endl;
    }

private:
    SingleTest()
    {
        m_name = "i'm singletest";
        cout << "constructor" << endl;
    }
    ~SingleTest()
    {
        cout << "destructor" << endl;
    }

    std::string m_name;
    friend class Singleton<SingleTest>;
};

typedef Singleton<SingleTest> SingleTestI;

/**
 * @brief main 
 */
int main ( int argc, char *argv[] )
{
    SingleTestI::instance().test();

    return EXIT_SUCCESS;
}

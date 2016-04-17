/**
 *       Filename:  LogTest.cpp
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

#include "Log.h"
#include "RbTree.h"
#include "EagleTime.h"

using namespace std;
using namespace eagle;

void print(RbTree<int, int> &tree)
{
    RbTree<int, int>::Iterator iter;

    iter = tree.getMin();
    if (iter != tree.end())
    {
        DEBUGLOG2("min : key %d, val %d", iter->key, iter->val);
    }
    iter = tree.getMax();
    if (iter != tree.end())
    {
        DEBUGLOG2("max : key %d, val %d", iter->key, iter->val);
    }
    DEBUGLOG1("elements %d", tree.size());
}

void test()
{
    RbTree<int, int> tree;
    RbTree<int, int>::Iterator iter;

    for (int i = 0; i < 10000; ++i)
    {
        tree.insert(i, i + 100);
    }

    for (int i = 100000; i > 80000; --i)
    {
        tree.insert(i, i + 100);
    }

    print(tree);
    
    for (iter = tree.begin(); iter != tree.end(); ++iter)
    {
        iter->val = iter->val - 88;
    }

    iter = tree.find(888);
    {
        DEBUGLOG2("key %d, val %d", iter->key, iter->val);
    }

    for (int i = 0; i < 10000; ++i)
    {
        iter = tree.find(i);
        if (iter != tree.end())
        {
            tree.erase(iter);
        }
    }
    print(tree);
    for (int i = 20000; i < 40000; ++i)
    {
        tree.insert(i, i + 100);
    }
    for (int i = 100000; i > 80000; --i)
    {
        iter = tree.find(i);
        if (iter != tree.end())
        {
            tree.erase(iter);
        }
    }
    print(tree);

    for (iter = tree.begin(); iter != tree.end(); )
    {
        iter = tree.erase(iter);
    }

    print(tree);
}

void test1()
{

}

/**
 * @brief main 
 */
int main ( int argc, char *argv[] )
{
    test();

    return EXIT_SUCCESS;
}

/**
 *       Filename:  EagleTest.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/04/2015 10:49:15 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan Chen, 228268157@qq.com
 *   Organization:  
 */

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "Eagle.h"
#include "EventHandler.h"

class TestMessage : public MessageHandler
{
public:
    ~TestMessage()
    {
        send("bye\n");
    }

    IoBuffer *handle(IoBuffer *ioBuf)
    {
        if (strncmp((char *)ioBuf->buf + ioBuf->offset, 
                    "quit", strlen("quit")) != 0)
        {
            send("hello\n");
        }else
        {
            close();
        }

        return ioBuf->next;
    }
};

REGISTER_REFLECTOR("test", TestMessage);

/**
 * @brief main 
 */
EAGLE_MAIN("0.1.0")

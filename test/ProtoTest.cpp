/**
 *       Filename:  ProtoTest.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07/26/2015 03:23:00 PM
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
#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/wire_format_lite_inl.h>

#include "Log.h"
#include "EagleTime.h"
#include "Propose.pb.h"

using namespace std;
using namespace eagle;
using namespace google::protobuf;
using namespace google::protobuf::internal;

void test1()
{
    int size;
    char buf[24];
    std::string str;
    Propose propose;    
    Propose propose1;    
    Propose propose2;    
    
    str.reserve(24);
    propose.set_seq(0);
    DEBUGLOG1("%d", propose.ByteSize());

    if (propose.name().empty())
    {
        propose.set_name("123");
    }
    if (propose.value().empty())
    {
        propose.set_value("456");
    }

    propose.set_seq(1);
    size = propose.ByteSize();
    DEBUGLOG1("%d", size);
    if (!propose.SerializeToString(&str))
    {
        DEBUGLOG("SerializeToString err");
    }
    if (!propose.SerializeToArray((void *)buf, sizeof(buf)))
    {
        DEBUGLOG("SerializeToString err");
    }

    if (!propose1.ParseFromString(str))
    {
        DEBUGLOG("ParseFromString err");
    }
    DEBUGLOG1("%s", propose1.DebugString().c_str());
    if (!propose2.ParseFromArray(buf, size))
    {
        DEBUGLOG("ParseFromString err");
    }
    DEBUGLOG1("%s", propose2.ShortDebugString().c_str());
}

void test2()
{
    Propose *propose = NULL;
    const Descriptor* descriptor =
        DescriptorPool::generated_pool()->FindMessageTypeByName("eagle.Propose");
    if (NULL == descriptor)
    {
        DEBUGLOG("FindMessageTypeByName err");

        return;
    }

    propose = (Propose *)MessageFactory::generated_factory()->GetPrototype(descriptor)->New();

    if (NULL == propose) return;

    propose->set_name("123");
    propose->set_value("456");
    propose->set_seq(1);
    DEBUGLOG1("%s", propose->ShortDebugString().c_str());
    delete propose;
}

void test3()
{
    std::string str;
    Propose propose;    
    const Descriptor* descriptor = propose.GetDescriptor();
    const Reflection* reflection = propose.GetReflection();
    const FieldDescriptor* field = descriptor->FindFieldByName("name");

    propose.set_name("123");
    propose.set_value("456");
    propose.set_seq(1);
    if (NULL != field)
    {
        if (reflection->HasField(propose, field))
        {
            str = reflection->GetString(propose, field);
            DEBUGLOG1("name %s", str.c_str());
        }
    }

    field = descriptor->FindFieldByName("seq");
    if (NULL != field)
    {
        if (reflection->HasField(propose, field))
        {
            DEBUGLOG1("name %d", reflection->GetInt32(propose, field));
        }
    }
}

void test4()
{
    Propose propose;    
    const FieldDescriptor* field;
    const Descriptor* descriptor = propose.GetDescriptor();
    const Reflection* reflection = propose.GetReflection();
    int size = descriptor->field_count();

    for (int i = 0; i < size; ++i)
    {
        field = descriptor->field(i);
        if (i == 0 && field->cpp_type() == FieldDescriptor::CPPTYPE_STRING)
        {
            reflection->SetString(&propose, field, "123");
        }else if (i == 1 && field->cpp_type() == FieldDescriptor::CPPTYPE_STRING)
        {
            reflection->SetString(&propose, field, "456");
        }else if (i == 2 && field->cpp_type() == FieldDescriptor::CPPTYPE_INT32)
        {
            reflection->SetInt32(&propose, field, 1);
        }else
        {
            DEBUGLOG("err");
        }
    }
    DEBUGLOG1("%s", propose.ShortDebugString().c_str());
}

void test5()
{
    char buf[24];
    std::string str;
    Propose propose;    
    Propose propose1;    
    const FieldDescriptor* field;
    const Descriptor* descriptor = propose.GetDescriptor();
    const Reflection* reflection = propose.GetReflection();
    int size = descriptor->field_count();
    google::protobuf::uint8 *tmp = (google::protobuf::uint8 *)buf;

    propose.set_name("123");
    propose.set_value("456");
    propose.set_seq(1);

    for (int i = 0; i < size; ++i)
    {
        field = descriptor->field(i);
        if (field->cpp_type() == FieldDescriptor::CPPTYPE_STRING)
        {
            tmp = WireFormatLite::WriteStringToArray(i + 1, 
                    reflection->GetString(propose, field), tmp);
        }else if (field->cpp_type() == FieldDescriptor::CPPTYPE_INT32)
        {
            tmp = WireFormatLite::WriteInt32ToArray(i + 1, 
                    reflection->GetInt32(propose, field), tmp);
        }else
        {
            DEBUGLOG("err");
        }
    }
    if (!propose1.ParseFromArray(buf, (char *)tmp - buf))
    {
        DEBUGLOG("ParseFromString err");
    }
    DEBUGLOG1("%s", propose1.ShortDebugString().c_str());
}

/**
 * @brief main 
 */
int main ( int argc, char *argv[] )
{
    EagleTimeI::instance().autoUpdate();

    test5();

    return EXIT_SUCCESS;
}

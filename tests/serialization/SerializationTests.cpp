/**************************************************************************
 * @file:  SerializationTests.cpp
 * @brief:
 *
 * Copyright 2021 O-Net Technologies (Group) Limited.
**************************************************************************/

#include <gtest/gtest.h>

#include "addressbook.capnp.h"
#include <capnp/message.h>
#include <capnp/serialize-packed.h>
#include <capnp/serialize-text.h>
#include <iostream>

#include <common/FileSystem.h>

#ifdef __linux__
#include <fcntl.h>
#endif

using addressbook::AddressBook;
using addressbook::Person;

void WriteAddressBook(int fd)
{
    capnp::MallocMessageBuilder message;
    AddressBook::Builder address_book = message.initRoot<AddressBook>();
    capnp::List<Person>::Builder people = address_book.initPeople(2);

    Person::Builder alice = people[0];
    alice.setId(123);
    alice.setName("Alice");
    alice.setEmail("alice@example.com");

    auto alice_phones = alice.initPhones(1);
    alice_phones[0].setNumber("555-1212");
    alice_phones[0].setType(Person::PhoneNumber::Type::MOBILE);
    alice.getEmployment().setSchool("MIT");

    Person::Builder bob = people[1];
    bob.setId(456);
    bob.setName("Bob");
    bob.setEmail("bob@example.com");
    auto bob_phones = bob.initPhones(2);
    bob_phones[0].setNumber("555-4567");
    bob_phones[0].setType(Person::PhoneNumber::Type::HOME);
    bob_phones[1].setNumber("555-7654");
    bob_phones[1].setType(Person::PhoneNumber::Type::WORK);
    bob.getEmployment().setUnemployed();

    capnp::TextCodec codec;
    codec.setPrettyPrint(true);
    auto text = codec.encode(message.getRoot<AddressBook>());

    std::cout << text.cStr() << std::endl;

    // writePackedMessageToFd(fd, message);
}

void PrintAddressBook(int fd)
{
    capnp::PackedFdMessageReader message(fd);
    AddressBook::Reader address_book = message.getRoot<AddressBook>();

    for (Person::Reader person : address_book.getPeople())
    {
        std::cout << person.getName().cStr() << ": "
                  << person.getEmail().cStr() << std::endl;
        for (Person::PhoneNumber::Reader phone : person.getPhones())
        {
            const char *type_name = "UNKNOWN";
            switch (phone.getType())
            {
            case Person::PhoneNumber::Type::MOBILE:
                type_name = "mobile";
                break;
            case Person::PhoneNumber::Type::HOME:
                type_name = "home";
                break;
            case Person::PhoneNumber::Type::WORK:
                type_name = "work";
                break;
            default:
                break;
            }
            std::cout << "  " << type_name << " phone: "
                      << phone.getNumber().cStr() << std::endl;
        }
        Person::Employment::Reader employment = person.getEmployment();
        switch (employment.which())
        {
        case Person::Employment::UNEMPLOYED:
            std::cout << "  unemployed" << std::endl;
            break;
        case Person::Employment::EMPLOYER:
            std::cout << "  employer: "
                      << employment.getEmployer().cStr() << std::endl;
            break;
        case Person::Employment::SCHOOL:
            std::cout << "  student at: "
                      << employment.getSchool().cStr() << std::endl;
            break;
        case Person::Employment::SELF_EMPLOYED:
            std::cout << "  self-employed" << std::endl;
            break;
        default:
            break;
        }
    }
}

#include "addressbook.capnp.h"
#include <capnp/message.h>
#include <capnp/serialize-packed.h>
#include <iostream>
#include <capnp/schema.h>
#include <capnp/dynamic.h>

using ::capnp::DynamicEnum;
using ::capnp::DynamicList;
using ::capnp::DynamicStruct;
using ::capnp::DynamicValue;
using ::capnp::EnumSchema;
using ::capnp::List;
using ::capnp::Schema;
using ::capnp::StructSchema;

using ::capnp::MallocMessageBuilder;
using ::capnp::PackedFdMessageReader;
using ::capnp::Text;
using ::capnp::Void;

void DynamicWriteAddressBook(int fd, StructSchema schema)
{
    // Write a message using the dynamic API to set each
    // field by text name.  This isn't something you'd
    // normally want to do; it's just for illustration.

    MallocMessageBuilder message;

    // Types shown for explanation purposes; normally you'd
    // use auto.
    DynamicStruct::Builder addressBook =
        message.initRoot<DynamicStruct>(schema);

    DynamicList::Builder people =
        addressBook.init("people", 2).as<DynamicList>();

    DynamicStruct::Builder alice =
        people[0].as<DynamicStruct>();
    alice.set("id", 123);
    alice.set("name", "Alice");
    alice.set("email", "alice@example.com");
    auto alicePhones = alice.init("phones", 1).as<DynamicList>();
    auto phone0 = alicePhones[0].as<DynamicStruct>();
    phone0.set("number", "555-1212");
    phone0.set("type", "mobile");
    alice.get("employment").as<DynamicStruct>().set("school", "MIT");

    auto bob = people[1].as<DynamicStruct>();
    bob.set("id", 456);
    bob.set("name", "Bob");
    bob.set("email", "bob@example.com");

    // Some magic:  We can convert a dynamic sub-value back to
    // the native type with as<T>()!
    List<Person::PhoneNumber>::Builder bobPhones =
        bob.init("phones", 2).as<List<Person::PhoneNumber>>();
    bobPhones[0].setNumber("555-4567");
    bobPhones[0].setType(Person::PhoneNumber::Type::HOME);
    bobPhones[1].setNumber("555-7654");
    bobPhones[1].setType(Person::PhoneNumber::Type::WORK);
    bob.get("employment").as<DynamicStruct>().set("unemployed", ::capnp::VOID);

    writePackedMessageToFd(fd, message);
}

void DynamicPrintValue(DynamicValue::Reader value)
{
    // Print an arbitrary message via the dynamic API by
    // iterating over the schema.  Look at the handling
    // of STRUCT in particular.

    switch (value.getType())
    {
    case DynamicValue::VOID:
        std::cout << "";
        break;
    case DynamicValue::BOOL:
        std::cout << (value.as<bool>() ? "true" : "false");
        break;
    case DynamicValue::INT:
        std::cout << value.as<int64_t>();
        break;
    case DynamicValue::UINT:
        std::cout << value.as<uint64_t>();
        break;
    case DynamicValue::FLOAT:
        std::cout << value.as<double>();
        break;
    case DynamicValue::TEXT:
        std::cout << '\"' << value.as<Text>().cStr() << '\"';
        break;
    case DynamicValue::LIST:
    {
        std::cout << "[";
        bool first = true;
        for (auto element : value.as<DynamicList>())
        {
            if (first)
            {
                first = false;
            }
            else
            {
                std::cout << ", ";
            }
            DynamicPrintValue(element);
        }
        std::cout << "]";
        break;
    }
    case DynamicValue::ENUM:
    {
        auto enumValue = value.as<DynamicEnum>();
        KJ_IF_MAYBE(enumerant, enumValue.getEnumerant())
        {
            std::cout << enumerant->getProto().getName().cStr();
        }
        else
        {
            // Unknown enum value; output raw number.
            std::cout << enumValue.getRaw();
        }
        break;
    }
    case DynamicValue::STRUCT:
    {
        std::cout << "(";
        auto structValue = value.as<DynamicStruct>();
        bool first = true;
        for (auto field : structValue.getSchema().getFields())
        {
            if (!structValue.has(field))
                continue;
            if (first)
            {
                first = false;
            }
            else
            {
                std::cout << ", ";
            }
            std::cout << field.getProto().getName().cStr()
                      << " = ";
            DynamicPrintValue(structValue.get(field));
        }
        std::cout << ")";
        break;
    }
    default:
        // There are other types, we aren't handling them.
        std::cout << "?";
        break;
    }
}

void DynamicPrintMessage(int fd, StructSchema schema)
{
    PackedFdMessageReader message(fd);
    DynamicPrintValue(message.getRoot<DynamicStruct>(schema));
    std::cout << std::endl;
}

char filename[] = "serialization-test.bin";

TEST(SerializationTests, WriteTest)
{
    int fd = open(filename, O_RDWR | O_CREAT);
    ASSERT_GT(fd, 0);
    WriteAddressBook(fd);
    close(fd);
}

TEST(SerializationTests, ReadTest)
{
    // int fd = open(filename, O_RDWR);
    // ASSERT_GT(fd, 0);
    PrintAddressBook(0);
    // close(fd);
    fs::remove(filename);
}

TEST(SerializationTests, DynamicWriteTest)
{
    int fd = open(filename, O_RDWR | O_CREAT);
    ASSERT_GT(fd, 0);
    StructSchema schema = Schema::from<AddressBook>();
    DynamicWriteAddressBook(fd, schema);
    close(fd);
}

TEST(SerializationTests, DynamicReadTest)
{
    // int fd = open(filename, O_RDWR);
    // ASSERT_GT(fd, 0);
    StructSchema schema = Schema::from<AddressBook>();
    DynamicPrintMessage(0, schema);
    // close(fd);
    fs::remove(filename);
}

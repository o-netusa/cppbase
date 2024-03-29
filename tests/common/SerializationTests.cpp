/**************************************************************************
 * @file: SerializationTests.cpp
 * @brief:
 *
 * Copyright (c) 2022 O-Net Communications Inc.
 * All rights reserved.
*************************************************************************/

#include <gtest/gtest.h>
#include <common/Serializer.h>
#include <uuid.h>

struct Object
{
    int i = 1;
    std::string s = "hello";
    bool b = true;
    double d = 3.14;
    std::vector<int> v = {1, 2, 3};
    std::unordered_map<std::string, int> m = {{"a", 1}, {"b", 2}};
    std::list<int> l = {1, 2, 3};
    uuids::uuid u = uuids::uuid::from_string("00000000-0000-0000-0000-000000000000").value();

    bool operator==(const Object& other) const
    {
        return i == other.i && s == other.s && b == other.b && d == other.d && v == other.v && m == other.m && l == other.l && u == other.u;
    }

    template<typename Archive>
    void serialize(Archive& archive, const uint32_t)
    {
        archive(i, s, b, d, v, m, l, u);
    }
};

class Object2
{
public:
    bool operator==(const Object2& other) const
    {
        return m_data == other.m_data;
    }

    void SetData(int data)
    {
        m_data = data;
    }
private:
    int m_data{0};

    template<class Archive>
    void serialize(Archive& archive, const uint32_t)
    {
        archive(m_data);
    }

    SERIALIZATION_FRIEND_ACCESS
};

TEST(SerializationTests, BasicTypes)
{
    // Test serialization of basic types
    {
        int i = 1;
        auto i_ser = cppbase::Serializer::SaveObjectToStringBinary(i);
        int i_deser = cppbase::Serializer::LoadObjectFromStringBinary<int>(i_ser);
        EXPECT_EQ(i, i_deser);

        std::string s = "hello";
        auto s_ser = cppbase::Serializer::SaveObjectToStringBinary(s);
        std::string s_deser = cppbase::Serializer::LoadObjectFromStringBinary<std::string>(s_ser);
        EXPECT_EQ(s, s_deser);

        bool b = true;
        auto b_ser = cppbase::Serializer::SaveObjectToStringBinary(b);
        bool b_deser = cppbase::Serializer::LoadObjectFromStringBinary<bool>(b_ser);
        EXPECT_EQ(b, b_deser);

        double d = 3.14;
        auto d_ser = cppbase::Serializer::SaveObjectToStringBinary(d);
        double d_deser = cppbase::Serializer::LoadObjectFromStringBinary<double>(d_ser);
        EXPECT_EQ(d, d_deser);

        std::vector<int> v = {1, 2, 3};
        auto v_ser = cppbase::Serializer::SaveObjectToStringBinary(v);
        std::vector<int> v_deser = cppbase::Serializer::LoadObjectFromStringBinary<std::vector<int>>(v_ser);
        EXPECT_EQ(v, v_deser);

        std::unordered_map<std::string, int> m = {{"a", 1}, {"b", 2}};
        auto m_ser = cppbase::Serializer::SaveObjectToStringBinary(m);
        auto m_deser = cppbase::Serializer::LoadObjectFromStringBinary<std::unordered_map<std::string, int>>(m_ser);
        EXPECT_EQ(m, m_deser);

        std::list<int> l = {1, 2, 3};
        auto l_ser = cppbase::Serializer::SaveObjectToStringBinary(l);
        std::list<int> l_deser = cppbase::Serializer::LoadObjectFromStringBinary<std::list<int>>(l_ser);
        EXPECT_EQ(l, l_deser);

        uuids::uuid u = uuids::uuid::from_string("00000000-0000-0000-0000-000000000000").value();
        auto u_ser = cppbase::Serializer::SaveObjectToStringBinary(u);
        uuids::uuid u_deser = cppbase::Serializer::LoadObjectFromStringBinary<uuids::uuid>(u_ser);
        EXPECT_EQ(u, u_deser);

        Object obj;
        std::string archivePath = "./test.bin";
        cppbase::Serializer::SaveObjectToFileBinary(obj, archivePath);
        Object obj2 = cppbase::Serializer::LoadObjectFromFileBinary<Object>(archivePath);
        EXPECT_EQ(obj, obj2);
    }

    // external serialization function
    {
        std::shared_ptr<Object2> obj = std::make_shared<Object2>();
        obj->SetData(10);
        auto obj_ser = cppbase::Serializer::SaveObjectToStringBinary(obj);
        auto obj_deser = cppbase::Serializer::LoadObjectFromStringBinary<std::shared_ptr<Object2>>(obj_ser);
        EXPECT_EQ(*obj, *obj_deser);
    }
}
/**************************************************************************
 * @file: Archive.h
 * @brief:
 *
 * Copyright (c) 2022 O-Net Communications Inc.
 * All rights reserved.
 *************************************************************************/

#pragma once

#include <cereal/cereal.hpp>
#include <cereal/types/array.hpp>
#include <cereal/types/list.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/utility.hpp>
#include <cereal/types/vector.hpp>
// Include additional types as needed

#include <cereal/archives/portable_binary.hpp>
// #include <cereal/archives/json.hpp>
// #include <cereal/archives/xml.hpp>

#include <uuid.h>

#include "PropertyPath.h"
// Include serializable utilities

// There is no save/load functions for std::pair<> https://github.com/USCiLab/cereal/issues/547
namespace cereal {

template <class Archive>
void save(Archive& archive, const cppbase::PropertyPath& path, const uint32_t)
{
    archive(path.GetType().get_name().to_string(), path.GetRootType().get_name().to_string(),
            path.ToString());
}

template <class Archive>
void load(Archive& archive, cppbase::PropertyPath& path, const uint32_t)
{
    std::string type_name;
    std::string root_type_name;
    std::string path_str;
    archive(type_name, root_type_name, path_str);
    try
    {
        path = cppbase::PropertyPath(cppbase::Variant::Type::get_by_name(type_name.c_str()),
                                     path_str);
    } catch (const std::exception&)
    {
        path = cppbase::PropertyPath(cppbase::Variant::Type::get_by_name(root_type_name.c_str()),
                                     path_str);
    }
}

template <class Archive>
struct specialize<Archive, cppbase::PropertyPath, cereal::specialization::non_member_load_save>
{};

}  // namespace cereal

namespace cppbase {

using BinaryInputArchive = cereal::PortableBinaryInputArchive;
using BinaryOutputArchive = cereal::PortableBinaryOutputArchive;
// using JSONInputArchive = cereal::JSONInputArchive;
// using JSONOutputArchive = cereal::JSONOutputArchive;
// using XMLInputArchive = cereal::XMLInputArchive;
// using XMLOutputArchive = cereal::XMLOutputArchive;

}  // namespace cppbase

namespace uuids {

template <typename Archive>
void save(Archive& archive, const uuid& id, const uint32_t)
{
    archive(to_string(id));
}

template <typename Archive>
void load(Archive& archive, uuid& id, const uint32_t)
{
    std::string id_str;
    archive(id_str);
    id = uuid::from_string(id_str).value();
}

}  // namespace uuids

// Register a derived polymorphic type that needs to be serialized through a smart
// pointer to base classes, Note that base classes do not need to be registered.
#define REGISTER_TYPE(...) CEREAL_REGISTER_TYPE(__VA_ARGS__)

// When a registered type is renamed, use REGISTER_RENAMED_TYPE(T, old_name)
// to replace REGISTER_TYPE(T), Note that old_name should keep the same with
// the first version name even if the type is renamed again and again.
#define REGISTER_RENAMED_TYPE(T, N) CEREAL_REGISTER_TYPE_WITH_NAME(T, #N);

// SERIALIZE_BASE_CLASS and SERIALIZE_VIRTUAL_BASE_CLASS can automatically
// register base-derived relationship, if there is no need to use them, this
// explicit registration is required to register any possible base that may
// be used to store a Derived pointer for serialization
#define REGISTER_BASE(B, T) CEREAL_REGISTER_POLYMORPHIC_RELATION(B, T)

#define CLASS_VERSION(T, V) CEREAL_CLASS_VERSION(T, V)

// Calling cereal::base_class<T>(this) or cereal::virtual_base_class<T>(this)
// allows Cereal to track the inheritance. It is safe to use virtual_base_class
// in all circumstances, even in cases where virtual inheritance doesn't take
// place, though it may be slightly faster to use cereal::base_class<>, and
// we don't use virtual inheritance very often.
#define SERIALIZE_BASE_CLASS(A, T) A(cereal::base_class<T>(this))
#define SERIALIZE_VIRTUAL_BASE_CLASS(A, T) A(cereal::virtual_base_class<T>(this))

#define SERIALIZATION_FRIEND_ACCESS friend class cereal::access;

// When the inherited archive function is different from truely used one,
// such as Derived class is using save-load pair while base class achieved
// serialize function, call SPECIALIZE_SAVE_LOAD_ARCHIVE(Derived) to provide
// a disambiguation for cereal.
#define SPECIALIZE_SERIALIZE_ARCHIVE(T) \
    CEREAL_SPECIALIZE_FOR_ALL_ARCHIVES(T, cereal::specialization::member_serialize)
#define SPECIALIZE_SAVE_LOAD_ARCHIVE(T) \
    CEREAL_SPECIALIZE_FOR_ALL_ARCHIVES(T, cereal::specialization::member_load_save)

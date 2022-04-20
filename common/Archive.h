/**************************************************************************
 * @file: Archive.h
 * @brief:
 *
 * Copyright (c) 2022 PIQS Technologies.
 * All rights reserved.
 *************************************************************************/

#pragma once

#include <cereal/cereal.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/types/array.hpp>
#include <cereal/types/list.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/utility.hpp>
#include <cereal/types/vector.hpp>
// Include additional types as needed

#include <uuid.h>
// Include serializable utilities

namespace cppbase {

using BinaryInputArchive = cereal::PortableBinaryInputArchive;
using BinaryOutputArchive = cereal::PortableBinaryOutputArchive;
using JSONInputArchive = cereal::JSONInputArchive;
using JSONOutputArchive = cereal::JSONOutputArchive;
using XMLInputArchive = cereal::XMLInputArchive;
using XMLOutputArchive = cereal::XMLOutputArchive;

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

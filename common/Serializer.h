/**************************************************************************
 * @file: Serializer.h
 * @brief:
 *
 * Copyright (c) 2022 PIQS Technologies.
 * All rights reserved.
 *************************************************************************/

#pragma once

#include <fstream>
#include <sstream>
#include <string>

#include "Archive.h"

namespace cppbase {

class Serializer
{
public:
    Serializer() = default;
    virtual ~Serializer() = default;

    /**
     * @brief Serialize an object to an archive file.
     *
     * @param obj Object to be serialized
     * @param archive_path Path to the archive to be created
     */
    template <typename OutArchive, typename T>
    static void SaveObjectToFile(const T& obj, const std::string& archive_path)
    {
        std::ofstream ofs(archive_path, std::ios::binary);
        if (!ofs.is_open())
        {
            throw std::runtime_error("Failed to open file for writing: " + archive_path);
        }
        OutArchive arch(ofs);
        arch(obj);
    }

    /**
     * @brief Deserialize and object from an archive file
     *
     * @param archive_path Path to the archive to be loaded
     * @return The loaded object
     */
    template <typename InArchive, typename T>
    static T LoadObjectFromFile(const std::string& archive_path)
    {
        std::ifstream ifs(archive_path, std::ios::binary);
        if (!ifs.is_open())
        {
            throw std::runtime_error("Failed to open file for reading: " + archive_path);
        }
        InArchive arch(ifs);
        // std::shared_ptr<T> obj;
        T obj;
        arch(obj);
        return obj;
    }

    /**
     * @brief Serialize an object to a string
     *
     * @param obj Object to be serialized
     * @return std::string String representation of the object
     */
    template <typename OutArchive, typename T>
    static std::string SaveObjectToString(const T& obj)
    {
        std::stringstream ss;
        OutArchive arch(ss);
        arch(obj);
        return ss.str();
    }

    /**
     * @brief Deserialize an object from a string
     *
     * @param archive String representation of the object
     * @return The loaded object
     */
    template <typename InArchive, typename T>
    static T LoadObjectFromString(const std::string& archive)
    {
        std::stringstream ss;
        ss << archive;
        InArchive arch(ss);
        T obj;
        arch(obj);
        return obj;
    }

    /**
     * @brief Serialize an object to a binary archive file
     *
     * @param obj Object to be serialized
     * @param archive_path Path to the archive file to be created
     */
    template<typename T>
    static void SaveObjectToFileBinary(const T& obj, const std::string& archive_path)
    {
        SaveObjectToFile<BinaryOutputArchive, T>(obj, archive_path);
    }

    /**
     * @brief Serialize an object to a binary archive string
     *
     * @param obj Object to be serialized
     * @return std::string String that contains the archive data
     */
    template<typename T>
    static std::string SaveObjectToStringBinary(const T& obj)
    {
        return SaveObjectToString<BinaryOutputArchive, T>(obj);
    }

    /**
     * @brief Deserialize an object from a binary archive file
     *
     * @param archive_path Path to the archive file to be loaded
     * @return The loaded object
     */
    template<typename T>
    static T LoadObjectFromFileBinary(const std::string& archive_path)
    {
        return LoadObjectFromFile<BinaryInputArchive, T>(archive_path);
    }

    /**
     * @brief Deserialize an object from a binary archive string
     *
     * @param archive String that contains the archive data
     * @return The loaded object
     */
    template<typename T>
    static T LoadObjectFromStringBinary(const std::string& archive)
    {
        return LoadObjectFromString<BinaryInputArchive, T>(archive);
    }
};

}  // namespace cppbase

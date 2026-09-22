/**************************************************************************
 * @file:  FileSystem.h
 * @brief: std::filesystem adapter
 *
 * Copyright (c) 2022 O-Net Communications Inc.
 *************************************************************************/

#pragma once

#if defined(_WIN32)

#include <filesystem>
namespace fs = std::filesystem;

#elif __has_include(<filesystem>)

#include <filesystem>
namespace fs = std::filesystem;

#elif __has_include(<experimental/filesystem>)

#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;

#else

#error "no filesystem support!"

#endif

namespace cppbase { namespace filesystem {

constexpr const char LogConfigFilename[] = "log.conf";
constexpr const char ConfigFolderName[] = "config";

inline std::string GetConfigDir() noexcept
{
    auto path = fs::current_path() / ConfigFolderName;
    return path.string();
}

inline std::string GetWorkspaceDir() noexcept
{
    // For debug only
    return "./workspace";
}

inline std::string GetLogDir() noexcept
{
    // For debug only
    return "./log";
}

}}  // namespace cppbase::filesystem

/**************************************************************************
 * @file:  FileSyste.h
 * @brief:
 *
 * Copyright(c) 2020-present O-Net Communications (ShenZhen) Limited.
 * This code is licensed under MIT license (see LICENSE for details)
 *
 *************************************************************************/

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

std::string GetConfigDir() noexcept
{
    // For debug only
    return "./config";
}

std::string GetWorkspaceDir() noexcept
{
    // For debug only
    return "./workspace";
}

std::string GetLogDir() noexcept
{
    // For debug only
    return "./log";
}

}} // namespace cppbase::filesystem

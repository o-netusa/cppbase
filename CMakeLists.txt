cmake_minimum_required(VERSION 3.16)

project(cppbase CXX)

find_package(Threads)

set(CPPBASE_STANDALONE_PROJECT OFF)
if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(CPPBASE_STANDALONE_PROJECT ON)
endif()

option(CPPBASE_BUILD_TESTING   "Build unit tests" ${CPPBASE_STANDALONE_PROJECT})
option(CPPBASE_BUILD_ASIO      "Build Asio"        ON)
option(CPPBASE_BUILD_CXXOPTS   "Build cxxopts"     ON)
option(CPPBASE_BUILD_EIGEN     "Build Eigen"       ON)
option(CPPBASE_BUILD_FMT       "Build FMT"         ON)
option(CPPBASE_BUILD_GETTEXT   "Build gettext"     ON) # For Windows only
option(CPPBASE_BUILD_GTEST     "Build Google Test" ON)
option(CPPBASE_BUILD_RAPIDJSON "Build RapidJson"   ON)
option(CPPBASE_BUILD_RAPIDXML  "Build RapidXml"    ON)
option(CPPBASE_BUILD_SIGSLOT   "Build sigslot"     ON)
option(CPPBASE_BUILD_SPDLOG    "Build spdlog"      ON)
option(CPPBASE_BUILD_STDUUID   "Build stduuid"     ON)
option(CPPBASE_BUILD_YAML_CPP  "Build YAML-CPP"    ON)
option(CPPBASE_USE_RTTR        "Use RTTR library (default=ON)" ON)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CXX_STD cxx_std_17)

# Custom CMake modules/scripts
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
include(Common)
include(CompilerWarnings)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
if(UNIX)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
else()
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
endif()
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

# MODULE_NAME is for logger module name
add_compile_definitions(MODULE_NAME="cppbase")
if (CPPBASE_USE_RTTR)
  add_compile_definitions(USE_RTTR)
endif()

# WIN32_LEAN_AND_MEAN is for winsock.h has already been included error
# _SILENCE_CXX17_ALLOCATOR_VOID_DEPRECATION_WARNING is for warning STL4009
#   std::allocator<void> is deprecated in C++17
if(MSVC)
  add_compile_definitions(
    NOMINMAX
    WIN32_LEAN_AND_MEAN
    _SILENCE_CXX17_ALLOCATOR_VOID_DEPRECATION_WARNING
    _CRT_SECURE_NO_WARNINGS)
  add_compile_options("/wd4251" "/bigobj")
  add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
  add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
endif()

add_subdirectory(thirdparty)

if(CPPBASE_BUILD_TESTING)
  enable_testing()
  add_subdirectory(tests)
endif()

include(ClangFormat)
clang_format_recurse("common" "logging" "network" "tests")

install(FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/common/Global.h
    ${CMAKE_CURRENT_SOURCE_DIR}/common/Semaphore.h
    DESTINATION include/common
)

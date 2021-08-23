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
option(CPPBASE_BUILD_GTEST     "Build Google Test" ON)
option(CPPBASE_BUILD_RAPIDJSON "Build RapidJson"   ON)
option(CPPBASE_BUILD_RAPIDXML  "Build RapidXml"    ON)
option(CPPBASE_BUILD_SIGSLOT   "Build sigslot"     ON)
option(CPPBASE_BUILD_SPDLOG    "Build spdlog"      ON)
option(CPPBASE_BUILD_STDUUID   "Build stduuid"     ON)
option(CPPBASE_BUILD_ZEROMQ    "Build zeromq"      ON)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CXX_STD cxx_std_17)
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

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

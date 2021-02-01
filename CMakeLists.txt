cmake_minimum_required(VERSION 3.10)

project(cppbase CXX)

set(CPPBASE_STANDALONE_PROJECT OFF)
if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(CPPBASE_STANDALONE_PROJECT ON)
endif()

option(CPPBASE_BUILD_TESTING "Build unit tests" ${CPPBASE_STANDALONE_PROJECT})

# set(CMAKE_INCLUDE_CURRENT_DIR ON) # this causes capnproto build to fail
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set(BUILD_SHARED ON)
set(CXX_STD cxx_std_17)

# Custom CMake modules/scripts
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
include(Common)
include(UseGoldLinker)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
if(UNIX)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
else()
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
endif()
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

# MODULE_NAME is useful for logger naming
add_compile_definitions(MODULE_NAME="cppbase")

add_subdirectory(thirdparty)

if(CPPBASE_BUILD_TESTING)
  enable_testing()
  add_subdirectory(tests)
endif()

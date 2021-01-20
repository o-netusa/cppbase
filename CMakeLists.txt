cmake_minimum_required(VERSION 3.10)

project(cppbase CXX)

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

# Build unit tests for standalone build only
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
  enable_testing()
  add_subdirectory(tests)
endif()

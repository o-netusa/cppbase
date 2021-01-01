cmake_minimum_required(VERSION 3.10)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(BUILD_SHARED ON)
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

add_compile_definitions(MODULE_NAME="cppbase")

if (MSVC)
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

# -DWIN32_LEAN_AND_MEAN is for winsock.h has already been included error
# -D_SILENCE_CXX17_ALLOCATOR_VOID_DEPRECATION_WARNING is for warning STL4009
#   std::allocator<void> is deprecated in C++17
add_definitions(
    -D_WIN32_WINNT=0x0A00
    -DNOMINMAX
    -DWIN32_LEAN_AND_MEAN
    -D_SILENCE_CXX17_ALLOCATOR_VOID_DEPRECATION_WARNING
    -D_CRT_SECURE_NO_WARNINGS)
endif()

enable_testing()

add_subdirectory(thirdparty)
add_subdirectory(tests)

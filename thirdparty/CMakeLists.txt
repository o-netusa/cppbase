cmake_minimum_required(VERSION 3.16)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
include(DownloadProject)

message(STATUS "===============================")
message(STATUS "Thirdparty Libs (${CMAKE_BUILD_TYPE})")
message(STATUS "===============================")

##
# For encoding/decoding and client/server communications, following options
# have been considered, but no decision has been made yet:
# [2021/02/01] rttr: RTTR is a reflection library and could also be used for serialization
# because it indeed encodes/decodes types, and it seems planning to support JSON as well.
# capnproto: this is the most likely choice, and there is only some cmake issue unresolved for Windows
# nanopb: functionality is limited
# protobuf: client/server communication is limited to http2
# rpclib: inactive development
#

# ============================================================
#                        abseil
# ============================================================
#
# message("==> Configuring abseil")
# download_project(
#   PROJ     abseil
#   URL      ${CMAKE_CURRENT_SOURCE_DIR}/abseil/20210324.2.tar.gz
#   URL_HASH MD5=e0b585398b89ee92a10d8c68ef7eed48
#   QUIET
# )
# add_subdirectory(${abseil_SOURCE_DIR} ${abseil_BINARY_DIR})

# ============================================================
#                        asio
# ============================================================
#
if(CPPBASE_BUILD_ASIO)
message("==> Configuring asio")
download_project(
  PROJ           asio
  GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
  GIT_TAG        asio-1-23-0
  GIT_SHALLOW    ON
  QUIET
)
add_library(asio INTERFACE)
target_include_directories(asio INTERFACE ${asio_SOURCE_DIR}/asio/include)
install(DIRECTORY ${asio_SOURCE_DIR}/asio/include/asio DESTINATION include)
install(FILES ${asio_SOURCE_DIR}/asio/include/asio.hpp DESTINATION include)
endif()

# ============================================================
#                        cereal
# ============================================================
#
if(CPPBASE_BUILD_CEREAL)
message("==> Configuring cereal")
set(BUILD_DOC OFF CACHE BOOL "")
set(BUILD_SANDBOX OFF CACHE BOOL "")
set(SKIP_PERFORMANCE_COMPARISON ON CACHE BOOL "")
download_project(
  PROJ           cereal
  GIT_REPOSITORY https://github.com/USCiLab/cereal.git
  GIT_TAG        v1.3.2
  GIT_SHALLOW    ON
  QUIET
)
add_subdirectory(${cereal_SOURCE_DIR} ${cereal_BINARY_DIR})
endif()

# ============================================================
#                        cxxopts
# ============================================================
#
if(CPPBASE_BUILD_CXXOPTS)
message("==> Configuring cxxopts")
download_project(
  PROJ           cxxopts
  GIT_REPOSITORY https://github.com/jarro2783/cxxopts.git
  GIT_TAG        v3.0.0
  GIT_SHALLOW    ON
  QUIET
)
add_library(cxxopts INTERFACE)
target_include_directories(cxxopts INTERFACE ${cxxopts_SOURCE_DIR}/include)
install(FILES ${cxxopts_SOURCE_DIR}/include/cxxopts.hpp DESTINATION include)
endif()

# ============================================================
#                        eigen
# ============================================================
#
if(CPPBASE_BUILD_EIGEN)
message("==> Configuring eigen")
set(BUILD_TESTING OFF CACHE BOOL "")
set(EIGEN_BUILD_PKGCONFIG OFF CACHE BOOL "")
download_project(
  PROJ     eigen
  URL      https://gitlab.com/libeigen/eigen/-/archive/3.4.0/eigen-3.4.0.tar.gz
  URL_HASH MD5=4c527a9171d71a72a9d4186e65bea559
  QUIET
)
add_subdirectory(${eigen_SOURCE_DIR} ${eigen_BINARY_DIR})
endif()

# ============================================================
#                        fmt
# ============================================================
#
if(CPPBASE_BUILD_FMT)
message("==> Configuring fmt")
download_project(
  PROJ           fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG        9.0.0
  GIT_SHALLOW    ON
  QUIET
)
add_subdirectory(${fmt_SOURCE_DIR} ${fmt_BINARY_DIR})
endif()

# ============================================================
#                        gettext
# ============================================================
#
if(CPPBASE_BUILD_GETTEXT AND (CMAKE_SYSTEM_NAME MATCHES "Windows"))
message("==> Configuring gettext")
download_project(
  PROJ     gettext
  URL      https://ftp.gnu.org/gnu/gettext/gettext-0.21.tar.gz
  URL_HASH MD5=b8166ac41b4ef3adf127b3fa0445e099
  QUIET
)
add_library(gettext INTERFACE)
target_include_directories(gettext INTERFACE ${gettext_SOURCE_DIR}/include)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
target_link_libraries(gettext INTERFACE ${gettext_SOURCE_DIR}/lib/debug/libintl.lib)
set(gettext_LIB_DIR "${gettext_SOURCE_DIR}/lib/debug" CACHE PATH "")
else()
target_link_libraries(gettext INTERFACE ${gettext_SOURCE_DIR}/lib/release/libintl.lib)
set(gettext_LIB_DIR "${gettext_SOURCE_DIR}/lib/release" CACHE PATH "")
endif()
endif()

# ============================================================
#                        googletest
# ============================================================
#
if(CPPBASE_BUILD_GTEST)
message("==> Configuring googletest")
set(INSTALL_GMOCK OFF CACHE BOOL "")
set(INSTALL_GTEST OFF CACHE BOOL "")
set(gtest_force_shared_crt ON CACHE BOOL "" FORCED)

download_project(
  PROJ           googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.12.1
  GIT_SHALLOW    ON
  QUIET
)
add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
endif()

# ============================================================
#                        rapidjson
# ============================================================
#
if(CPPBASE_BUILD_RAPIDJSON)
message("==> Configuring rapidjson")
download_project(
  PROJ           rapidjson
  GIT_REPOSITORY https://github.com/Tencent/rapidjson.git
  GIT_TAG        master
  GIT_SHALLOW    ON
  QUIET
)
add_library(rapidjson INTERFACE)
target_include_directories(rapidjson INTERFACE ${rapidjson_SOURCE_DIR}/include)
endif()

# ============================================================
#                        rapidxml
# ============================================================
#
# if(CPPBASE_BUILD_RAPIDXML)
# message("==> Configuring rapidxml")
# download_project(
#   PROJ     rapidxml
#   URL      ${CMAKE_CURRENT_SOURCE_DIR}/rapidxml/rapidxml.zip
#   URL_HASH MD5=113b9040ccaba2cc652f8aeac7fd9536
#   QUIET
# )
# add_library(rapidxml INTERFACE)
# target_include_directories(rapidxml INTERFACE ${rapidxml_SOURCE_DIR})
# endif()

# ============================================================
#                        RTTR
# ============================================================
#
message(STATUS "Configuring RTTR")
set(BUILD_UNIT_TESTS    OFF CACHE BOOL "")
set(BUILD_EXAMPLES      OFF CACHE BOOL "")
set(BUILD_DOCUMENTATION OFF CACHE BOOL "")
set(BUILD_STATIC        OFF CACHE BOOL "")

download_project(
  PROJ           rttr
  GIT_REPOSITORY https://github.com/rttrorg/rttr.git
  GIT_TAG        master
  GIT_SHALLOW    ON
  QUIET
)
add_subdirectory(${rttr_SOURCE_DIR} ${rttr_BINARY_DIR})

# ============================================================
#                        spdlog
# ============================================================
#
if(CPPBASE_BUILD_SPDLOG)
message("==> Configuring spdlog")
set(SPDLOG_FMT_EXTERNAL_HO ON CACHE BOOL "")
download_project(
  PROJ           spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG        v1.10.0
  GIT_SHALLOW    ON
  QUIET
)
add_subdirectory(${spdlog_SOURCE_DIR} ${spdlog_BINARY_DIR})
endif()

# ============================================================
#                        stduuid
# ============================================================
#
if(CPPBASE_BUILD_STDUUID)
message("==> Configuring stduuid")
set(UUID_BUILD_TESTS OFF CACHE BOOL "")
download_project(
  PROJ           stduuid
  GIT_REPOSITORY https://github.com/mariusbancila/stduuid.git
  GIT_TAG        v1.2.2
  GIT_SHALLOW    ON
  QUIET
)
add_subdirectory(${stduuid_SOURCE_DIR} ${stduuid_BINARY_DIR})
endif()

# ============================================================
#                        TaskFlow
# ============================================================
#
message(STATUS "Configuring TaskFlow")
set(TF_BUILD_TESTS    OFF CACHE BOOL "")
set(TF_BUILD_EXAMPLES OFF CACHE BOOL "")
download_project(
  PROJ           taskflow
  GIT_REPOSITORY https://github.com/taskflow/taskflow.git
  GIT_TAG        v3.4.0
  GIT_SHALLOW    ON
  QUIET
)
add_subdirectory(${taskflow_SOURCE_DIR} ${taskflow_BINARY_DIR})

# ============================================================
#                        yaml-cpp
# ============================================================
#
if(CPPBASE_BUILD_YAML_CPP)
message("==> Configuring yaml-cpp")
set(UUID_BUILD_TESTS OFF CACHE BOOL "")
download_project(
  PROJ           yamlcpp
  GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
  GIT_TAG        yaml-cpp-0.7.0
  GIT_SHALLOW    ON
  QUIET
)
add_subdirectory(${yamlcpp_SOURCE_DIR} ${yamlcpp_BINARY_DIR})
endif()

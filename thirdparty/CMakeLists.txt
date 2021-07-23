cmake_minimum_required(VERSION 3.10...3.20 FATAL_ERROR)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
include(DownloadProject)

message(STATUS "===============================")
message(STATUS "Thirdparty Libs (${CMAKE_BUILD_TYPE})")
message(STATUS "===============================")

set(BUILD_TESTING OFF CACHE BOOL "")

# ============================================================
#                        asio
# ============================================================
#
message("==> Configuring asio")
download_project(
  PROJ     asio
  URL      ${CMAKE_CURRENT_SOURCE_DIR}/asio/asio-1.18.0.tar.gz
  URL_HASH MD5=a6f23c10db1d2a22c813e21f978e5049
  QUIET
)
add_library(asio INTERFACE)
target_include_directories(asio INTERFACE ${asio_SOURCE_DIR}/include)

##
# For encoding/decoding and client/server communications, following options
# have been considered, but no decision has been made yet:
# [2021/02/01] rttr: RTTR is a reflection library and could also be used for serialization
# because it indeed encodes/decodes types, and it seems planning to support JSON as well.
# capnproto: this is the most likely choice, and there is only some cmake issue unresolved for Windows
# nanopb: functionality is limited
# protobuf: client/server communication is limited to http2
# rpclib: inactive development

# ============================================================
#                        cxxopts
# ============================================================
#
message("==> Configuring cxxopts")
download_project(
  PROJ     cxxopts
  URL      ${CMAKE_CURRENT_SOURCE_DIR}/cxxopts/cxxopts-2.2.1.tar.gz
  URL_HASH MD5=6e70da4fc17a09f32612443f1866042e
  QUIET
)
add_library(cxxopts INTERFACE)
target_include_directories(cxxopts INTERFACE ${cxxopts_SOURCE_DIR}/include)

# ============================================================
#                        eigen
# ============================================================
#
message("==> Configuring eigen")
download_project(
  PROJ     eigen
  URL      ${CMAKE_CURRENT_SOURCE_DIR}/eigen/eigen-v2.8.5.tar.gz
  URL_HASH MD5=8115d08b9089fe861b9288d9facd3bdd
  QUIET
)
add_subdirectory(${eigen_SOURCE_DIR} ${eigen_BINARY_DIR})

# ============================================================
#                        fmt
# ============================================================
#
message("==> Configuring fmt")
set(BUILD_SHARED_LIBS ON CACHE BOOL "")
download_project(
  PROJ     fmt
  URL      ${CMAKE_CURRENT_SOURCE_DIR}/fmt/6.2.1.tar.gz
  URL_HASH MD5=f77569874b25ff3ece7f0229a4236feb
  QUIET
)
add_subdirectory(${fmt_SOURCE_DIR} ${fmt_BINARY_DIR})

# ============================================================
#                        googletest
# ============================================================
#
if(CPPBASE_BUILD_GTEST)
message("==> Configuring googletest")
set(BUILD_SHARED_LIBS ON CACHE BOOL "")
set(INSTALL_GMOCK OFF CACHE BOOL "")
set(INSTALL_GTEST OFF CACHE BOOL "")
set(gtest_force_shared_crt ON CACHE BOOL "" FORCED)

download_project(
  PROJ     googletest
  URL      ${CMAKE_CURRENT_SOURCE_DIR}/googletest/googletest-release-1.10.0.tar.gz
  URL_HASH MD5=ecd1fa65e7de707cd5c00bdac56022cd
  QUIET
)
add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
endif()

# ============================================================
#                        rapidjson
# ============================================================
#
message("==> Configuring rapidjson")
download_project(
  PROJ     rapidjson
  URL      ${CMAKE_CURRENT_SOURCE_DIR}/rapidjson/rapidjson-master.zip
  URL_HASH MD5=fb721489c7f4aa384372050993e37d1d
  QUIET
)
add_library(rapidjson INTERFACE)
target_include_directories(rapidjson INTERFACE ${rapidjson_SOURCE_DIR}/include)

# ============================================================
#                        rapidxml
# ============================================================
#
message("==> Configuring rapidxml")
download_project(
  PROJ     rapidxml
  URL      ${CMAKE_CURRENT_SOURCE_DIR}/rapidxml/rapidxml.zip
  URL_HASH MD5=113b9040ccaba2cc652f8aeac7fd9536
  QUIET
)
add_library(rapidxml INTERFACE)
target_include_directories(rapidxml INTERFACE ${rapidxml_SOURCE_DIR})

# ============================================================
#                        sigslot
# ============================================================
#
message("==> Configuring sigslot")
download_project(
  PROJ     sigslot
  URL      ${CMAKE_CURRENT_SOURCE_DIR}/sigslot/sigslot-master.zip
  URL_HASH MD5=209a71004526e5d66025ab233200da0f
  QUIET
)
add_library(sigslot INTERFACE)
target_include_directories(sigslot INTERFACE ${sigslot_SOURCE_DIR}/include)

# ============================================================
#                        spdlog
# ============================================================
#
message("==> Configuring spdlog")
set(SPDLOG_FMT_EXTERNAL_HO ON CACHE BOOL "")
download_project(
  PROJ     spdlog
  URL      ${CMAKE_CURRENT_SOURCE_DIR}/spdlog/v1.8.5.tar.gz
  URL_HASH MD5=8755cdbc857794730a022722a66d431a
  QUIET
)
add_subdirectory(${spdlog_SOURCE_DIR} ${spdlog_BINARY_DIR})

# ============================================================
#                        stduuid
# ============================================================
#
message("==> Configuring stduuid")
set(UUID_BUILD_TESTS OFF CACHE BOOL "")
download_project(
  PROJ     stduuid
  URL      ${CMAKE_CURRENT_SOURCE_DIR}/stduuid/stduuid-1.0.tar.gz
  URL_HASH MD5=05ac92614b195dee6b1af0ba04684c57
  QUIET
)
add_library(stduuid INTERFACE)
target_include_directories(stduuid INTERFACE ${stduuid_SOURCE_DIR}/include)
install(DIRECTORY ${stduuid_SOURCE_DIR}/include/gsl DESTINATION include)
install(FILES ${stduuid_SOURCE_DIR}/include/uuid.h DESTINATION include)

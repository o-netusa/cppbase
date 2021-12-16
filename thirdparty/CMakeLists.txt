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
  PROJ     asio
  URL      ${CMAKE_CURRENT_SOURCE_DIR}/asio/asio-1.18.0.tar.gz
  URL_HASH MD5=a6f23c10db1d2a22c813e21f978e5049
  QUIET
)
add_library(asio INTERFACE)
target_include_directories(asio INTERFACE ${asio_SOURCE_DIR}/include)
endif()

# ============================================================
#                        cxxopts
# ============================================================
#
if(CPPBASE_BUILD_CXXOPTS)
message("==> Configuring cxxopts")
download_project(
  PROJ     cxxopts
  URL      ${CMAKE_CURRENT_SOURCE_DIR}/cxxopts/cxxopts-2.2.1.tar.gz
  URL_HASH MD5=6e70da4fc17a09f32612443f1866042e
  QUIET
)
add_library(cxxopts INTERFACE)
target_include_directories(cxxopts INTERFACE ${cxxopts_SOURCE_DIR}/include)
endif()

# ============================================================
#                        eigen
# ============================================================
#
if(CPPBASE_BUILD_EIGEN)
message("==> Configuring eigen")
download_project(
  PROJ     eigen
  URL      ${CMAKE_CURRENT_SOURCE_DIR}/eigen/eigen-3.3.9.tar.gz
  URL_HASH MD5=609286804b0f79be622ccf7f9ff2b660
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
  PROJ     fmt
  URL      ${CMAKE_CURRENT_SOURCE_DIR}/fmt/6.2.1.tar.gz
  URL_HASH MD5=f77569874b25ff3ece7f0229a4236feb
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
  URL      ${CMAKE_CURRENT_SOURCE_DIR}/gettext/gettext-0.21.tar.gz
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
  PROJ     googletest
  URL      ${CMAKE_CURRENT_SOURCE_DIR}/googletest/release-1.11.0.tar.gz
  URL_HASH MD5=e8a8df240b6938bb6384155d4c37d937
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
  PROJ     rapidjson
  URL      ${CMAKE_CURRENT_SOURCE_DIR}/rapidjson/rapidjson-master.zip
  URL_HASH MD5=fb721489c7f4aa384372050993e37d1d
  QUIET
)
add_library(rapidjson INTERFACE)
target_include_directories(rapidjson INTERFACE ${rapidjson_SOURCE_DIR}/include)
endif()

# ============================================================
#                        rapidxml
# ============================================================
#
if(CPPBASE_BUILD_RAPIDXML)
message("==> Configuring rapidxml")
download_project(
  PROJ     rapidxml
  URL      ${CMAKE_CURRENT_SOURCE_DIR}/rapidxml/rapidxml.zip
  URL_HASH MD5=113b9040ccaba2cc652f8aeac7fd9536
  QUIET
)
add_library(rapidxml INTERFACE)
target_include_directories(rapidxml INTERFACE ${rapidxml_SOURCE_DIR})
endif()

# ============================================================
#                        RTTR
# ============================================================
#
message(STATUS "Configuring RTTR")
set(BUILD_UNIT_TESTS    OFF CACHE BOOL "")
set(BUILD_EXAMPLES      OFF CACHE BOOL "")
set(BUILD_DOCUMENTATION OFF CACHE BOOL "")
set(BUILD_INSTALLER     OFF CACHE BOOL "") # Install rttr dll
set(BUILD_PACKAGE       OFF CACHE BOOL "")
set(BUILD_STATIC        OFF CACHE BOOL "")

download_project(
  PROJ     rttr
  URL      ${CMAKE_CURRENT_SOURCE_DIR}/rttr/rttr-master.zip
  URL_HASH MD5=78d80e22d52a460807812ffec9a88599
  QUIET
)
add_subdirectory(${rttr_SOURCE_DIR} ${rttr_BINARY_DIR})

# ============================================================
#                        sigslot
# ============================================================
#
if(CPPBASE_BUILD_SIGSLOT)
message("==> Configuring sigslot")
download_project(
  PROJ     sigslot
  URL      ${CMAKE_CURRENT_SOURCE_DIR}/sigslot/sigslot-master.zip
  URL_HASH MD5=209a71004526e5d66025ab233200da0f
  QUIET
)
add_library(sigslot INTERFACE)
target_include_directories(sigslot INTERFACE ${sigslot_SOURCE_DIR}/include)
endif()

# ============================================================
#                        spdlog
# ============================================================
#
if(CPPBASE_BUILD_SPDLOG)
message("==> Configuring spdlog")
set(SPDLOG_FMT_EXTERNAL_HO ON CACHE BOOL "")
download_project(
  PROJ     spdlog
  URL      ${CMAKE_CURRENT_SOURCE_DIR}/spdlog/v1.8.5.tar.gz
  URL_HASH MD5=8755cdbc857794730a022722a66d431a
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
  PROJ     stduuid
  URL      ${CMAKE_CURRENT_SOURCE_DIR}/stduuid/stduuid-1.0.tar.gz
  URL_HASH MD5=05ac92614b195dee6b1af0ba04684c57
  QUIET
)
add_library(stduuid INTERFACE)
target_include_directories(stduuid INTERFACE ${stduuid_SOURCE_DIR}/include)
# install(DIRECTORY ${stduuid_SOURCE_DIR}/include/gsl DESTINATION include)
# install(FILES ${stduuid_SOURCE_DIR}/include/uuid.h DESTINATION include)
endif()

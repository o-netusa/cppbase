cmake_minimum_required(VERSION 3.10)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
include(DownloadProject)

message(STATUS "===============================")
message(STATUS "Thirdparty Libs (${CMAKE_BUILD_TYPE})")
message(STATUS "===============================")

set(BUILD_TESTING OFF CACHE BOOL "")

# Many libs may depend on zlib, so put it at the beginning
# ============================================================
#                        zlib
# ============================================================
#
message("Configuring zlib")
download_project(
  PROJ     zlib
  URL      ${CMAKE_CURRENT_SOURCE_DIR}/zlib/zlib-1.2.11.tar.gz
  URL_HASH MD5=5cd021884a9e9aca588a427ad096662b
  QUIET
)
add_subdirectory(${zlib_SOURCE_DIR} ${zlib_BINARY_DIR})


# ============================================================
#                        asio
# ============================================================
#
message("Configuring asio")
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
# capnproto: this is the most likely choice, and there is only some cmake issue unresolved for Windows
# nanopb: functionality is limited
# protobuf: client/server communication is limited to http2
# rpclib: inactive development

if(UNIX)
# ============================================================
#                        capnproto
# ============================================================
#
message("Building capnproto")
download_project(
  PROJ     capnproto
  URL      ${CMAKE_CURRENT_SOURCE_DIR}/capnproto/capnproto-0.8.0.tar.gz
  URL_HASH MD5=1601022b45a083e7f0f8803e50f7e06b
  QUIET
)
add_subdirectory(${capnproto_SOURCE_DIR} ${capnproto_BINARY_DIR})
endif()

# ============================================================
#                        fmt
# ============================================================
#
message("Configuring fmt")
download_project(
  PROJ     fmt
  URL      ${CMAKE_CURRENT_SOURCE_DIR}/fmt/fmt-6.2.0.tar.gz
  URL_HASH MD5=0a7d2dfb5d0877e88c0d7c389408b369
  QUIET
)
add_subdirectory(${fmt_SOURCE_DIR} ${fmt_BINARY_DIR})

# ============================================================
#                        googletest
# ============================================================
#
message("Configuring googletest")
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

# ============================================================
#                        libcurl
# ============================================================
#
# MESSAGE("Configuring libcurl")
# set(BUILD_CURL_EXE      OFF CACHE BOOL "")
# set(ENABLE_MANUAL       OFF CACHE BOOL "")
# set(CURL_CA_PATH_SET    OFF CACHE BOOL "")
# set(CMAKE_USE_OPENSSL   OFF CACHE BOOL "")
# set(CURL_DISABLE_TELNET ON  CACHE BOOL "")
# set(CURL_DISABLE_DICT   ON  CACHE BOOL "")
# set(CURL_DISABLE_FILE   ON  CACHE BOOL "")
# set(CURL_DISABLE_TFTP   ON  CACHE BOOL "")
# set(CURL_DISABLE_LDAP   ON  CACHE BOOL "")
# set(CURL_DISABLE_LDAPS  ON  CACHE BOOL "")
# set(CURL_DISABLE_HTTP   ON  CACHE BOOL "")
# set(CURL_DISABLE_RTSP   ON  CACHE BOOL "")
# set(CURL_DISABLE_PROXY  ON  CACHE BOOL "")
# set(CURL_DISABLE_POP3   ON  CACHE BOOL "")
# set(CURL_DISABLE_IMAP   ON  CACHE BOOL "")
# set(CURL_DISABLE_SMTP   ON  CACHE BOOL "")
# set(CURL_DISABLE_GOPHER ON  CACHE BOOL "")

# download_project(
#   PROJ     curl
#   URL      ${CMAKE_CURRENT_SOURCE_DIR}/curl/curl-7_74_0.tar.gz
#   URL_HASH MD5=f9eb1ef3867584da7c8dc5f57ed4d95f
#   QUIET
# )
# add_subdirectory(${curl_SOURCE_DIR} ${curl_BINARY_DIR})

# ============================================================
#                        rapidjson
# ============================================================
#
message("Configuring rapidjson")
download_project(
  PROJ     rapidjson
  URL      ${CMAKE_CURRENT_SOURCE_DIR}/rapidjson/rapidjson-master.zip
  URL_HASH MD5=fb721489c7f4aa384372050993e37d1d
  QUIET
)
add_subdirectory(${rapidjson_SOURCE_DIR} ${rapidjson_BINARY_DIR})

# ============================================================
#                        sigslot
# ============================================================
#
message("Configuring sigslot")
set(SIGSLOT_COMPILE_EXAMPLES OFF CACHE BOOL "")
set(SIGSLOT_COMPILE_TESTS    OFF CACHE BOOL "")
download_project(
  PROJ     sigslot
  URL      ${CMAKE_CURRENT_SOURCE_DIR}/sigslot/sigslot-master.zip
  URL_HASH MD5=209a71004526e5d66025ab233200da0f
  QUIET
)
add_subdirectory(${sigslot_SOURCE_DIR} ${sigslot_BINARY_DIR})

# ============================================================
#                        spdlog
# ============================================================
#
message("Configuring spdlog")
set(SPDLOG_FMT_EXTERNAL ON CACHE BOOL "")
download_project(
  PROJ     spdlog
  URL      ${CMAKE_CURRENT_SOURCE_DIR}/spdlog/v1.8.2.tar.gz
  URL_HASH MD5=22518fb28d4be66c92a703c67d99b1d1
  QUIET
)
add_subdirectory(${spdlog_SOURCE_DIR} ${spdlog_BINARY_DIR})

# ============================================================
#                        stduuid
# ============================================================
#
message("Configuring stduuid")
set(UUID_BUILD_TESTS OFF CACHE BOOL "")
download_project(
  PROJ     stduuid
  URL      ${CMAKE_CURRENT_SOURCE_DIR}/stduuid/stduuid-master.zip
  URL_HASH MD5=aae5218f4e0e8be3e1a48f2de520ef81
  QUIET
)
add_library(stduuid INTERFACE)
target_include_directories(stduuid INTERFACE
  ${stduuid_SOURCE_DIR}/include
  ${stduuid_SOURCE_DIR}/gsl)

if (FALSE)
# ============================================================
#                        vld
# ============================================================
#
message("Configuring vld")
set(VLD_FMT_EXTERNAL ON CACHE BOOL "")
download_project(
  PROJ     vld
  URL      ${CMAKE_CURRENT_SOURCE_DIR}/vld/2.5.2.tar.gz
  URL_HASH MD5=daa96b5e7427411ba7258fcf8e72df92
  QUIET
)
add_subdirectory(${vld_SOURCE_DIR} ${vld_BINARY_DIR})
endif()

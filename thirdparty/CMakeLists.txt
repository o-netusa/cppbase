cmake_minimum_required(VERSION 3.10)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
include(DownloadProject)

MESSAGE(STATUS "===============================")
MESSAGE(STATUS "Thirdparty Libs (${CMAKE_BUILD_TYPE})")
MESSAGE(STATUS "===============================")

# ============================================================
#                        asio
# ============================================================
#
download_project(
  PROJ     asio
  URL      ${CMAKE_CURRENT_SOURCE_DIR}/asio/asio-1.18.0.tar.gz
  URL_HASH MD5=a6f23c10db1d2a22c813e21f978e5049
  QUIET
)
add_library(asio INTERFACE)
target_include_directories(asio INTERFACE ${asio_SOURCE_DIR}/include)

# ============================================================
#                        fmt
# ============================================================
#
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
set(BUILD_SHARED_LIBS ON CACHE BOOL "")
set(INSTALL_GMOCK ON CACHE BOOL "")
set(INSTALL_GTEST ON CACHE BOOL "")
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
set(BUILD_CURL_EXE      OFF CACHE BOOL "")
set(BUILD_TESTING       OFF CACHE BOOL "")
set(ENABLE_MANUAL       OFF CACHE BOOL "")
set(CURL_CA_PATH_SET    OFF CACHE BOOL "")
set(CMAKE_USE_OPENSSL   OFF CACHE BOOL "")
set(CURL_DISABLE_TELNET ON  CACHE BOOL "")
set(CURL_DISABLE_DICT   ON  CACHE BOOL "")
set(CURL_DISABLE_FILE   ON  CACHE BOOL "")
set(CURL_DISABLE_TFTP   ON  CACHE BOOL "")
set(CURL_DISABLE_LDAP   ON  CACHE BOOL "")
set(CURL_DISABLE_LDAPS  ON  CACHE BOOL "")
set(CURL_DISABLE_HTTP   ON  CACHE BOOL "")
set(CURL_DISABLE_RTSP   ON  CACHE BOOL "")
set(CURL_DISABLE_PROXY  ON  CACHE BOOL "")
set(CURL_DISABLE_POP3   ON  CACHE BOOL "")
set(CURL_DISABLE_IMAP   ON  CACHE BOOL "")
set(CURL_DISABLE_SMTP   ON  CACHE BOOL "")
set(CURL_DISABLE_GOPHER ON  CACHE BOOL "")

# download_project(
#   PROJ     curl
#   URL      ${CMAKE_CURRENT_SOURCE_DIR}/curl/curl-7_74_0.tar.gz
#   URL_HASH MD5=f9eb1ef3867584da7c8dc5f57ed4d95f
#   QUIET
# )
# add_subdirectory(${curl_SOURCE_DIR} ${curl_BINARY_DIR})

# ============================================================
#                        sigslot
# ============================================================
#
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
download_project(
  PROJ     stduuid
  URL      ${CMAKE_CURRENT_SOURCE_DIR}/stduuid/stduuid-master.zip
  URL_HASH MD5=aae5218f4e0e8be3e1a48f2de520ef81
  QUIET
)
add_library(stduuid INTERFACE)
target_include_directories(stduuid INTERFACE ${stduuid_SOURCE_DIR}/include)

if (FALSE)
# ============================================================
#                        vld
# ============================================================
#
set(VLD_FMT_EXTERNAL ON CACHE BOOL "")
download_project(
  PROJ     vld
  URL      ${CMAKE_CURRENT_SOURCE_DIR}/vld/2.5.2.tar.gz
  URL_HASH MD5=daa96b5e7427411ba7258fcf8e72df92
  QUIET
)
add_subdirectory(${vld_SOURCE_DIR} ${vld_BINARY_DIR})
endif()

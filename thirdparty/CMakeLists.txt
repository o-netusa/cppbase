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
  URL      ${CMAKE_CURRENT_SOURCE_DIR}/asio/asio-1-22-0.tar.gz
  URL_HASH MD5=7f0658ace1fb6416c85a0999027346b2
  QUIET
)
add_library(asio INTERFACE)
target_include_directories(asio INTERFACE ${asio_SOURCE_DIR}/asio/include)
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
  PROJ     cereal
  URL      ${CMAKE_CURRENT_SOURCE_DIR}/cereal/v1.3.2.tar.gz
  URL_HASH MD5=ab6070fadc7c50072ef4153fb1c46a87
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
  PROJ     cxxopts
  URL      ${CMAKE_CURRENT_SOURCE_DIR}/cxxopts/v3.0.0.tar.gz
  URL_HASH MD5=4c4cb6e2f252157d096fe18451ab451e
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
set(BUILD_TESTING OFF CACHE BOOL "")
set(EIGEN_BUILD_PKGCONFIG OFF CACHE BOOL "")
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
  URL      ${CMAKE_CURRENT_SOURCE_DIR}/fmt/8.1.1.tar.gz
  URL_HASH MD5=fe7f1585844b14c647bd332ad5562832
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
install(TARGETS rttr_core DESTINATION $<$<PLATFORM_ID:Windows>:bin>$<$<PLATFORM_ID:Linux>:lib>)

# ============================================================
#                        spdlog
# ============================================================
#
if(CPPBASE_BUILD_SPDLOG)
message("==> Configuring spdlog")
set(SPDLOG_FMT_EXTERNAL_HO ON CACHE BOOL "")
download_project(
  PROJ     spdlog
  URL      ${CMAKE_CURRENT_SOURCE_DIR}/spdlog/v1.10.0.tar.gz
  URL_HASH MD5=effea813cd81cfb5588806c5754e14f1
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
  URL      ${CMAKE_CURRENT_SOURCE_DIR}/stduuid/v1.2.2.tar.gz
  URL_HASH MD5=2d85213945c231bcfdf40b382ed41620
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
  PROJ     taskflow
  URL      ${CMAKE_CURRENT_SOURCE_DIR}/taskflow/v3.1.0.tar.gz
  URL_HASH MD5=de98b4ac60352f2bdc259d135a6c9594
  UPDATE_DISCONNECTED TRUE
  QUIET
)
add_library(taskflow INTERFACE)
target_include_directories(taskflow INTERFACE ${taskflow_SOURCE_DIR})

# ============================================================
#                        yaml-cpp
# ============================================================
#
if(CPPBASE_BUILD_YAML_CPP)
message("==> Configuring yaml-cpp")
set(UUID_BUILD_TESTS OFF CACHE BOOL "")
download_project(
  PROJ     yamlcpp
  URL      ${CMAKE_CURRENT_SOURCE_DIR}/yamlcpp/yaml-cpp-0.7.0.tar.gz
  URL_HASH MD5=74d646a3cc1b5d519829441db96744f0
  QUIET
)
add_subdirectory(${yamlcpp_SOURCE_DIR} ${yamlcpp_BINARY_DIR})
endif()

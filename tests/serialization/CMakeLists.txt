cmake_minimum_required(VERSION 3.10)

include(CapnProto)
CAPNP_GENERATE_CPP(addressbookSources addressbookHeaders addressbook.capnp)

set(SOURCE
  main.cpp
  SerializationTests.cpp
  ${addressbookSources}
)
add_executable(serializaton_test ${SOURCE})

target_include_directories(serializaton_test PUBLIC
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_BINARY_DIR}/capnproto-src/c++/src
  ${CMAKE_CURRENT_BINARY_DIR}
)

# set_target_warnings_as_error(TARGET serializaton_test)
target_compile_features(serializaton_test PRIVATE ${CXX_STD})

target_link_libraries(serializaton_test gtest CapnProto::capnp CapnProto::kj capnpc)
if(UNIX)
  target_link_libraries(serializaton_test
      $<$<AND:$<CXX_COMPILER_ID:GNU>,$<VERSION_LESS:$<CXX_COMPILER_VERSION>,9.0>>:stdc++fs>)
endif()

include(CTest)
include(GoogleTest)
gtest_discover_tests(serializaton_test)

cmake_minimum_required(VERSION 3.16)

set(SOURCE
  ../main.cpp
  LoggingTests.cpp
)

add_executable(logging_test ${SOURCE})

target_include_directories(logging_test PUBLIC
  ${CMAKE_SOURCE_DIR}
)

# set_target_warnings_as_error(TARGET logging_test)
target_compile_features(logging_test PRIVATE ${CXX_STD})

target_link_libraries(logging_test gtest spdlog_header_only)
if(UNIX)
  target_link_libraries(logging_test
      $<$<AND:$<CXX_COMPILER_ID:GNU>,$<VERSION_LESS:$<CXX_COMPILER_VERSION>,9.0>>:stdc++fs>)
endif()

include(CTest)

include(GoogleTest)
gtest_discover_tests(logging_test)

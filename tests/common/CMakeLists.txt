cmake_minimum_required(VERSION 3.10)

set(SOURCE
  main.cpp
  BlockingQueueTests.cpp
  FlagsTests.cpp
  MemoryLeaksTests.cpp
  TimerTests.cpp
  ThreadPoolTests.cpp
)

add_executable(common_test ${SOURCE})

target_include_directories(common_test PUBLIC
  ${CMAKE_SOURCE_DIR}
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:include>
)

# set_target_warnings_as_error(TARGET common_test)
target_compile_features(common_test PRIVATE ${CXX_STD})

target_link_libraries(common_test gtest spdlog)
if(UNIX)
  target_link_libraries(common_test
      $<$<AND:$<CXX_COMPILER_ID:GNU>,$<VERSION_LESS:$<CXX_COMPILER_VERSION>,9.0>>:stdc++fs>)
endif()

include(CTest)

include(GoogleTest)
gtest_discover_tests(common_test)

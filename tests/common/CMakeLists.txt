cmake_minimum_required(VERSION 3.10...3.20 FATAL_ERROR)

set(SOURCE
  main.cpp
  BlockingQueueTests.cpp
  MemoryLeaksTests.cpp
  TimerTests.cpp
  ThreadPoolTests.cpp
  SemaphoreTests.cpp
  UuidTests.cpp
)

add_executable(common_test ${SOURCE})

target_include_directories(common_test PUBLIC
  ${CMAKE_SOURCE_DIR}
)

# set_target_warnings_as_error(TARGET common_test)
target_compile_features(common_test PRIVATE ${CXX_STD})

target_link_libraries(common_test gtest spdlog stduuid)
if(UNIX)
  target_link_libraries(common_test
      $<$<AND:$<CXX_COMPILER_ID:GNU>,$<VERSION_LESS:$<CXX_COMPILER_VERSION>,9.0>>:stdc++fs>)
endif()

include(CTest)

include(GoogleTest)
gtest_discover_tests(common_test)

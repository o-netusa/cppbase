cmake_minimum_required(VERSION 3.16)

set(SOURCE
  ../main.cpp
  BlockingQueueTests.cpp
  MemoryLeaksTests.cpp
  TimerTests.cpp
  ThreadPoolTests.cpp
  SemaphoreTests.cpp
  UuidTests.cpp
  EigenTests.cpp
  VariantTests.cpp
  ForkJoinPoolTests.cpp
)

add_executable(common_test ${SOURCE})

target_include_directories(common_test PUBLIC
  ${CMAKE_SOURCE_DIR}
)

# set_target_warnings_as_error(TARGET common_test)
target_compile_features(common_test PRIVATE ${CXX_STD})

target_link_libraries(common_test gtest asio spdlog_header_only stduuid eigen)
target_link_libraries(common_test $<$<AND:$<CXX_COMPILER_ID:GNU>,$<VERSION_LESS:$<CXX_COMPILER_VERSION>,9.0>>:stdc++fs>)
if (CPPBASE_USE_RTTR)
  target_link_libraries(common_test rttr_core)
  add_custom_command(TARGET common_test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:rttr_core> $<TARGET_FILE_DIR:common_test>)
endif()

include(CTest)

include(GoogleTest)
gtest_discover_tests(common_test)

cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

include(CTest)
include(GoogleTest)

##################################################################################################
add_executable(processor_test
  ProcessorTests.cpp
  ../main.cpp
)
target_include_directories(processor_test PUBLIC
  ${CMAKE_SOURCE_DIR}
)

# set_target_warnings_as_error(TARGET processor_test)
target_compile_features(processor_test PRIVATE ${CXX_STD})
target_link_libraries(processor_test cereal gtest spdlog_header_only stduuid rttr_core)
add_custom_command(TARGET processor_test POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:rttr_core> $<TARGET_FILE_DIR:processor_test>
)

gtest_discover_tests(processor_test)

##################################################################################################
add_executable(sequence_test
  SequenceTests.cpp
  ../main.cpp
)
target_include_directories(sequence_test PUBLIC
  ${CMAKE_SOURCE_DIR}
)

# set_target_warnings_as_error(TARGET sequence_test)
target_compile_features(sequence_test PRIVATE ${CXX_STD})
target_link_libraries(sequence_test gtest cereal spdlog_header_only stduuid Taskflow rttr_core)

gtest_discover_tests(sequence_test)

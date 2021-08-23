cmake_minimum_required(VERSION 3.16)

set(SOURCE
  main.cpp
  TcpTests.cpp
  UdpTests.cpp
)

add_executable(network_test ${SOURCE})

target_include_directories(network_test PUBLIC
  ${CMAKE_SOURCE_DIR}
)

# set_target_warnings_as_error(TARGET network_test)
target_compile_features(network_test PRIVATE ${CXX_STD})

target_link_libraries(network_test gtest asio spdlog_header_only)
target_link_libraries(network_test
  $<$<AND:$<CXX_COMPILER_ID:GNU>,$<VERSION_LESS:$<CXX_COMPILER_VERSION>,9.0>>:stdc++fs>)

include(CTest)
include(GoogleTest)
gtest_discover_tests(network_test)

set(ZMQTESTS_SOURCE
  main.cpp
  ZMQTests.cpp
)

add_executable(zmq_test ${ZMQTESTS_SOURCE})

target_include_directories(zmq_test PUBLIC
  ${CMAKE_SOURCE_DIR}
)

# set_target_warnings_as_error(TARGET network_test)
target_compile_features(zmq_test PRIVATE ${CXX_STD})

target_link_libraries(zmq_test gtest spdlog_header_only cppzmq)

gtest_discover_tests(zmq_test)

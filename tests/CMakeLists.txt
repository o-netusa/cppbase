add_subdirectory(common)
add_subdirectory(logging)
if(CPPBASE_BUILD_ASIO)
  add_subdirectory(network)
endif()
add_subdirectory(sequence)

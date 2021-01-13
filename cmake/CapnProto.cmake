add_executable(CapnProto::capnp_tool IMPORTED)
set_target_properties(CapnProto::capnp_tool PROPERTIES
  IMPORTED_LOCATION "${CMAKE_BINARY_DIR}/bin/capnp${CMAKE_EXECUTABLE_SUFFIX}"
)

add_executable(CapnProto::capnpc_cpp IMPORTED)
set_target_properties(CapnProto::capnpc_cpp PROPERTIES
  IMPORTED_LOCATION "${CMAKE_BINARY_DIR}/bin/capnpc-c++${CMAKE_EXECUTABLE_SUFFIX}"
)

add_executable(CapnProto::capnpc_capnp IMPORTED)
set_target_properties(CapnProto::capnpc_capnp PROPERTIES
  IMPORTED_LOCATION "${CMAKE_BINARY_DIR}/bin/capnpc-capnp${CMAKE_EXECUTABLE_SUFFIX}"
)

include("${CMAKE_BINARY_DIR}/capnproto-build/c++/cmake/CapnProtoMacros.cmake")
if (NOT ${CAPNP_EXECUTABLE} STREQUAL "")
message("capnp executable is ${CAPNP_EXECUTABLE}")
endif()

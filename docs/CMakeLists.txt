find_package(Doxygen)

if(NOT DOXYGEN_FOUND)
    message(STATUS "No Doxygen found. Documentation won't be built")
else()
    set(OUT_DOXYFILE ${CMAKE_BINARY_DIR}/Doxyfile)

    configure_file(Doxyfile.in ${OUT_DOXYFILE} @ONLY)

    add_custom_target(docs ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${OUT_DOXYFILE}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Generating API documentation with doxygen"
        VERBATIM
    )

    install(DIRECTORY ${CMAKE_BINARY_DIR}/docs/html
        DESTINATION docs
    )
endif()

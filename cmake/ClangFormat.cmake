# project/
# --third3ty/
# --src/
# --test/
# --doc/
# --.clang_format
#
# clang_format_recurse("src" "test")

option(USE_CLANG_FORMAT "Apply clang-format to files in-place." OFF)

function(clang_format_recurse DIRS...)
    if(USE_CLANG_FORMAT)
        # Try to locate "clang-format"
        find_program(CLANG_FORMAT clang-format PATHS ENV PATH)
        if(CLANG_FORMAT)
            message(STATUS "clang-format found at: ${CLANG_FORMAT}")
            execute_process(COMMAND ${CLANG_FORMAT} --version)
        else()
            message(FATAL_ERROR "clang-format not found, style not available")
        endif()

        # Process individual file
        foreach(DIR ${ARGV})
            file(GLOB_RECURSE FILES
                "${DIR}/*.cpp"
                "${DIR}/*.h"
                "${DIR}/*.hpp"
            )
            foreach(FILE ${FILES})
                if(NOT "${FILE}" STREQUAL "")
                    execute_process(COMMAND ${CLANG_FORMAT} -style=file -i ${FILE})
                endif()
            endforeach(FILE)
        endforeach(DIR)
    endif()
endfunction(clang_format_recurse)

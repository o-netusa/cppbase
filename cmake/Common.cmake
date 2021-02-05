# Instructs CMake to treat warnings as errors for the specified target.
function(set_target_warnings_as_error target)
  set(currentlib ${ARGN})
  if(MSVC)
    target_compile_options(${currentlib} PRIVATE /W4 /WX)
  else()
    target_compile_options(${currentlib} PRIVATE -Wall -Wextra -pedantic -Werror)
  endif()
endfunction(set_target_warnings_as_error)

if (MSVC)
  add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
  add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

  # -DWIN32_LEAN_AND_MEAN is for winsock.h has already been included error
  # -D_SILENCE_CXX17_ALLOCATOR_VOID_DEPRECATION_WARNING is for warning STL4009
  #   std::allocator<void> is deprecated in C++17
  add_definitions(
      -D_WIN32_WINNT=0x0A00
      -DNOMINMAX
      -DWIN32_LEAN_AND_MEAN
      -D_SILENCE_CXX17_ALLOCATOR_VOID_DEPRECATION_WARNING
      -D_CRT_SECURE_NO_WARNINGS)
endif()

#
# This function will prevent in-source builds
function(AssureOutOfSourceBuilds)
  # make sure the user doesn't play dirty with symlinks
  get_filename_component(srcdir "${CMAKE_SOURCE_DIR}" REALPATH)
  get_filename_component(bindir "${CMAKE_BINARY_DIR}" REALPATH)

  # disallow in-source builds
  if("${srcdir}" STREQUAL "${bindir}")
    message("######################################################")
    message("Warning: in-source builds are disabled")
    message("Please create a separate build directory and run cmake from there")
    message("######################################################")
    message(FATAL_ERROR "Quitting configuration")
  endif()
endfunction()

AssureOutOfSourceBuilds()

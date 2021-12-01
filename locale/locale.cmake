cmake_minimum_required(VERSION 3.18 FATAL_ERROR)

if(CMAKE_SYSTEM_NAME MATCHES "Windows")
  set(gettext_msgfmt "${CMAKE_SOURCE_DIR}/locale/bin/msgfmt.exe")
else()
  set(gettext_msgfmt "msgfmt")
endif()

function(generate_mo locale_name output_dir)
  message(STATUS "Calling generate_mo ${locale_name} ${output_dir}")
  file(MAKE_DIRECTORY "${output_dir}/${locale_name}/LC_MESSAGES")
  execute_process(COMMAND ${gettext_msgfmt}
      --output-file=${output_dir}/${locale_name}/LC_MESSAGES/lidar_viewer.mo
      ${CMAKE_SOURCE_DIR}/locale/${locale_name}/lidar_viewer.po)
endfunction()

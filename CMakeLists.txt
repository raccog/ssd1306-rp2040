cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)
include(cmake/VersionFromGit.cmake)

get_version(ssd1306)
message(STATUS "SSD1306 library version: ${ssd1306_VERSION}")
configure_file(include/ssd1306/version.h.in include/ssd1306/version.h @ONLY)

project(ssd1306)

pico_sdk_init()

add_subdirectory(src)
add_subdirectory(test)


cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)
include(cmake/VersionFromGit.cmake)

get_version(SSD1306)
configure_file(include/ssd1306/version.h.in include/ssd1306/version.h @ONLY)

message(STATUS "SSD1306 library version: ${SSD1306_VERSION}")

project(ssd1306)

pico_sdk_init()

add_subdirectory(src)
add_subdirectory(test)


cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)

project(ssd1306)

pico_sdk_init()

add_subdirectory(src)
add_subdirectory(test)


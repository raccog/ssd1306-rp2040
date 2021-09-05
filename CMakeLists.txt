cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)
include(cmake/VersionFromGit.cmake)

get_version(ssd1306)
message(STATUS "SSD1306 library version: ${ssd1306_VERSION}")
configure_file(include/ssd1306/version.h.in include/ssd1306/version.h @ONLY)

project(ssd1306)

pico_sdk_init()

add_library(${PROJECT_NAME} INTERFACE)
target_sources(${PROJECT_NAME} INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/src/ssd1306.c
)
target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include ${CMAKE_CURRENT_BINARY_DIR}/include)
target_link_libraries(${PROJECT_NAME} INTERFACE
    pico_stdlib hardware_i2c
)

add_subdirectory(test)


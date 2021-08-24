cmake_minimum_required(VERSION 3.12)

execute_process(COMMAND "git" "describe" OUTPUT_VARIABLE version_string RESULT_VARIABLE result)

if (result EQUAL 0)
    string(STRIP ${version_string} version_string)
    message(STATUS "Version number was extracted from git.")
    string(REGEX MATCHALL "[0-9]+" version_list ${version_string})
    list(GET version_list 0 SSD1306_VERSION_MAJOR)
    list(GET version_list 1 SSD1306_VERSION_MINOR)
    list(GET version_list 2 SSD1306_VERSION_PATCH)
else()
    set(SSD1306_VERSION_MAJOR 0)
    set(SSD1306_VERSION_MINOR 1)
    set(SSD1306_VERSION_PATCH 0)
    set(version_string "${SSD1306_VERSION_MAJOR}.${SSD1306_VERSION_MINOR}.${SSD1306_VERSION_PATCH}")
    message(STATUS "Version could not be extracted from git. Using version number from Cmake.")
endif()

set(SSD1306_VERSION ${version_string})

configure_file(src/version.h.in src/version.h @ONLY)
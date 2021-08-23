add_executable(ssd1306_test
    ssd1306_test.c
)
pico_add_extra_outputs(ssd1306_test)
target_link_libraries(ssd1306_test ssd1306)

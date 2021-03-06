#include <stdio.h>

#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"

#include "ssd1306/ssd1306.h"

#define TEST_SLEEP_MS 2000

const uint8_t HOUSEWIFE_HORIZONTAL[] = {
    0x55, 0x0, 0xaa, 0x11, 0x44, 0x91, 0x24, 0x49, 0x2, 0x54, 0x1, 0x54, 0x21, 0xa, 0x50, 0x5,  // 16 bytes
    0x48, 0x11, 0x42, 0x14, 0x41, 0x14, 0x1, 0x48, 0x2, 0x11, 0x40, 0x4, 0x11, 0x40, 0x4, 0x91,  // 32 bytes
    0x0, 0x84, 0x21, 0x40, 0x90, 0x21, 0x50, 0x8, 0x41, 0x90, 0x0, 0x29, 0x80, 0x10, 0x41, 0x0,  // 48 bytes
    0x29, 0x80, 0x10, 0x41, 0x88, 0x20, 0x40, 0x81, 0x28, 0x0, 0xa8, 0x1, 0x54, 0x20, 0x88, 0x25,  // 64 bytes
    0x50, 0x4, 0xb0, 0x5, 0xaa, 0x50, 0x5, 0xa8, 0x2, 0xa9, 0x14, 0x40, 0x95, 0x20, 0x14, 0x41,  // 80 bytes
    0x14, 0x40, 0x95, 0x0, 0xa8, 0x5, 0x50, 0x0, 0x90, 0x1, 0xa0, 0x0, 0x20, 0x1, 0x48, 0x11,  // 96 bytes
    0x26, 0x49, 0x36, 0x49, 0x96, 0x29, 0x56, 0x29, 0x56, 0x89, 0x36, 0x49, 0x26, 0x49, 0x16, 0xc9,  // 112 bytes
    0x12, 0xa5, 0x4a, 0x95, 0x22, 0x55, 0x8a, 0x55, 0xa2, 0xd, 0xb2, 0x45, 0x9a, 0x25, 0x5a, 0xd5,  // 128 bytes
    0x55, 0x0, 0x4a, 0x10, 0x45, 0x90, 0x24, 0x49, 0x4, 0x51, 0x24, 0x89, 0x24, 0x81, 0x2a, 0x41,  // 144 bytes
    0x94, 0x41, 0x94, 0x21, 0xc8, 0x11, 0xa8, 0x52, 0xa4, 0x9, 0xf4, 0x3, 0x54, 0x29, 0x82, 0x0,  // 160 bytes
    0x28, 0x2, 0x90, 0x1, 0x54, 0x0, 0x41, 0x14, 0x80, 0x22, 0x0, 0x8, 0x82, 0x10, 0x4, 0xa9,  // 176 bytes
    0x0, 0x2a, 0x40, 0x9, 0x42, 0x14, 0x4d, 0x30, 0xc5, 0x1a, 0xa4, 0x49, 0x12, 0xa5, 0x8, 0x52,  // 192 bytes
    0x5, 0x20, 0x8a, 0x20, 0xa, 0x0, 0x55, 0x0, 0x42, 0x8, 0xa1, 0x4, 0x40, 0x14, 0xa1, 0x4,  // 208 bytes
    0x21, 0x8, 0x42, 0x0, 0x54, 0x0, 0x11, 0x44, 0x10, 0x42, 0x8, 0xa0, 0x5, 0x80, 0x29, 0x0,  // 224 bytes
    0x45, 0x10, 0x0, 0x45, 0x90, 0x4, 0x21, 0x84, 0x11, 0x44, 0x0, 0x55, 0x0, 0x25, 0x80, 0xa,  // 240 bytes
    0x20, 0x8a, 0x0, 0x2a, 0x81, 0x14, 0x42, 0x8, 0x12, 0x41, 0x14, 0x82, 0x29, 0x84, 0x11, 0x6e,  // 256 bytes
    0x55, 0x0, 0x29, 0x42, 0x8, 0x52, 0x84, 0x51, 0xa4, 0x49, 0xb2, 0x44, 0xba, 0x44, 0xb3, 0x44,  // 272 bytes
    0x2a, 0xd2, 0x24, 0x99, 0x66, 0x88, 0x75, 0x8a, 0x75, 0x2, 0xa4, 0x9, 0x40, 0x2, 0x8, 0x0,  // 288 bytes
    0x22, 0x0, 0x88, 0x2, 0x20, 0x9, 0x2, 0x20, 0x8, 0x0, 0x0, 0x84, 0x0, 0x40, 0x4, 0x20,  // 304 bytes
    0x4, 0xa1, 0x8, 0x42, 0x90, 0x25, 0xd0, 0x5, 0xaa, 0x40, 0x12, 0x48, 0x12, 0xa8, 0x4, 0x21,  // 320 bytes
    0x8, 0x2, 0x20, 0x4, 0x81, 0x14, 0x80, 0x25, 0x80, 0x2a, 0x0, 0x54, 0x1, 0xa8, 0x2, 0x8,  // 336 bytes
    0xa1, 0x4, 0x50, 0x8a, 0x71, 0x84, 0x2b, 0x50, 0xa4, 0x9, 0xa0, 0x4, 0x10, 0x42, 0x8, 0x80,  // 352 bytes
    0x25, 0x80, 0x22, 0x88, 0x52, 0x80, 0x15, 0xa0, 0x4a, 0x0, 0xa9, 0x0, 0x52, 0x0, 0x94, 0x41,  // 368 bytes
    0x0, 0x14, 0x42, 0x0, 0x14, 0x40, 0x11, 0x84, 0x1, 0x28, 0x82, 0x8, 0x20, 0x82, 0x8, 0x73,  // 384 bytes
    0x55, 0x0, 0x55, 0xa0, 0xdd, 0xb2, 0x6e, 0xd9, 0xb6, 0x69, 0xd6, 0x2d, 0xf2, 0x8d, 0x72, 0x8d,  // 400 bytes
    0x72, 0x85, 0x3a, 0xc5, 0x3a, 0xc5, 0x3a, 0xc5, 0x28, 0x95, 0x40, 0x2a, 0x80, 0x54, 0x1, 0x50,  // 416 bytes
    0x84, 0x20, 0x8, 0x0, 0x0, 0x0, 0x1, 0x0, 0x0, 0x20, 0x8, 0x40, 0x10, 0xac, 0x2a, 0x5c,  // 432 bytes
    0x6a, 0xd6, 0x3d, 0xea, 0x37, 0xea, 0x9d, 0x62, 0xdd, 0x22, 0xd9, 0x22, 0x48, 0x92, 0x28, 0x41,  // 448 bytes
    0x14, 0xa0, 0x2, 0xa8, 0x0, 0x54, 0x80, 0x2a, 0x40, 0x14, 0xa1, 0xa, 0xa0, 0xa, 0xa0, 0x8,  // 464 bytes
    0x42, 0x0, 0x11, 0x2, 0xa4, 0x9, 0x2, 0x49, 0x80, 0x12, 0x24, 0x49, 0x90, 0x2, 0xa4, 0x0,  // 480 bytes
    0xaa, 0x0, 0x2a, 0x50, 0x8a, 0x24, 0x52, 0x88, 0x22, 0xc9, 0x14, 0xc0, 0x15, 0xa8, 0x2, 0xa8,  // 496 bytes
    0x41, 0x14, 0x81, 0x28, 0x1, 0xa0, 0xa, 0x20, 0x80, 0xa, 0x20, 0x0, 0x8a, 0x20, 0x0, 0xdb,  // 512 bytes
    0x95, 0x20, 0x4f, 0xb4, 0x6b, 0x9e, 0x6b, 0x95, 0x6e, 0x91, 0x6f, 0x90, 0x6f, 0x94, 0x29, 0xd6,  // 528 bytes
    0x29, 0xc6, 0x38, 0xc7, 0x38, 0xc7, 0x98, 0x22, 0x95, 0x40, 0x15, 0x2, 0xac, 0x1, 0xaa, 0x11,  // 544 bytes
    0x84, 0x20, 0x1, 0x48, 0x0, 0x88, 0x22, 0x0, 0x42, 0x8, 0x0, 0x2, 0x10, 0x80, 0x4, 0x51,  // 560 bytes
    0x4, 0x29, 0x84, 0x69, 0xb, 0x50, 0xa3, 0x14, 0x43, 0x8c, 0x1b, 0x24, 0x5b, 0x24, 0x90, 0x21,  // 576 bytes
    0x40, 0x88, 0x22, 0x44, 0xa9, 0x12, 0xa4, 0x48, 0x55, 0x88, 0x52, 0xa4, 0xa, 0x50, 0x2, 0x8,  // 592 bytes
    0x22, 0x0, 0x1, 0x0, 0x14, 0x20, 0xa, 0xa0, 0xa, 0x41, 0xb4, 0x5, 0xa8, 0x12, 0xa4, 0x4a,  // 608 bytes
    0x10, 0xa5, 0x48, 0x15, 0xa2, 0x49, 0x94, 0x2b, 0x40, 0xae, 0x10, 0xa5, 0x4a, 0x14, 0xa1, 0xa,  // 624 bytes
    0x50, 0x8a, 0x20, 0x95, 0x0, 0xaa, 0x0, 0xa8, 0x2, 0x48, 0x21, 0x4, 0x50, 0x2, 0x20, 0xdd,  // 640 bytes
    0x52, 0x4, 0x50, 0x5, 0xaa, 0x45, 0x2a, 0x1d, 0xe2, 0x3d, 0xc2, 0x3d, 0xc2, 0x3d, 0xc2, 0x3d,  // 656 bytes
    0xc2, 0x2d, 0xd0, 0x27, 0xd8, 0x26, 0xd9, 0x26, 0xe8, 0x4, 0x51, 0x4, 0xa0, 0x2, 0x48, 0x2,  // 672 bytes
    0x54, 0x0, 0x55, 0x2a, 0x40, 0xaa, 0x40, 0xa8, 0x2, 0x40, 0x1, 0x0, 0x0, 0x40, 0x8, 0x0,  // 688 bytes
    0xaa, 0x0, 0xaa, 0x0, 0x29, 0x42, 0x98, 0x22, 0x49, 0xb6, 0x48, 0x95, 0x20, 0x0, 0x44, 0x90,  // 704 bytes
    0x1, 0x42, 0x8, 0x85, 0x10, 0x5, 0x54, 0x9, 0x92, 0x24, 0x49, 0x12, 0xa4, 0x1, 0x28, 0x0,  // 720 bytes
    0x44, 0x0, 0x90, 0x4, 0x49, 0x2, 0x8, 0xa2, 0x5, 0x48, 0x12, 0x1, 0x54, 0x2, 0x50, 0x25,  // 736 bytes
    0x8, 0x52, 0x84, 0x29, 0x42, 0x8, 0x52, 0x89, 0x22, 0x54, 0x1, 0x54, 0xa2, 0x9, 0xa4, 0x12,  // 752 bytes
    0x49, 0xa0, 0xa, 0x24, 0x92, 0x8, 0xa2, 0x14, 0x42, 0x31, 0x84, 0x50, 0x25, 0x90, 0x42, 0xbd,  // 768 bytes
    0x55, 0x0, 0x4a, 0x1, 0x54, 0x0, 0x55, 0x2, 0xa8, 0x11, 0x42, 0x95, 0x2, 0xaf, 0x10, 0x4f,  // 784 bytes
    0x34, 0x4b, 0xb4, 0x4b, 0xb4, 0xcb, 0x34, 0xcb, 0x34, 0xcb, 0x34, 0xc9, 0x24, 0x90, 0x42, 0x20,  // 800 bytes
    0x89, 0x2, 0x55, 0xa, 0x51, 0xa4, 0xa, 0xa0, 0x15, 0x40, 0x12, 0x0, 0x4, 0x50, 0x2, 0x8,  // 816 bytes
    0xa0, 0x2, 0x28, 0x44, 0x93, 0x24, 0x5a, 0x25, 0x8a, 0x39, 0x42, 0x95, 0x48, 0xb4, 0x49, 0xa6,  // 832 bytes
    0x58, 0xa5, 0x4a, 0x90, 0x6a, 0x94, 0x20, 0xca, 0x30, 0x84, 0x70, 0x4, 0xe8, 0x12, 0xa0, 0x55,  // 848 bytes
    0x28, 0xc0, 0x1a, 0xa4, 0x50, 0x25, 0x4a, 0x90, 0xa2, 0x14, 0xa1, 0x54, 0x89, 0x24, 0xd1, 0x24,  // 864 bytes
    0x89, 0x72, 0x84, 0x30, 0x45, 0xaa, 0x11, 0xe4, 0xa, 0xb1, 0x4c, 0x91, 0x66, 0x98, 0x62, 0x95,  // 880 bytes
    0x4a, 0xb0, 0x45, 0x20, 0x4a, 0xa0, 0x4a, 0x94, 0x21, 0x4a, 0x94, 0x21, 0x8a, 0x24, 0x90, 0x6f,  // 896 bytes
    0x55, 0x80, 0x29, 0x84, 0x51, 0x4, 0xa9, 0x0, 0xaa, 0x0, 0xca, 0x10, 0xa2, 0x4, 0xc8, 0x11,  // 912 bytes
    0x42, 0x94, 0x20, 0x85, 0x50, 0x82, 0x9, 0xa2, 0x5, 0x42, 0x8d, 0x2, 0x9d, 0x42, 0x1d, 0xa2,  // 928 bytes
    0x1d, 0x82, 0x34, 0x48, 0x90, 0x20, 0x88, 0x0, 0x82, 0x20, 0x81, 0x10, 0x80, 0x1, 0xa0, 0x8,  // 944 bytes
    0x80, 0x4, 0x92, 0x48, 0x16, 0xa8, 0x5, 0x5a, 0x84, 0x13, 0x4c, 0x83, 0x14, 0x4b, 0x84, 0x19,  // 960 bytes
    0x46, 0x99, 0x6, 0xb9, 0x6, 0xb9, 0x86, 0x39, 0x46, 0xb9, 0x6, 0x79, 0x86, 0x39, 0x86, 0xb9,  // 976 bytes
    0x2, 0xbd, 0x42, 0x1c, 0xa1, 0x1f, 0x80, 0x2e, 0x91, 0xc, 0x53, 0x8c, 0x32, 0xcd, 0x10, 0xef,  // 992 bytes
    0x10, 0xee, 0x11, 0xee, 0x99, 0x66, 0xd9, 0x26, 0xfd, 0x82, 0x7d, 0x82, 0xfd, 0x2, 0xfd, 0x2,  // 1008 bytes
    0xec, 0x13, 0xe4, 0xb, 0xf4, 0x2, 0xed, 0x10, 0xa5, 0xaa, 0x44, 0x99, 0x62, 0x94, 0x4a, 0xb5 // 1024 bytes
};

int main() {
    stdio_init_all();
    i2c_init(i2c1, 100 * 1000);
    gpio_set_function(2, GPIO_FUNC_I2C);
    gpio_set_function(3, GPIO_FUNC_I2C);
    gpio_pull_up(2);
    gpio_pull_up(3);

    printf("--------------------------------------------------------------\n");
    printf("Starting SSD1306 test program in %i milliseconds...\n", TEST_SLEEP_MS);
    printf("Running library version: %s\n", ssd1306_VERSION);
    printf("--------------------------------------------------------------\n");
    sleep_ms(TEST_SLEEP_MS);

    printf("Initializing OLED with empty screen (%i milliseconds)...\n\n", TEST_SLEEP_MS);
    ssd1306_state_t ssd;
    ssd1306_init(&ssd, 0x3c, i2c1, BLACK);
    sleep_ms(TEST_SLEEP_MS);
    ssd1306_free(&ssd);

    printf("Reinitializing OLED with full screen (%i milliseconds)...\n\n", TEST_SLEEP_MS);
    ssd1306_init(&ssd, 0x3c, i2c1, WHITE);
    ssd1306_set_full_rotation(&ssd, true);
    sleep_ms(TEST_SLEEP_MS);
    
    printf("Setting sleep mode on (%i milliseconds)...\nDisplay should be blank.\n\n", TEST_SLEEP_MS);
    ssd1306_set_display_power(&ssd, false);
    sleep_ms(TEST_SLEEP_MS);

    printf("Setting sleep mode off (%i milliseconds)...\nDisplay should be full.\n\n", TEST_SLEEP_MS);
    ssd1306_set_display_power(&ssd, true);
    sleep_ms(TEST_SLEEP_MS);

    printf("Setting inverse mode on (%i milliseconds)...\nDisplay should be blank.\n\n", TEST_SLEEP_MS);
    ssd1306_set_invert_colors(&ssd, true);
    sleep_ms(TEST_SLEEP_MS);

    printf("Setting inverse mode off (%i milliseconds)...\nDisplay should be full.\n\n", TEST_SLEEP_MS);
    ssd1306_set_invert_colors(&ssd, false);
    sleep_ms(TEST_SLEEP_MS);

    printf("Pausing desplay (%i milliseconds)...\nDisplay should stay full.\n\n", TEST_SLEEP_MS);
    ssd1306_set_pause_display(&ssd, true);
    ssd1306_buffer_fill_pixels(&ssd, BLACK);
    ssd1306_set_pixels(&ssd);
    sleep_ms(TEST_SLEEP_MS);

    printf("Resuming display (%i milliseconds)...\nDisplay should be blank.\n\n", TEST_SLEEP_MS);
    ssd1306_set_pause_display(&ssd, false);
    sleep_ms(TEST_SLEEP_MS);

    printf("Drawing lines (%i milliseconds)...\nDisplay should be showing a real housewife.\n\n", TEST_SLEEP_MS);
    ssd1306_buffer_set_pixels_direct(&ssd, HOUSEWIFE_HORIZONTAL);
    ssd1306_set_pixels(&ssd);
    sleep_ms(TEST_SLEEP_MS);

    printf("Flipping horizontally (%i milliseconds)...\nDisplay should be flipped on the wide, horizontal axis.\n\n", TEST_SLEEP_MS);
    ssd1306_set_horizontal_flip(&ssd, false);
    ssd1306_set_pixels(&ssd);
    sleep_ms(TEST_SLEEP_MS);

    printf("Flipping vertically (%i milliseconds)...\nDisplay should be flipped on the short, vertical axis.\n\n", TEST_SLEEP_MS);
    ssd1306_set_vertical_flip(&ssd, false);
    ssd1306_set_pixels(&ssd);
    sleep_ms(TEST_SLEEP_MS);

    printf("Rotating display by 180 degrees (%i milliseconds)...\nDisplay should be flipped on both axes.\n\n", TEST_SLEEP_MS);
    ssd1306_set_full_rotation(&ssd, true);
    ssd1306_set_pixels(&ssd);
    sleep_ms(TEST_SLEEP_MS);

    printf("Finished SSD1306 test program.\n");
    printf("--------------------------------------------------------------\n\n");
    ssd1306_free(&ssd);
    return 0;
}

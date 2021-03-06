#ifndef __SSD1306_H__
#define __SSD1306_H__

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "hardware/i2c.h"

#include "ssd1306/version.h"

#define SSD1306_CONTRAST 0x81
#define SSD1306_DISPLAYPAUSEOFF 0xA4
#define SSD1306_DISPLAYPAUSEON 0xA5
#define SSD1306_NORMALDISPLAY 0xA6
#define SSD1306_INVERTDISPLAY 0xA7
#define SSD1306_DISPLAYOFF 0xAE
#define SSD1306_DISPLAYON 0xAF
#define SSD1306_DISPLAYOFFSET 0xD3
#define SSD1306_COMPINS 0xDA
#define SSD1306_VCOMDESELECT 0xDB
#define SSD1306_DISPLAYCLOCKDIV 0xD5
#define SSD1306_PRECHARGE 0xD9
#define SSD1306_MULTIPLEX 0xA8
#define SSD1306_LOWCOLUMN 0x00
#define SSD1306_HIGHCOLUMN 0x10
#define SSD1306_STARTLINE 0x40
#define SSD1306_MEMORYMODE 0x20
#define SSD1306_COLUMNADDR 0x21
#define SSD1306_PAGEADDR   0x22
#define SSD1306_COMSCANNORMAL 0xC0
#define SSD1306_COMSCANREMAP 0xC8
#define SSD1306_SEGNORMAL 0xA0
#define SSD1306_SEGREMAP 0xA1
#define SSD1306_CHARGEPUMP 0x8D
#define SSD1306_EXTERNALVCC 0x1
#define SSD1306_SWITCHCAPVCC 0x2

#define SSD1306_CLOCKDIVRESET 0x80
#define SSD1306_DISPLAYOFFSETRESET 0x00
#define SSD1306_ENABLECHARGEPUMP 0x14
#define SSD1306_COMPINSRESET 0x12
#define SSD1306_CONTRASTRESET 0x7f
#define SSD1306_PRECHARGERESET 0x22
#define SSD1306_VCOMDESELECTRESET 0x20
#define SSD1306_PAGEADDRSTART 0x00
#define SSD1306_PAGEADDREND 0x07
#define SSD1306_COLUMNADDRSTART 0x00

#define SSD1306_CONTROL_COMMAND 0x00
#define SSD1306_CONTROL_DATA 0x40

/*

ssd1306_state_t structs and enums.

*/

typedef enum {
	BLACK = 0,
    WHITE = 1,
} ssd1306_color_t;

typedef enum {
    HORIZONTAL_ADDR = 0x00,
    VERTICAL_ADDR = 0x01,
    PAGE_ADDR = 0x02
} ssd1306_memory_mode_t;

typedef struct {
    uint16_t addr;
    i2c_inst_t *i2c;
    uint8_t width;
    uint8_t height;
    uint8_t *buffer;
} ssd1306_state_t;

/*
 
ssd1306_state_t initialization and free functions.

*/

uint32_t ssd1306_init(ssd1306_state_t *ssd, uint16_t addr, i2c_inst_t *i2c, ssd1306_color_t color);

static inline void ssd1306_free(ssd1306_state_t *ssd) {
    free(ssd->buffer);
}

/*

Low level state changing functions.

These functions change the ssd1306_state_t state by sending either commands or data.

*/

static inline void ssd1306_send_command(ssd1306_state_t *ssd, uint8_t command) {
    const uint8_t message[] = {SSD1306_CONTROL_COMMAND, command};
    i2c_write_blocking(ssd->i2c, ssd->addr, message, 2, false);
}

void ssd1306_send_command_list(ssd1306_state_t *ssd, const uint8_t *commands, size_t command_size);

void ssd1306_send_data(ssd1306_state_t *ssd, const uint8_t *data, size_t data_size);

/*

ssd1306_state_t state changing functions.

These functions send commands to the ssd1306_state_t and affect its state.

*/

void ssd1306_set_pixels(ssd1306_state_t *ssd);

static inline void ssd1306_set_display_power(ssd1306_state_t *ssd, bool power) {
    ssd1306_send_command(ssd, SSD1306_DISPLAYOFF | (uint8_t)power);
}

static inline void ssd1306_set_pause_display(ssd1306_state_t *ssd, bool pause) {
    ssd1306_send_command(ssd, SSD1306_DISPLAYPAUSEOFF | (uint8_t)pause);
}

static inline void ssd1306_set_invert_colors(ssd1306_state_t *ssd, bool invert) {
    ssd1306_send_command(ssd, SSD1306_NORMALDISPLAY | (uint8_t)invert);
}

static inline void ssd1306_set_memory_mode(ssd1306_state_t *ssd, ssd1306_memory_mode_t mode) {
    const uint8_t message[] = {SSD1306_MEMORYMODE, (uint8_t)mode};
    ssd1306_send_command_list(ssd, message, 2);
}

static inline void ssd1306_set_vertical_flip(ssd1306_state_t *ssd, bool flipped) {
    ssd1306_send_command(ssd, SSD1306_COMSCANNORMAL | ((uint8_t)flipped) * 0x8);
}

static inline void ssd1306_set_horizontal_flip(ssd1306_state_t *ssd, bool flipped) {
    ssd1306_send_command(ssd, SSD1306_SEGNORMAL | (uint8_t)flipped);
}

static inline void ssd1306_set_full_rotation(ssd1306_state_t *ssd, bool rotated) {
    ssd1306_set_vertical_flip(ssd, rotated);
    ssd1306_set_horizontal_flip(ssd, rotated);
}

/*

Buffer changing functions.

These functions alter the buffer in memory and do not change the ssd1306_state_t state
until a state changing function is used.

*/

static inline void ssd1306_buffer_set_pixels_direct(ssd1306_state_t *ssd, const uint8_t *pixels) {
    memcpy(ssd->buffer, pixels, ssd->width * ssd->height / 8);
}

static inline void ssd1306_buffer_fill_pixels(ssd1306_state_t *ssd, ssd1306_color_t color) {
    memset(ssd->buffer, (color == WHITE) ? 0xff : 0x00, ssd->width * ssd->height / 8);
}

#endif

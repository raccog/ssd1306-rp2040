#include "ssd1306/ssd1306.h"

#include <assert.h>

uint32_t ssd1306_init(ssd1306_state_t *ssd, uint16_t addr, i2c_inst_t *i2c, ssd1306_color_t color) {
    ssd->addr = addr;
    ssd->i2c = i2c;
    ssd->width = 128;
    ssd->height = 64;
    ssd->buffer = malloc(ssd->width * ssd->height / 8);

    const uint8_t init[] = {
        SSD1306_DISPLAYOFF,
        SSD1306_DISPLAYCLOCKDIV,
        SSD1306_CLOCKDIVRESET, // reset clock div
        SSD1306_MULTIPLEX,
        ssd->height - 1, // reset multiplex
        SSD1306_DISPLAYOFFSET,
        SSD1306_DISPLAYOFFSETRESET, // no offset
        SSD1306_STARTLINE,
        SSD1306_CHARGEPUMP,
        SSD1306_ENABLECHARGEPUMP, // enable charge pump
        SSD1306_MEMORYMODE,
        (uint8_t)HORIZONTAL_ADDR, // horizontal addressing mode
        SSD1306_SEGNORMAL,
        SSD1306_COMSCANNORMAL,
        SSD1306_COMPINS,
        SSD1306_COMPINSRESET, // alt pin config and disable left/right remap
        SSD1306_CONTRAST,
        SSD1306_CONTRASTRESET, // max contrast
        SSD1306_PRECHARGE,
        SSD1306_PRECHARGERESET, // reset phase 1 and 2
        SSD1306_VCOMDESELECT,
        SSD1306_VCOMDESELECTRESET, 
        SSD1306_DISPLAYPAUSEOFF,
        SSD1306_NORMALDISPLAY,
        SSD1306_DISPLAYON,
    };
    ssd1306_send_command_list(ssd, init, sizeof(init));
    
    ssd1306_buffer_fill_pixels(ssd, color);
    ssd1306_set_pixels(ssd);
}


void ssd1306_set_pixels(ssd1306_state_t *ssd) {
    assert(ssd != NULL);
    const size_t BUFFER_SIZE = ssd->width * ssd->height / 8;
    const uint8_t init[] = {
        SSD1306_PAGEADDR,
        SSD1306_PAGEADDRSTART,
        SSD1306_PAGEADDREND,
        SSD1306_COLUMNADDR,
        SSD1306_COLUMNADDRSTART,
        ssd->width - 1,
    };
    ssd1306_send_command_list(ssd, init, sizeof(init));
    ssd1306_send_data(ssd, ssd->buffer, BUFFER_SIZE);
}

void ssd1306_send_command_list(ssd1306_state_t *ssd, const uint8_t *commands, size_t command_size) {
    assert(ssd != NULL && commands != NULL && command_size > 0);
    uint8_t command_buffer[command_size + 1];
    command_buffer[0] = SSD1306_CONTROL_COMMAND;
    memcpy(command_buffer + 1, commands, command_size);
    i2c_write_blocking(ssd->i2c, ssd->addr, command_buffer, command_size + 1, false);
}

void ssd1306_send_data(ssd1306_state_t *ssd, const uint8_t *data, size_t data_size) {
    uint8_t pixel_buffer[data_size + 1];
    memcpy(pixel_buffer + 1, ssd->buffer, data_size);
    pixel_buffer[0] = SSD1306_CONTROL_DATA;
    i2c_write_blocking(ssd->i2c, ssd->addr, pixel_buffer, data_size + 1, false);
}


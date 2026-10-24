#include "test_lcd.h"
#include "ppu.h"
#include "lodepng.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const uint32_t GREYSCALE_RGBA8_PALLETE[4] = {
    0xFFFFFFFF, // lightest
    0xAAAAAAFF, // second lighest
    0x555555FF, // second darkest
    0x000000FF, // darkest
};

uint8_t* render_lcd_grayscale_rgba8() {
    uint32_t* image_buffer = (uint32_t*)malloc(LCD_RGBA8_SIZE_BYTES);
    if (image_buffer == NULL) return NULL;

    for (int y = 0; y < LCD_RES_Y; y++) {
        for (int x = 0; x < LCD_RES_X; x++) {
            int index = (y * LCD_RES_X) + x;
            uint8_t pallete_index = lcd_frame_buffer[index / 4] >> ((index % 4) * 2); // 2 bits corresponding to this pixel's color
            uint32_t rgba = GREYSCALE_RGBA8_PALLETE[pallete_index & 0x03];
            
            uint8_t* px = (uint8_t*)&image_buffer[index];
            px[0] = (uint8_t)(rgba >> 24); // R
            px[1] = (uint8_t)(rgba >> 16); // G
            px[2] = (uint8_t)(rgba >> 8); // B
            px[3] = (uint8_t)(rgba);  // A
        }
    }
    return (uint8_t*)image_buffer;
}

bool lcd_equal(uint8_t* lcd_buf, uint8_t* expected_buf) {
    return memcmp(lcd_buf, expected_buf, LCD_RGBA8_SIZE_BYTES) == 0;
}

uint8_t* diff_lcd(uint8_t* lcd_buf, uint8_t* expected_buf) {
    uint8_t* diff_img_buf = (uint8_t*)malloc(LCD_RGBA8_SIZE_BYTES);
    for (int y = 0; y < LCD_RES_Y; y++) {
        for (int x = 0; x < LCD_RES_X; x++) {
            size_t px = ((size_t)y * LCD_RES_X + x) * 4;
            const uint8_t *e = &expected_buf[px];
            const uint8_t *a = &lcd_buf[px];
            uint8_t* out_px = (uint8_t*)&diff_img_buf[px];

            if (memcmp(e, a, 4) != 0) {
                out_px[0] = 0xFF; // R
                out_px[1] = 0x00; // G
                out_px[2] = 0x00; // B
            } else {
                // washed-out copy of the expected pixel so the red stands out
                out_px[0] = e[0] / 3 + 170; // R
                out_px[1] = e[1] / 3 + 170; // G
                out_px[2] = e[2] / 3 + 170; // B
            }
            out_px[3] = 0xFF;  // A
        }
    }

    return diff_img_buf;
}

bool write_lcd_png(char* file_path, uint8_t* image_buffer) {
    unsigned int err = lodepng_encode32_file(file_path, image_buffer, LCD_RES_X, LCD_RES_Y);

    if (err) {
        fprintf(stderr, "Error on PNG write: path=%s, err=%s", file_path, lodepng_error_text(err));
    }

    return err == 0;
}


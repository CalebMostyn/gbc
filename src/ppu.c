#include "ppu.h"
#include "emu_core.h"

uint8_t lcd_frame_buffer[(LCD_RES_X * LCD_RES_Y) / 4];

int draw_index = 0;
int flip = 0;
int flip_count = 0;
void clock_ppu() {
    // TODO: PPU functionality
    lcd_frame_buffer[draw_index] = draw_index / (LCD_RES_X / 4) % 2 == flip ? 0b11011000 : 0b00100111;

    draw_index++;
    if (draw_index >= sizeof(lcd_frame_buffer)) {
        flip_count += draw_index;
        draw_index = 0;
        if (flip_count > M_CYCLE_HZ) {
            flip_count = 0;
            flip ^= 1;
        }
    }
}

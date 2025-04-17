#include "ppu.h"

extern const Color LCD_PALLETE[LCD_COLOR_PALLETE_SIZE] = {
    (Color){0x9B, 0xBC, 0x0F, 0xFF},
    (Color){0x8B, 0xAC, 0x0F, 0xFF},
    (Color){0x30, 0x62, 0x30, 0xFF},
    (Color){0x0F, 0x38, 0x0F, 0xFF}
};

void clock_ppu() {
    // TODO: PPU functionality
}

void render_lcd() {
    for (int x = 0; x < LCD_RES_X; x++) {
        for (int y = 0; y < LCD_RES_Y; y++) {
            DrawRectangle(((GetScreenWidth() - (LCD_RES_X) * pixel_scale) / 2) + (x * pixel_scale), ((GetScreenHeight() - (LCD_RES_Y) * pixel_scale) / 2) + (y * pixel_scale), pixel_scale, pixel_scale, LCD_PALLETE[(x+y)%LCD_COLOR_PALLETE_SIZE]);
        }
    }
}

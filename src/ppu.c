#include "ppu.h"
#include "emu_core.h"

// middle two colors order is swapped from what might be expected as the DMG
// hardware reverses the 2-bit index order (saving a few operations per pixel)
extern const Color LCD_PALLETE[LCD_COLOR_PALLETE_SIZE] = {
    (Color){0x9B, 0xBC, 0x0F, 0xFF}, // lightest
    (Color){0x30, 0x62, 0x30, 0xFF}, // second darkest
    (Color){0x8B, 0xAC, 0x0F, 0xFF}, // second lighest
    (Color){0x0F, 0x38, 0x0F, 0xFF} // darkest
};

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
        if (flip_count > CPU_CLOCK_HZ) {
            flip_count = 0;
            flip ^= 1;
        }
    }
}

void render_lcd() {
    for (int x = 0; x < LCD_RES_X; x++) {
        for (int y = 0; y < LCD_RES_Y; y++) {
            int index = (y * LCD_RES_X) + x;
            uint8_t val = lcd_frame_buffer[index / 4];
            uint8_t pallete_index = val >> ((index % 4) * 2); // 2 bits corresponding to this pixel
            DrawRectangle(
                ((GetScreenWidth() - (LCD_RES_X) * pixel_scale) / 2) + (x * pixel_scale),
                ((GetScreenHeight() - (LCD_RES_Y) * pixel_scale) / 2) + (y * pixel_scale),
                pixel_scale,
                pixel_scale,
                LCD_PALLETE[pallete_index & 0x03]
            );
        }
    }
}

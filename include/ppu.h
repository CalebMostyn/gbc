#ifndef PPU_H
#define PPU_H

#include "raylib.h"
#include "stdint.h"

#define LCD_RES_X 160 // 160 pixels wide
#define LCD_RES_Y 144 // 144 pixels tall
#define LCD_COLOR_PALLETE_SIZE 4

extern const Color LCD_PALLETE[LCD_COLOR_PALLETE_SIZE];
extern int pixel_scale;

// each 8-bit num represents 4 2-bit pallete indexes for that
// equivalent pixel, "big-endian"
// also of note, DMG stores the 2-bit pallete indexes in reverse order,
// however rather than use operations to reverse the bits, the pallete order has been changed to reflect this
// i.e. 0b01100011 -> pixel 0: 3, 1: 0, 2: 1, 3: 2
extern uint8_t lcd_frame_buffer[(LCD_RES_X * LCD_RES_Y) / 4];

void clock_ppu(); // emulate a clock tick on the picture processing unit
void render_lcd(); // render the LCD image

#define VRAM_BEGIN 0x8000
#define VRAM_END 0x9FFF
#define VRAM_SIZE (VRAM_END - VRAM_BEGIN + 1)

#endif // PPU_H

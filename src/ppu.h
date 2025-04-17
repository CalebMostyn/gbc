#ifndef PPU_H
#define PPU_H

#include "raylib.h"

#define LCD_RES_X 160 // 160 pixels wide
#define LCD_RES_Y 144 // 144 pixels tall
#define LCD_COLOR_PALLETE_SIZE 4

extern const Color LCD_PALLETE[LCD_COLOR_PALLETE_SIZE];
extern int pixel_scale;

void clock_ppu(); // emulate a clock tick on the picture processing unit
void render_lcd(); // render the LCD image

#endif // PPU_H

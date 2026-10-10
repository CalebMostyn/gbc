#ifndef LCD_H
#define LCD_H

#include "raylib.h"

#define LCD_COLOR_PALLETE_SIZE 4

extern const Color LCD_PALLETE[LCD_COLOR_PALLETE_SIZE];
extern int pixel_scale;

void init_lcd();
void render_lcd(); // render the LCD image
void unload_lcd();

#endif // LCD_H

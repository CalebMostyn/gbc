#include <stdbool.h>
#include <stdint.h>

#define LCD_RGBA8_SIZE_BYTES (LCD_RES_X * LCD_RES_Y * sizeof(uint32_t))

uint8_t* render_lcd_grayscale_rgba8();

bool lcd_equal(uint8_t* lcd_buf, uint8_t* expected_buf);

uint8_t* diff_lcd(uint8_t* lcd_buf, uint8_t* expected_buf);

bool write_lcd_png(char* file_path, uint8_t* image_buffer);

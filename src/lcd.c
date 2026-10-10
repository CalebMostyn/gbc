#include "lcd.h"
#include "ppu.h"
#include <stdio.h>
#include <stdlib.h>

// middle two colors order is swapped from what might be expected as the DMG
// hardware reverses the 2-bit index order (saving a few operations per pixel)
const Color LCD_PALLETE[LCD_COLOR_PALLETE_SIZE] = {
    (Color){0x9B, 0xBC, 0x0F, 0xFF}, // lightest
    (Color){0x30, 0x62, 0x30, 0xFF}, // second darkest
    (Color){0x8B, 0xAC, 0x0F, 0xFF}, // second lighest
    (Color){0x0F, 0x38, 0x0F, 0xFF} // darkest
};

static Color *pixels;
Texture2D tex;

void init_lcd() {
    // buffer for texture color data
    pixels = (Color *)malloc(LCD_RES_X * LCD_RES_Y * sizeof(Color));

    // Image to create texture from
    Image img = {
        .data = pixels,
        .width = LCD_RES_X,
        .height = LCD_RES_Y,
        .mipmaps = 1,
        .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8
    };
 
    // Create the GPU texture ONCE (must happen after InitWindow)
    tex = LoadTextureFromImage(img);
    // keep pixels crisp when scaled up
    SetTextureFilter(tex, TEXTURE_FILTER_POINT);
}

void render_lcd() {
    // update texture buffer with color data from frame buffer
    for (int x = 0; x < LCD_RES_X; x++) {
        for (int y = 0; y < LCD_RES_Y; y++) {
            int index = (y * LCD_RES_X) + x;
            uint8_t pallete_index = lcd_frame_buffer[index / 4] >> ((index % 4) * 2); // 2 bits corresponding to this pixel's color
            pixels[index] = LCD_PALLETE[pallete_index & 0x03];
        }
    }
    UpdateTexture(tex, pixels); // refresh texture

    // Draw texture to screen
    DrawTexturePro(tex,
        (Rectangle){ 0, 0, LCD_RES_X, LCD_RES_Y}, // Use full tex image
        (Rectangle){
            ((GetScreenWidth() - (LCD_RES_X) * pixel_scale) / 2), // Centered in X axis
            ((GetScreenHeight() - (LCD_RES_Y) * pixel_scale) / 2), // Centered in Y axis
            pixel_scale * LCD_RES_X, pixel_scale * LCD_RES_Y // Dimensions = Resolution * Scale
        },
        (Vector2){ 0, 0 }, // texture origin? don't touch..
        0.0f, // rotation? also leave alone
        WHITE // Tint color, white indicates no tint
    );
}

void unload_lcd() {
    UnloadTexture(tex);
    free(pixels);
}

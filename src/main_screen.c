/**********************************************************************************************
*
*   raylib - Advance Game template
*
*   Main Screen Functions Definitions (Init, Update, Draw, Unload)
*
*   Copyright (c) 2014-2022 Ramon Santamaria (@raysan5)
*
*   This software is provided "as-is", without any express or implied warranty. In no event
*   will the authors be held liable for any damages arising from the use of this software.
*
*   Permission is granted to anyone to use this software for any purpose, including commercial
*   applications, and to alter it and redistribute it freely, subject to the following restrictions:
*
*     1. The origin of this software must not be misrepresented; you must not claim that you
*     wrote the original software. If you use this software in a product, an acknowledgment
*     in the product documentation would be appreciated but is not required.
*
*     2. Altered source versions must be plainly marked as such, and must not be misrepresented
*     as being the original software.
*
*     3. This notice may not be removed or altered from any source distribution.
*
**********************************************************************************************/

#include "ppu.h"
#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "screens.h"
#include "file.h"
#include "emu_core.h"

/* ---- Module Variables Definition (local) ---- */
static int framesCounter = 0;
static int finishScreen = 0;
static bool load_rom = false;
static char textBuffer[128];
static int textBufferIndex = 0;
static FILE* rom_file;

// Main Screen Initialization logic
void InitMainScreen(void) {
    framesCounter = 0;
    finishScreen = 0;
}

// Main Screen Update logic
void UpdateMainScreen(void) {
    if (load_rom) {
    #if defined(PLATFORM_WEB)
    if (web_rom_written()) { // Need to wait on Async JS on Web version
    #endif
        rom_file = get_rom();
        if (rom_file != NULL) {
            TraceLog(LOG_INFO, "File Selected: %s", rom_file);
            load_boot_rom();
            rom_loaded = true;
        } else {
            TraceLog(LOG_INFO, "File Failed to Load");
        }
        load_rom = false;
    #if defined(PLATFORM_WEB)
    }
    #endif
    }
}

float scale_slider_val = 1;
float speed_slider_val = 1;
// Main Screen Draw logic
void DrawMainScreen(void) {
    // DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), RAYWHITE);
    // Create a button
    if (GuiButton((Rectangle){ 10, 10, 100, 50 }, "Select a ROM")) {
        if (rom_file != NULL) fclose(rom_file);
        open_file_dialog();
        load_rom = true;
        rom_loaded = false;
        start = false;
    }

    if (rom_loaded) {
        // lcd_frame_buffer[0] = 0b11000011;
        if (!start && GuiButton((Rectangle){ 10, 70, 100, 50 }, "Start")) {
            start = true;
        }
        DrawText("Display Scale", 120, 10, 10, BLACK);
        GuiSlider((Rectangle){120, 30, 100, 10}, "", "", &scale_slider_val, 1, 5);
        pixel_scale = (int)scale_slider_val;
        DrawText("Clock Speed", 120, 50, 10, BLACK);
        GuiSlider((Rectangle){120, 70, 100, 10}, "", "", &speed_slider_val, 0.1f, 5);
#ifdef _DEBUG
        clock_speed_multiplier = (2.5e-7) * speed_slider_val;
#else 
        clock_speed_multiplier = speed_slider_val;
#endif
        DrawRectangle((GetScreenWidth() - (LCD_RES_X + 2) * pixel_scale) / 2, (GetScreenHeight() - (LCD_RES_Y + 2) * pixel_scale) / 2, (LCD_RES_X + 2) * pixel_scale, (LCD_RES_Y + 2) * pixel_scale, BLACK);
        if (start) render_lcd();
    }

}

// Main Screen Unload logic
void UnloadMainScreen(void) {
    if (rom_file != NULL) fclose(rom_file);
}

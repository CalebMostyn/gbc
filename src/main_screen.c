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

#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "screens.h"
#include "file.h"

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int framesCounter = 0;
static int finishScreen = 0;
static bool load_rom = false;
static char textBuffer[128];
static int textBufferIndex = 0;
static FILE* rom_file;

//----------------------------------------------------------------------------------
// Main Screen Functions Definition
//----------------------------------------------------------------------------------

// Main Screen Initialization logic
void InitMainScreen(void)
{
    // TODO: Initialize GAMEPLAY screen variables here!
    framesCounter = 0;
    finishScreen = 0;
}

// Main Screen Update logic
void UpdateMainScreen(void)
{
    // TODO: Update GAMEPLAY screen variables here!

    if (load_rom) {
    #if defined(PLATFORM_WEB)
    if (web_rom_written()) { // Need to wait on Async JS on Web version
    #endif
        rom_file = get_rom();
        if (rom_file != NULL){
            TraceLog(LOG_INFO, "File Selected: %s", rom_file);
        } else {
            TraceLog(LOG_INFO, "File Failed to Load");
        }
        load_rom = false;
    #if defined(PLATFORM_WEB)
    }
    #endif
    }
}

// Main Screen Draw logic
void DrawMainScreen(void)
{
    // TODO: Draw GAMEPLAY screen here!
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), RAYWHITE);
    // Create a button
    if (GuiButton((Rectangle){ 10, 10, 100, 50 }, "Select a ROM"))
    {
        if (rom_file != NULL) { fclose(rom_file); }
        open_file_dialog();
        load_rom = true;
    }

    if (rom_file != NULL) {
        if (GuiButton((Rectangle){ 10, 70, 100, 50 }, "Print From File"))
        {
            // int c = fgetc(rom_file);
            // textBuffer[textBufferIndex++] = (char)c;
            size_t bytesRead = fread(textBuffer, 1, sizeof(textBuffer) - 1, rom_file);
            textBuffer[bytesRead] = '\0'; // null-terminate
        }
    }
    

    // Display a text box where the user can see and edit the buffer
    GuiTextBox((Rectangle){ 200, 250, 400, 30 }, textBuffer, 128, false);

}

// Main Screen Unload logic
void UnloadMainScreen(void)
{
    // TODO: Unload GAMEPLAY screen variables here!
    if (rom_file != NULL) fclose(rom_file);
}

// Main Screen should finish?
int FinishMainScreen(void)
{
    return finishScreen;
}

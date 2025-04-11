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
#include "screens.h"

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#else
    #include "tinyfiledialogs.h"
#endif

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int framesCounter = 0;
static int finishScreen = 0;

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

    // Press enter or tap to change to ENDING screen
#if defined(PLATFORM_WEB)
    if (IsKeyPressed(KEY_ENTER)) {
        EM_ASM({
            document.getElementById('fileInput').click();
        });
    }
#else
    if (IsKeyPressed(KEY_ENTER)) {
        char const * filter[1] = {"*.gb"};
        const char *file = tinyfd_openFileDialog(
            "Select a ROM", "", 1, filter, "Game Boy ROMs", 0);
        if (file) {
            TraceLog(LOG_INFO, "File selected: %s", file);
        }
    }
#endif
}

// Main Screen Draw logic
void DrawMainScreen(void)
{
    // TODO: Draw GAMEPLAY screen here!
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), PURPLE);
    Vector2 pos = { 20, 10 };
    DrawText("GAMEPLAY SCREEN", 20, 10, 30, MAROON);
    DrawText("PRESS ENTER or TAP to JUMP to ENDING SCREEN", 130, 220, 20, MAROON);
}

// Main Screen Unload logic
void UnloadMainScreen(void)
{
    // TODO: Unload GAMEPLAY screen variables here!
}

// Main Screen should finish?
int FinishMainScreen(void)
{
    return finishScreen;
}
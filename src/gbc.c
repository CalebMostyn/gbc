/*******************************************************************************************
*
*   GBC: An Original Game Boy Emulator
*
*   This game has been created using raylib (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2021 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"
#include "screens.h"    // NOTE: Declares global (extern) variables and screens functions
#include <stddef.h>


#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

/* ---- Constants ---- */
#define CPU_CLOCK_HZ 4194304     // 4 MHz

/* ---- Shared Variables Definition (global) ---- */
// NOTE: Those variables are shared between modules through screens.h
GameScreen currentScreen = LOGO;
float clock_speed_multiplier = 1;

/* ---- Local Variables Definition (local to this module) ---- */
static const int screenWidth = 800;
static const int screenHeight = 450;
static double last_time = 0;
static double cpu_cycles_to_run = 0;
// Required variables to manage screen transitions (fade-in, fade-out)
static float transAlpha = 0.0f;
static bool onTransition = false;
static bool transFadeOut = false;
static int transFromScreen = -1;
static GameScreen transToScreen = UNKNOWN;

/* ---- Local Functions Declaration ---- */
static void ChangeToScreen(int screen);     // Change to screen, no transition effect

static void TransitionToScreen(int screen); // Request transition to next screen
static void UpdateTransition(void);         // Update transition effect
static void DrawTransition(void);           // Draw transition effect (full-screen rectangle)

static void UpdateDrawFrame(void);          // Update and draw one frame

int main(void)
{
    /* ---- Initialization ---- */
    InitWindow(screenWidth, screenHeight, "GBC - Original Game Boy Emulator");

    // Initialize audio device
    InitAudioDevice();

    // Load global data (assets that must be available in all screens, i.e. font)
    // font = LoadFont("resources/mecha.png");

    // SetMusicVolume(music, 1.0f);
    // PlayMusicStream(music);

    // Setup and init first screen
    currentScreen = LOGO; // refactor? I don't plan on having too many screens that they need such management
    InitLogoScreen();

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
    SetTargetFPS(60);       // Set our game to run at 60 frames-per-second

    // Main game loop
    while (!WindowShouldClose()) { // Detect window close button or ESC key
        UpdateDrawFrame();
    }
#endif

    /* ---- De-Initialization ---- */
    // Unload current screen data before closing
    switch (currentScreen) {
        case LOGO: UnloadLogoScreen(); break;
        case MAIN: UnloadMainScreen(); break;
        default: break;
    }

    // Unload global data loaded
    // UnloadFont(font);

    CloseAudioDevice();     // Close audio context

    CloseWindow();          // Close window and OpenGL context

    return 0;
}

// Change to next screen, no transition
static void ChangeToScreen(int screen) {
    // Unload current screen
    switch (currentScreen) {
        case LOGO: UnloadLogoScreen(); break;
        case MAIN: UnloadMainScreen(); break;
        default: break;
    }

    // Init next screen
    switch (screen) {
        case LOGO: InitLogoScreen(); break;
        case MAIN: InitMainScreen(); break;
        default: break;
    }

    currentScreen = screen;
}

// Request transition to next screen
static void TransitionToScreen(int screen) {
    onTransition = true;
    transFadeOut = false;
    transFromScreen = currentScreen;
    transToScreen = screen;
    transAlpha = 0.0f;
}

// Update transition effect (fade-in, fade-out)
static void UpdateTransition(void) {
    if (!transFadeOut) {
        transAlpha += 0.05f;

        // NOTE: Due to float internal representation, condition jumps on 1.0f instead of 1.05f
        // For that reason we compare against 1.01f, to avoid last frame loading stop
        if (transAlpha > 1.01f) {
            transAlpha = 1.0f;

            // Unload current screen
            switch (transFromScreen) {
                case LOGO: UnloadLogoScreen(); break;
                default: break;
            }

            // Load next screen
            switch (transToScreen) {
                case LOGO: InitLogoScreen(); break;
                case MAIN: InitMainScreen(); break;
                default: break;
            }

            currentScreen = transToScreen;

            // Activate fade out effect to next loaded screen
            transFadeOut = true;
        }
    } else { // Transition fade out logic
        transAlpha -= 0.02f;

        if (transAlpha < -0.01f) {
            transAlpha = 0.0f;
            transFadeOut = false;
            onTransition = false;
            transFromScreen = -1;
            transToScreen = UNKNOWN;
        }
    }
}

// Draw transition effect (full-screen rectangle)
static void DrawTransition(void) {
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, transAlpha));
}

// Update and draw game frame
// static void UpdateDrawFrame(void) {
void emulate_clock_cycle() {
    1 + 1;
}

void UpdateDrawFrame(void) {
    // Timing dependent emu control
    if (last_time == 0) {
        last_time = GetTime();
    }

    // find delta time
    double current_time = GetTime();
    double delta_time = current_time - last_time;
    last_time = current_time;

    // cycles to run this frame = (delta_time / clock ticks per second) * speed_multiplier
    cpu_cycles_to_run += delta_time * CPU_CLOCK_HZ * clock_speed_multiplier;

    int num_cycles = 0;
    while (cpu_cycles_to_run >= 1.0) {
        cpu_cycles_to_run -= 1.0;
        emulate_clock_cycle(); // Runs 1 cycle (or instruction, depending on implementation)
        num_cycles++;
    }
    #ifdef _DEBUG 
    DrawText(TextFormat("Cycles This Frame: %d", num_cycles), screenWidth - 350, 10, 20, RED);
    #endif
    num_cycles = 0;
    if (!onTransition) {
        switch(currentScreen) {
            case LOGO:
                UpdateLogoScreen();
                if (FinishLogoScreen()) TransitionToScreen(MAIN);
                break;
            case MAIN: UpdateMainScreen(); break;
            default: break;
        }
    } else {
        UpdateTransition();    // Update transition (fade-in, fade-out)
    }
    BeginDrawing();

        ClearBackground(RAYWHITE);

        switch(currentScreen) {
            case LOGO: DrawLogoScreen(); break;
            case MAIN: DrawMainScreen(); break;
            default: break;
        }

        // Draw full screen rectangle in front of everything
        if (onTransition) DrawTransition();

    // Can't remove this? Without it FPS is drawn for some reason
    // DrawFPS(10,10);

    EndDrawing();
}

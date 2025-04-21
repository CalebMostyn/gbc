#ifndef FILE_H
#define FILE_H

#include <stddef.h>
#include <stdio.h>
#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#else
    #include "tinyfiledialogs.h"
#endif

// File location of ROM (as selected by sys dialog)
static const char *file;

// JS handles loading into MEMFS, then C code can proceed after confirm_rom_written is called
#if defined(PLATFORM_WEB)
#define WEB_ROM_LOCATION "/rom.gb"
static bool memfs_rom_written = false;

void EMSCRIPTEN_KEEPALIVE confirm_rom_written() {
    memfs_rom_written = true;
    file = WEB_ROM_LOCATION;
}
bool web_rom_written() {
    return memfs_rom_written;
}
#endif

// If valid GB ROM, returns opened file pointer, otherwise NULL
FILE* get_rom() {
    if (file) {
        // TODO: ROM Validation?
        return (FILE*)fopen(file, "rb");
    }
    return NULL;
}

// Opens Sys File Dialog for User to select a ROM
void open_file_dialog() {
#if defined(PLATFORM_WEB)
    EM_ASM({
        document.getElementById('fileInput').click();
    });
#else
    char const * filter[1] = {"*.gb"};
    file = tinyfd_openFileDialog(
        "Select a ROM", "", 1, filter, "Game Boy ROMs", 0);
#endif
}


#endif // FILE_H

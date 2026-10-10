#include "file.h"
#include "memory_bus.h"
#include <stddef.h>
#include <stdio.h>
#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#else
    #include "tinyfiledialogs.h"
#endif
#include "cpu.h"
#include <stdlib.h>

// File location of ROM (as selected by sys dialog)
static const char *file;

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

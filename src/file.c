#include "file.h"
#include <stddef.h>
#include <stdio.h>
#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#else
    #include "tinyfiledialogs.h"
#endif
#include "cpu.h"
#include <stdlib.h>

extern uint8_t memory[0x10000]; // 16 bit addresses 0x0000 - 0xFFFF

// File location of ROM (as selected by sys dialog)
static const char *file;
FILE* cartridge_rom;

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
        cartridge_rom = (FILE*)fopen(file, "rb");
        return cartridge_rom;
    }
    return NULL;
}
// General-purpose ROM loader with a custom memory offset
void load_rom_to_mem(FILE* rom_pointer, uint16_t start_address) {
    if (!rom_pointer) {
        fprintf(stderr, "Error: ROM pointer is NULL.\n");
        return;
    }

    size_t address = start_address;
    int byte;

    while ((byte = fgetc(rom_pointer)) != EOF && address < 0x10000) {
        memory[address++] = (uint8_t)byte;
    }

    if (address >= 0x10000) {
        fprintf(stderr, "Warning: ROM overflowed memory (truncated at 0xFFFF).\n");
    }
}

void load_boot_rom() {
    FILE* rom = fopen("resources/dmg_boot.bin", "rb");
    if (!rom) {
        fprintf(stderr, "Failed to open boot ROM: %s\n", "resources/dmg_boot.bin");
        exit(EXIT_FAILURE);
    }

    load_rom_to_mem(rom, 0x0000); // Load boot ROM at start
    fclose(rom);
}

void load_cartridge_rom(int boot_rom_loaded) {
    if (!cartridge_rom) {
        fprintf(stderr, "Failed to open cartridge ROM: %s\n", file);
        exit(EXIT_FAILURE);
    }

    uint16_t offset = boot_rom_loaded ? 0x0100 : 0x0000;
    load_rom_to_mem(cartridge_rom, offset);
    fclose(cartridge_rom);
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

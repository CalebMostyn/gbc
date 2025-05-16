#ifndef FILE_H
#define FILE_H

#include <stdio.h>
#include <stdint.h>

// Declarations only:
void load_rom_to_mem(FILE* rom_pointer, uint16_t start_addr);
void load_boot_rom();
void load_cartridge_rom(int boot_rom_loaded);
void open_file_dialog();
FILE* get_rom();

#endif // FILE_H

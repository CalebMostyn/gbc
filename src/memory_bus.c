#include "memory_bus.h"
#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint8_t memory[0x10000]; // 16 bit addresses 0x0000 - 0xFFFF

bool boot_rom_enabled = false;
uint8_t boot_memory[0x100]; // Boot ROM is only 256 bytes

char serial_output[4096];
size_t serial_output_len = 0;

uint8_t memory_bus_read(uint16_t addr) {
    if (addr < 0x100 && boot_rom_enabled) {
        return boot_memory[addr];
    }

    if (addr >= 0xE000 && addr <= 0xFDFF) {
        // Echo RAM
        addr -= 0x2000; // map to 0xE000 to 0xC000, and so on
    }
    if (addr >= 0xFEA0 && addr <= 0xFEFF) return 0; // Unused mem space
    if (addr == 0xFF44) return 0x90; // LY register for LCD..?
    return memory[addr];
}

#define SB 0xFF01
#define SC 0xFF02
void memory_bus_write(uint16_t addr, uint8_t val) {
    if (addr == 0xFF50 && val != 0 && boot_rom_enabled) {
        boot_rom_enabled = false;
    }

    if (addr < 0x8000) return; // ignore writes to cartridge
    if (addr >= 0xE000 && addr <= 0xFDFF) {
        // Echo RAM
        addr -= 0x2000; // map to 0xE000 to 0xC000, and so on
    }
    if (addr >= 0xFEA0 && addr <= 0xFEFF) return; // Unused mem space
    if (addr == SC) {
        if (val == 0x81) {
            // write to serial output
            uint8_t ch = memory[SB];

            serial_output[serial_output_len++] = ch;
            serial_output[serial_output_len] = '\0';

            // Emulate transfer completing.
            val = 0x01;
        }
    }
    memory[addr] = val;
}

uint8_t dma_read(uint16_t addr) {
    return memory[addr];
}

void dma_write(uint16_t addr, uint8_t val) {
    memory[addr] = val;
}

void load_boot_rom() {
    boot_rom_enabled = true;
    FILE* rom = fopen("resources/dmg_boot.bin", "rb");
    if (!rom) {
        fprintf(stderr, "Failed to open boot ROM: %s\n", "resources/dmg_boot.bin");
        exit(EXIT_FAILURE);
    }

    memset(boot_memory, 0, sizeof(boot_memory));
    size_t n = fread(boot_memory, sizeof(uint8_t), sizeof(boot_memory), rom);

    if (n != sizeof(boot_memory)) {
        fprintf(stderr, "Boot ROM short read: got %zu bytes, expected 256\n", n);
        exit(EXIT_FAILURE);
    }

    fclose(rom);
}

// General-purpose ROM loader with a custom memory offset
void load_cartridge_rom(FILE* rom_pointer, uint16_t start_address) {
    if (!rom_pointer) {
        fprintf(stderr, "Error: ROM pointer is NULL.\n");
        return;
    }

    memset(memory, 0, sizeof(memory));

    size_t address = start_address;
    int byte;

    while ((byte = fgetc(rom_pointer)) != EOF && address < 0x10000) {
        memory[address++] = (uint8_t)byte;
    }

    if (address >= 0x10000) {
        fprintf(stderr, "Warning: ROM overflowed memory (truncated at 0xFFFF).\n");
    }
}

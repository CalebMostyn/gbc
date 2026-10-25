#include "memory_bus.h"
#include "cpu.h"
#include "timer.h"
#include "ppu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// temp
uint8_t joypad_reg_select = 0x30;

uint8_t memory[0x10000]; // 16 bit addresses 0x0000 - 0xFFFF

bool boot_rom_enabled = false;
uint8_t boot_memory[0x100]; // Boot ROM is only 256 bytes

char serial_output[4096];
size_t serial_output_len = 0;

uint8_t memory_bus_read(uint16_t addr) {
    if (addr < 0x100 && boot_rom_enabled) {
        return boot_memory[addr];
    }

    if (addr >= VRAM_BEGIN && addr <= VRAM_END) {
        return vram[addr - VRAM_BEGIN];
    }
    if (addr >= 0xE000 && addr <= 0xFDFF) {
        // Echo RAM
        addr -= 0x2000; // map to 0xE000 to 0xC000, and so on
    }
    if (addr >= OAM_BEGIN && addr <= OAM_END) {
        return oam[addr - OAM_BEGIN];
    }
    if (addr >= 0xFEA0 && addr <= 0xFEFF) return 0; // Unused mem space
    // joypad register, temp
    if (addr == 0xFF00) {
        return 0xC0 | joypad_reg_select| 0x0F;
    }
    // timer registers
    if (addr == TIMER_REG_DIV) {
        return timer_reg_div;
    }
    if (addr == TIMER_REG_TIMA) {
        return timer_reg_tima;
    }
    if (addr == TIMER_REG_TMA) {
        return timer_reg_tma;
    }
    if (addr == TIMER_REG_TAC) {
        return timer_reg_tac;
    }
    if (addr == PPU_REG_LCDC) {
        return ppu_reg_lcdc;
    }
    if (addr == PPU_REG_STAT) {
        return ppu_reg_stat;
    }
    if (addr == PPU_REG_SCY) {
        return ppu_reg_scy;
    }
    if (addr == PPU_REG_SCX) {
        return ppu_reg_scx;
    }
    if (addr == PPU_REG_LY) {
        return ppu_reg_ly;
    }
    if (addr == PPU_REG_LYC) {
        return ppu_reg_lyc;
    }
    if (addr == PPU_REG_DMA) {
        return ppu_reg_dma;
    }
    if (addr == PPU_REG_BGP) {
        return ppu_reg_bgp;
    }
    if (addr == PPU_REG_OBP0) {
        return ppu_reg_obp0;
    }
    if (addr == PPU_REG_OBP1) {
        return ppu_reg_obp1;
    }
    if (addr == PPU_REG_WY) {
        return ppu_reg_wy;
    }
    if (addr == PPU_REG_WX) {
        return ppu_reg_wx;
    }
    return memory[addr];
}

#define SB 0xFF01
#define SC 0xFF02
void memory_bus_write(uint16_t addr, uint8_t val) {
    if (addr == 0xFF50 && val != 0 && boot_rom_enabled) {
        boot_rom_enabled = false;
    }

    if (addr < 0x8000) return; // ignore writes to cartridge
    if (addr >= VRAM_BEGIN && addr <= VRAM_END) {
        vram[addr - VRAM_BEGIN] = val;
        return;
    }
    if (addr >= 0xE000 && addr <= 0xFDFF) {
        // Echo RAM
        addr -= 0x2000; // map to 0xE000 to 0xC000, and so on
    }
    if (addr >= OAM_BEGIN && addr <= OAM_END) {
        oam[addr - OAM_BEGIN] = val;
        return;
    }
    if (addr >= 0xFEA0 && addr <= 0xFEFF) return; // Unused mem space
    if (addr == SC) {
        if (val == 0x81) { // Transfer enabled, write
            // write to serial output
            uint8_t ch = memory[SB];

            serial_output[serial_output_len++] = ch;
            serial_output[serial_output_len] = '\0';

            // Emulate transfer completing.
            val = 0x01;
        }
    }
    // joypad register, temp
    if (addr == 0xFF00) {
        joypad_reg_select = val & 0x30;
    }
    // timer registers
    if (addr == TIMER_REG_DIV) {
        timer_reg_div = 0; // any write sets to 0
        return;
    }
    if (addr == TIMER_REG_TIMA) {
        if (tima_reloading) return; // ignored on cycle of reload
        timer_reg_tima = val;
        tima_pending_reload = false; // writes cancel any pending reloaad
        return;
    }
    if (addr == TIMER_REG_TMA) {
        timer_reg_tma = val;
        if (tima_reloading) {
            // writes during cycle of reload set tima to new val
            timer_reg_tima = val;
        }
        return;
    }
    if (addr == TIMER_REG_TAC) {
        timer_reg_tac = val;
        return;
    }
    if (addr == PPU_REG_LCDC) {
        ppu_reg_lcdc = val;
        return;
    }
    if (addr == PPU_REG_STAT) {
        ppu_reg_stat = val;
        return;
    }
    if (addr == PPU_REG_SCY) {
        ppu_reg_scy = val;
        return;
    }
    if (addr == PPU_REG_SCX) {
        ppu_reg_scx = val;
        return;
    }
    if (addr == PPU_REG_LY) {
        ppu_reg_ly = val;
        return;
    }
    if (addr == PPU_REG_LYC) {
        ppu_reg_lyc = val;
        return;
    }
    if (addr == PPU_REG_DMA) {
        ppu_reg_dma = val;
        return;
    }
    if (addr == PPU_REG_BGP) {
        ppu_reg_bgp = val;
        return;
    }
    if (addr == PPU_REG_OBP0) {
        ppu_reg_obp0 = val;
        return;
    }
    if (addr == PPU_REG_OBP1) {
        ppu_reg_obp1 = val;
        return;
    }
    if (addr == PPU_REG_WY) {
        ppu_reg_wy = val;
        return;
    }
    if (addr == PPU_REG_WX) {
        ppu_reg_wx = val;
        return;
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

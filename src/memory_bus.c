#include "memory_bus.h"
#include "cpu.h"
#include <stdio.h>

uint8_t memory[0x10000]; // 16 bit addresses 0x0000 - 0xFFFF

char serial_output[4096];
size_t serial_output_len = 0;

uint8_t memory_bus_read(uint16_t addr) {
    if (addr == 0xFF44) return 0x90;
    return memory[addr];
}
#define SB 0xFF01
#define SC 0xFF02
void memory_bus_write(uint16_t addr, uint8_t val) {
    if (addr < 0x8000) return;
    if (addr == SC) {
        if (val == 0x81) {
            // write to serial output
            uint8_t ch = dma_read(SB);

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

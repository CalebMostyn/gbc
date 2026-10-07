#include "memory_bus.h"

uint8_t memory[0x10000]; // 16 bit addresses 0x0000 - 0xFFFF

// to fix:
// file.c (remove entirely?)

uint8_t memory_bus_read(uint16_t addr) {
    return memory[addr];
}

void memory_bus_write(uint16_t addr, uint8_t val) {
    memory[addr] = val;
}

uint8_t dma_read(uint16_t addr) {
    return memory[addr];
}

void dma_write(uint16_t addr, uint8_t val) {
    memory[addr] = val;
}

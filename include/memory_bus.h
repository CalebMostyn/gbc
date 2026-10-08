#ifndef MEMORY_BUS_H
#define MEMORY_BUS_H

#include <stddef.h>
#include <stdint.h>

extern char serial_output[4096];
extern size_t serial_output_len;

// Emualates real memory read/writes, has potential side effects
uint8_t memory_bus_read(uint16_t addr);
void memory_bus_write(uint16_t addr, uint8_t val);
// No side effects, use with caution
uint8_t dma_read(uint16_t addr);
void dma_write(uint16_t addr, uint8_t val);

#endif // MEMORY_BUS_H

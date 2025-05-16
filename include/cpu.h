#ifndef CPU_H
#define CPU_H

#include <stdint.h>

void clock_cpu(); // emulate a clock tick on the cpu

typedef union {
    struct {
        uint8_t l;
        uint8_t r;
    };
    uint16_t lr;
} reg8_pair;

typedef struct {
    bool IME; // interrupt flag
    reg8_pair AF;
    reg8_pair BC;
    reg8_pair DE;
    reg8_pair HL;
    uint16_t PC;
    uint16_t SP;
} register_file;

#define IE_ADDR 0xFFFF
#define IF_ADDR 0xFF0F

#define ZERO_FLAG_OFFSET 7
#define SUB_FLAG_OFFSET 6
#define HALF_CARRY_FLAG_OFFSET 5
#define CARRY_FLAG_OFFSET 4

extern register_file rf;
extern uint8_t memory[0x10000]; // 16 bit addresses 0x0000 - 0xFFFF

#endif // CPU_H

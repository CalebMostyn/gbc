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
    reg8_pair IR_IE;
    reg8_pair AF;
    reg8_pair BC;
    reg8_pair DE;
    reg8_pair HL;
    uint16_t PC;
    uint16_t SP;
} register_file;

extern register_file rf;

#endif // CPU_H

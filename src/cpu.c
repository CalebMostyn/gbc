#include "cpu.h"
#include "instructions.h"
#include <stdint.h>
#include <stddef.h>
#include "raylib.h"

register_file rf;

uint8_t inst_mem[500] = {
                        0x40, 0x06, 0x46, 0x70, 0x36,
                        0x0A, 0x1A, 0x02, 0x12, 0xFA,
                        0xEA, 0xF2, 0xE2, 0xF0, 0xE0,
                        0x3A, 0x32, 0x2A, 0x22, 0x01,
                        0x08, 0xF9, 0xC5, 0xC1, 0xF8
                        };

uint8_t* opcode = NULL;
void clock_cpu() {
    // TODO: CPU functionality

    // first, execute the previously fetched instruction
    if (opcode != NULL) {
        // instruction has been fetched
        if(LD_R_HLA(*opcode)) {
            TraceLog(LOG_INFO, "Load Register from HL indirect: %d", *opcode);
            opcode = NULL;
        } else if(LD_HLA_R(*opcode)) {
            TraceLog(LOG_INFO, "Load HL indirect from Register: %d", *opcode);
            opcode = NULL;
        } else if(LD_HLA_I(*opcode)) {
            TraceLog(LOG_INFO, "Load HL indirect Immediate: %d", *opcode);
            opcode = NULL;
        } else if (LD_R_R(*opcode)) {
            TraceLog(LOG_INFO, "Load Register from Register: %d", *opcode);
            opcode = NULL;
        } else if(LD_R_I(*opcode)) {
            TraceLog(LOG_INFO, "Load Register from Immediate: %d", *opcode);
            opcode = NULL;
        } else if(LD_A_BCA(*opcode)) {
            TraceLog(LOG_INFO, "Load Accumulator from BC indirect: %d", *opcode);
            opcode = NULL;
        } else if(LD_A_DEA(*opcode)) {
            TraceLog(LOG_INFO, "Load Accumulator from DE indirect: %d", *opcode);
            opcode = NULL;
        } else if(LD_BCA_A(*opcode)) {
            TraceLog(LOG_INFO, "Load BC indirect from Accumulator: %d", *opcode);
            opcode = NULL;
        } else if(LD_DEA_A(*opcode)) {
            TraceLog(LOG_INFO, "Load DE indirect from Accumulator: %d", *opcode);
            opcode = NULL;
        } else if(LD_A_IIA(*opcode)) {
            TraceLog(LOG_INFO, "Load Accumulator from Immediate indirect: %d", *opcode);
            opcode = NULL;
        } else if(LD_IIA_A(*opcode)) {
            TraceLog(LOG_INFO, "Load Immediate indirect from Accumulator: %d", *opcode);
            opcode = NULL;
        } else if(LD_A_CA(*opcode)) {
            TraceLog(LOG_INFO, "Load Accumulator from 0xFF00 + C indirect: %d", *opcode);
            opcode = NULL;
        } else if(LD_CA_A(*opcode)) {
            TraceLog(LOG_INFO, "Load 0xFF00 + C indirect from Accumulator: %d", *opcode);
            opcode = NULL;
        } else if(LD_A_IA(*opcode)) {
            TraceLog(LOG_INFO, "Load Accumulator from 0xFF00 + Immediate indirect: %d", *opcode);
            opcode = NULL;
        } else if(LD_IA_A(*opcode)) {
            TraceLog(LOG_INFO, "Load 0xFF00 + Immediate indirect from Accumulator: %d", *opcode);
            opcode = NULL;
        } else if(LD_A_HLA_DEC(*opcode)) {
            TraceLog(LOG_INFO, "Load Accumulator from HL indirect (decrement): %d", *opcode);
            opcode = NULL;
        } else if(LD_HLA_A_DEC(*opcode)) {
            TraceLog(LOG_INFO, "Load HL indirect from Accumulator (decrement): %d", *opcode);
            opcode = NULL;
        } else if(LD_A_HLA_INC(*opcode)) {
            TraceLog(LOG_INFO, "Load Accumulator from HL indirect (increment): %d", *opcode);
            opcode = NULL;
        } else if(LD_HLA_A_INC(*opcode)) {
            TraceLog(LOG_INFO, "Load HL indirect from Accumulator (increment): %d", *opcode);
            opcode = NULL;
        } else if(LD_RP_II(*opcode)) {
            TraceLog(LOG_INFO, "Load Register Pair from Immediate", *opcode);
            opcode = NULL;
        } else if(LD_IIA_SP(*opcode)) {
            TraceLog(LOG_INFO, "Load Immediate indirect from SP", *opcode);
            opcode = NULL;
        } else if(LD_SP_HL(*opcode)) {
            TraceLog(LOG_INFO, "Load SP from HL", *opcode);
            opcode = NULL;
        } else if(PUSH(*opcode)) {
            TraceLog(LOG_INFO, "Push to stack from Register", *opcode);
            opcode = NULL;
        } else if(POP(*opcode)) {
            TraceLog(LOG_INFO, "Pop from stack to Register", *opcode);
            opcode = NULL;
        } else if(LD_HL_SPE(*opcode)) {
            TraceLog(LOG_INFO, "Load HL from SP + e", *opcode);
            opcode = NULL;
        } else {
            // undefined opcode
            opcode = NULL;
        }
    }
    // then, if execution is finished, fetch the next instruction
    if (opcode == NULL) {
        // previous execution is done
        opcode = &inst_mem[rf.PC % 500];
        rf.PC++;
    }
}

#include "cpu.h"
#include "instructions.h"
#include <stdint.h>
#include <stddef.h>
#include "raylib.h"

register_file rf;
bool f_zero, f_sub, f_carry, f_hcarry = false;

uint8_t memory[0x10000] = {
0x2A, 0x00, 0x1A, 0x3C
// 0xCB, 0x00, 0xCB, 0x01, 0xCB, 0x02, 0xCB, 0x03, 0xCB, 0x04, 0xCB, 0x05, 0xCB, 0x06, 0xCB, 0x07, 0xCB, 0x08, 0xCB, 0x09, 0xCB, 0x0A, 0xCB, 0x0B, 0xCB, 0x0C, 0xCB, 0x0D, 0xCB, 0x0E, 0xCB, 0x0F,
// 0xCB, 0x10, 0xCB, 0x11, 0xCB, 0x12, 0xCB, 0x13, 0xCB, 0x14, 0xCB, 0x15, 0xCB, 0x16, 0xCB, 0x17, 0xCB, 0x18, 0xCB, 0x19, 0xCB, 0x1A, 0xCB, 0x1B, 0xCB, 0x1C, 0xCB, 0x1D, 0xCB, 0x1E, 0xCB, 0x1F,
// 0xCB, 0x20, 0xCB, 0x21, 0xCB, 0x22, 0xCB, 0x23, 0xCB, 0x24, 0xCB, 0x25, 0xCB, 0x26, 0xCB, 0x27, 0xCB, 0x28, 0xCB, 0x29, 0xCB, 0x2A, 0xCB, 0x2B, 0xCB, 0x2C, 0xCB, 0x2D, 0xCB, 0x2E, 0xCB, 0x2F,
// 0xCB, 0x30, 0xCB, 0x31, 0xCB, 0x32, 0xCB, 0x33, 0xCB, 0x34, 0xCB, 0x35, 0xCB, 0x36, 0xCB, 0x37, 0xCB, 0x38, 0xCB, 0x39, 0xCB, 0x3A, 0xCB, 0x3B, 0xCB, 0x3C, 0xCB, 0x3D, 0xCB, 0x3E, 0xCB, 0x3F,
// 0xCB, 0x40, 0xCB, 0x41, 0xCB, 0x42, 0xCB, 0x43, 0xCB, 0x44, 0xCB, 0x45, 0xCB, 0x46, 0xCB, 0x47, 0xCB, 0x48, 0xCB, 0x49, 0xCB, 0x4A, 0xCB, 0x4B, 0xCB, 0x4C, 0xCB, 0x4D, 0xCB, 0x4E, 0xCB, 0x4F,
// 0xCB, 0x50, 0xCB, 0x51, 0xCB, 0x52, 0xCB, 0x53, 0xCB, 0x54, 0xCB, 0x55, 0xCB, 0x56, 0xCB, 0x57, 0xCB, 0x58, 0xCB, 0x59, 0xCB, 0x5A, 0xCB, 0x5B, 0xCB, 0x5C, 0xCB, 0x5D, 0xCB, 0x5E, 0xCB, 0x5F,
// 0xCB, 0x60, 0xCB, 0x61, 0xCB, 0x62, 0xCB, 0x63, 0xCB, 0x64, 0xCB, 0x65, 0xCB, 0x66, 0xCB, 0x67, 0xCB, 0x68, 0xCB, 0x69, 0xCB, 0x6A, 0xCB, 0x6B, 0xCB, 0x6C, 0xCB, 0x6D, 0xCB, 0x6E, 0xCB, 0x6F,
// 0xCB, 0x70, 0xCB, 0x71, 0xCB, 0x72, 0xCB, 0x73, 0xCB, 0x74, 0xCB, 0x75, 0xCB, 0x76, 0xCB, 0x77, 0xCB, 0x78, 0xCB, 0x79, 0xCB, 0x7A, 0xCB, 0x7B, 0xCB, 0x7C, 0xCB, 0x7D, 0xCB, 0x7E, 0xCB, 0x7F,
// 0xCB, 0x80, 0xCB, 0x81, 0xCB, 0x82, 0xCB, 0x83, 0xCB, 0x84, 0xCB, 0x85, 0xCB, 0x86, 0xCB, 0x87, 0xCB, 0x88, 0xCB, 0x89, 0xCB, 0x8A, 0xCB, 0x8B, 0xCB, 0x8C, 0xCB, 0x8D, 0xCB, 0x8E, 0xCB, 0x8F,
// 0xCB, 0x90, 0xCB, 0x91, 0xCB, 0x92, 0xCB, 0x93, 0xCB, 0x94, 0xCB, 0x95, 0xCB, 0x96, 0xCB, 0x97, 0xCB, 0x98, 0xCB, 0x99, 0xCB, 0x9A, 0xCB, 0x9B, 0xCB, 0x9C, 0xCB, 0x9D, 0xCB, 0x9E, 0xCB, 0x9F,
// 0xCB, 0xA0, 0xCB, 0xA1, 0xCB, 0xA2, 0xCB, 0xA3, 0xCB, 0xA4, 0xCB, 0xA5, 0xCB, 0xA6, 0xCB, 0xA7, 0xCB, 0xA8, 0xCB, 0xA9, 0xCB, 0xAA, 0xCB, 0xAB, 0xCB, 0xAC, 0xCB, 0xAD, 0xCB, 0xAE, 0xCB, 0xAF,
// 0xCB, 0xB0, 0xCB, 0xB1, 0xCB, 0xB2, 0xCB, 0xB3, 0xCB, 0xB4, 0xCB, 0xB5, 0xCB, 0xB6, 0xCB, 0xB7, 0xCB, 0xB8, 0xCB, 0xB9, 0xCB, 0xBA, 0xCB, 0xBB, 0xCB, 0xBC, 0xCB, 0xBD, 0xCB, 0xBE, 0xCB, 0xBF,
// 0xCB, 0xC0, 0xCB, 0xC1, 0xCB, 0xC2, 0xCB, 0xC3, 0xCB, 0xC4, 0xCB, 0xC5, 0xCB, 0xC6, 0xCB, 0xC7, 0xCB, 0xC8, 0xCB, 0xC9, 0xCB, 0xCA, 0xCB, 0xCB, 0xCB, 0xCC, 0xCB, 0xCD, 0xCB, 0xCE, 0xCB, 0xCF,
// 0xCB, 0xD0, 0xCB, 0xD1, 0xCB, 0xD2, 0xCB, 0xD3, 0xCB, 0xD4, 0xCB, 0xD5, 0xCB, 0xD6, 0xCB, 0xD7, 0xCB, 0xD8, 0xCB, 0xD9, 0xCB, 0xDA, 0xCB, 0xDB, 0xCB, 0xDC, 0xCB, 0xDD, 0xCB, 0xDE, 0xCB, 0xDF,
// 0xCB, 0xE0, 0xCB, 0xE1, 0xCB, 0xE2, 0xCB, 0xE3, 0xCB, 0xE4, 0xCB, 0xE5, 0xCB, 0xE6, 0xCB, 0xE7, 0xCB, 0xE8, 0xCB, 0xE9, 0xCB, 0xEA, 0xCB, 0xEB, 0xCB, 0xEC, 0xCB, 0xED, 0xCB, 0xEE, 0xCB, 0xEF,
// 0xCB, 0xF0, 0xCB, 0xF1, 0xCB, 0xF2, 0xCB, 0xF3, 0xCB, 0xF4, 0xCB, 0xF5, 0xCB, 0xF6, 0xCB, 0xF7, 0xCB, 0xF8, 0xCB, 0xF9, 0xCB, 0xFA, 0xCB, 0xFB, 0xCB, 0xFC, 0xCB, 0xFD, 0xCB, 0xFE, 0xCB, 0xFF
// 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
// 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
// 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
// 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
// 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,
// 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F,
// 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,
// 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F,
// 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F,
// 0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F,
// 0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF,
// 0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF,
// 0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF,
// 0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF,
// 0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF,
// 0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF
};

// fetches a single word from instruction memory
// returns a pointer to word and increments PC
uint8_t* fetch_inst() {
    return &memory[rf.PC++];
}

// Writes flags to register F
//    ┌-> Carry
//  ┌-+> Subtraction
//  | | ┌┌┌┌----always zeroes
// 1111 0000
// | |
// └-+> Zero
//   └-> Half Carry
void write_flags() {
    uint8_t flags = (f_zero ? 1 : 0)    << ZERO_FLAG_OFFSET |
                    (f_sub ? 1 : 0)     << SUB_FLAG_OFFSET |
                    (f_hcarry ? 1 : 0)  << HALF_CARRY_FLAG_OFFSET |
                    (f_carry ? 1 : 0)   << CARRY_FLAG_OFFSET;
    rf.AF.r = flags;
}

void handle_interrupt(uint8_t interrupt_bit, uint16_t address) {
    // Clear IME so no nested interrupts
    rf.IME = false;

    // Clear interrupt flag bit (by writing 0 to it)
    memory[IF_ADDR] &= ~(1 << interrupt_bit);

    // Push current PC to stack 
    memory[--rf.SP] = (uint8_t)(nn&0x00FF); // lsbyte
    memory[--rf.SP] = (uint8_t)(nn&0xFF00); // msbyte
    
    // Jump to ISR
    rf.PC = address;
}

uint8_t* opcode = NULL;
uint8_t* cb_opcode = NULL;
bool jump_cond = false;
bool to_enable_ime = false;
bool cpu_stopped = false;
bool cpu_halted = false;
int cpu_cycles_waited = 0; // for emulating semi-accurate instruction timing
void clock_cpu() {
    // first, handle interrupts
    uint8_t IE = memory[IE_ADDR];
    uint8_t IF = memory[IF_ADDR];
    if (rf.IME && (IE & IF & 0x1F)) {
        // handle interrupts in order of priority
        if (IE & IF & 0x01) handle_interrupt(0, 0x0040); // V-Blank
        else if (IE & IF & 0x02) handle_interrupt(1, 0x0048); // LCD STAT
        else if (IE & IF & 0x04) handle_interrupt(2, 0x0050); // Timer
        else if (IE & IF & 0x08) handle_interrupt(3, 0x0058); // Serial
        else if (IE & IF & 0x10) handle_interrupt(4, 0x0060); // Joypad
        cpu_stopped = false; cpu_halted = false; // resume if stopped or halted
        return; // Skip instruction execution this cycle
    }

    if (cpu_stopped) {
        // Allow STOP to resume on Joypad input (IF bit 4 set)
        if (IF & 0x10) {
            cpu_stopped = false; // wake up
        } else {
            return; // do nothing else this cycle
        }
    }

    if (cpu_halted) {
        if (!rf.IME && (IE & IF & 0x1F)) {
            // HALT bug
            cpu_halted = false;
            rf.PC++; // skip one byte
        } else {
            return; // stay halted
        }
    }

    // execute the previously fetched instruction
    if (!cpu_stopped && opcode != NULL) {
        // instruction has been fetched
        if(NOP(*opcode)) {
            TraceLog(LOG_INFO, "NOP", *opcode);
            opcode = NULL;
        } else if(HALT(*opcode)) {
            TraceLog(LOG_INFO, "HALT", *opcode);
            cpu_halted = true;
            opcode = NULL;
        } else if(LD_R_HLA(*opcode)) {
            if (cpu_cycles_waited == 0) {
                uint8_t target = (*opcode&0x38)>>3;
                uint8_t val = memory[rf.HL.lr];
                TraceLog(LOG_INFO, "Load Register %d from HL indirect (addr %d, val %d)", target, rf.HL.lr, val);

                switch (target) {
                    case 0: rf.BC.l = val; break; // B
                    case 1: rf.BC.r = val; break; // C
                    case 2: rf.DE.l = val; break; // D
                    case 3: rf.DE.r = val; break; // E
                    case 4: rf.HL.l = val; break; // H
                    case 5: rf.HL.r = val; break; // L
                    case 7: rf.AF.l = val; break;  // A 
                }

                // flags remain unmodified
            }
            if (++cpu_cycles_waited >= LD_R_HLA_CYCLES) {
                opcode = NULL;
                cpu_cycles_waited = 0;
            }
        } else if(LD_HLA_R(*opcode)) {
            if (cpu_cycles_waited == 0) {
                uint8_t source = (*opcode&0x07);

                uint8_t val;
                switch (source) {
                    case 0: val = rf.BC.l; break; // B
                    case 1: val = rf.BC.r; break; // C
                    case 2: val = rf.DE.l; break; // D
                    case 3: val = rf.DE.r; break; // E
                    case 4: val = rf.HL.l; break; // H
                    case 5: val = rf.HL.r; break; // L
                    case 7: val = rf.AF.l; break;  // A 
                }
                TraceLog(LOG_INFO, "Load HL indirect (addr %d) from Register %d (val %d)", rf.HL.lr, source, val);

                memory[rf.HL.lr] = val;

                // flags remain unmodified
            }
            if (++cpu_cycles_waited >= LD_HLA_R_CYCLES) {
                opcode = NULL;
                cpu_cycles_waited = 0;
            }
        } else if(LD_HLA_I(*opcode)) {
            TraceLog(LOG_INFO, "LD_HLA_I");
            if (cpu_cycles_waited == 0) {
                uint8_t n = *fetch_inst();
                TraceLog(LOG_INFO, "Load HL indirect (addr %d) Immediate Value %d", rf.HL.lr, n);

                memory[rf.HL.lr] = n;

                // flags remain unmodified
            }
            if (++cpu_cycles_waited >= LD_HLA_I_CYCLES) {
                opcode = NULL;
                cpu_cycles_waited = 0;
            }
        } else if (LD_R_R(*opcode)) {
            uint8_t target_reg = (*opcode&0x38)>>3;
            uint8_t source_reg = (*opcode&0x07);
            TraceLog(LOG_INFO, "Load Register %d from Register %d", target_reg, source_reg);

            uint8_t num;
            switch (source_reg) {
                case 0: num = rf.BC.l; break; // B
                case 1: num = rf.BC.r; break; // C
                case 2: num = rf.DE.l; break; // D
                case 3: num = rf.DE.r; break; // E
                case 4: num = rf.HL.l; break; // H
                case 5: num = rf.HL.r; break; // L
                case 7: num = rf.AF.l; break;  // A 
            }

            switch (target_reg) {
                case 0: rf.BC.l = num; break; // B
                case 1: rf.BC.r = num; break; // C
                case 2: rf.DE.l = num; break; // D
                case 3: rf.DE.r = num; break; // E
                case 4: rf.HL.l = num; break; // H
                case 5: rf.HL.r = num; break; // L
                case 7: rf.AF.l = num; break;  // A 
            }

            // flags remain unmodified

            opcode = NULL;
        } else if(LD_R_I(*opcode)) {
            if (cpu_cycles_waited == 0) {
                uint8_t target = (*opcode&0x38)>>3;
                uint8_t n = *fetch_inst();
                TraceLog(LOG_INFO, "Load Register %d from Immediate Value %d", target, n);

                switch (target) {
                    case 0: rf.BC.l = n; break; // B
                    case 1: rf.BC.r = n; break; // C
                    case 2: rf.DE.l = n; break; // D
                    case 3: rf.DE.r = n; break; // E
                    case 4: rf.HL.l = n; break; // H
                    case 5: rf.HL.r = n; break; // L
                    case 7: rf.AF.l = n; break;  // A 
                }

                // flags remain unmodified
            }
            if (++cpu_cycles_waited >= LD_R_I_CYCLES) {
                opcode = NULL;
                cpu_cycles_waited = 0;
            }
        } else if(LD_A_BCA(*opcode)) {
            if (cpu_cycles_waited == 0) {
                uint8_t val = memory[rf.BC.lr];
                TraceLog(LOG_INFO, "Load Register A from BC indirect (addr %d, val %d)", rf.BC.lr, val);

                rf.AF.l = val;

                // flags remain unmodified
            }
            if (++cpu_cycles_waited >= LD_A_BCA_CYCLES) {
                opcode = NULL;
                cpu_cycles_waited = 0;
            }
        } else if(LD_A_DEA(*opcode)) {
            if (cpu_cycles_waited == 0) {
                uint8_t val = memory[rf.DE.lr];
                TraceLog(LOG_INFO, "Load Register A from DE indirect (addr %d, val %d)", rf.DE.lr, val);

                rf.AF.l = val;

                // flags remain unmodified
            }
            if (++cpu_cycles_waited >= LD_A_DEA_CYCLES) {
                opcode = NULL;
                cpu_cycles_waited = 0;
            }
        } else if(LD_BCA_A(*opcode)) {
            if (cpu_cycles_waited == 0) {
                TraceLog(LOG_INFO, "Load BC indirect (addr %d) from Register A", rf.BC.lr);
                memory[rf.BC.lr] = rf.AF.l;

                // flags remain unmodified
            }
            if (++cpu_cycles_waited >= LD_BCA_A_CYCLES) {
                opcode = NULL;
                cpu_cycles_waited = 0;
            }
        } else if(LD_DEA_A(*opcode)) {
            if (cpu_cycles_waited == 0) {
                TraceLog(LOG_INFO, "Load DE indirect (addr %d) from Register A", rf.DE.lr);
                memory[rf.DE.lr] = rf.AF.l;

                // flags remain unmodified
            }
            if (++cpu_cycles_waited >= LD_DEA_A_CYCLES) {
                opcode = NULL;
                cpu_cycles_waited = 0;
            }
        } else if(LD_A_IIA(*opcode)) {
            if (cpu_cycles_waited == 0) {
                uint16_t nn = (uint16_t)*fetch_inst() | ((uint16_t)*fetch_inst() << 8);
                uint8_t val = memory[nn];
                TraceLog(LOG_INFO, "Load Register A from Immediate indirect (addr %d, val %d)", nn, val);

                rf.AF.l = val;

                // flags remain unmodified
            }
            if (++cpu_cycles_waited >= LD_A_IIA_CYCLES) {
                opcode = NULL;
                cpu_cycles_waited = 0;
            }
        } else if(LD_IIA_A(*opcode)) {
            if (cpu_cycles_waited == 0) {
                uint16_t nn = (uint16_t)*fetch_inst() | ((uint16_t)*fetch_inst() << 8);
                TraceLog(LOG_INFO, "Load Immediate indirect (addr %d) from Register A", nn);

                memory[nn] = rf.AF.l;

                // flags remain unmodified
            }
            if (++cpu_cycles_waited >= LD_IIA_A_CYCLES) {
                opcode = NULL;
                cpu_cycles_waited = 0;
            }
        } else if(LD_A_CA(*opcode)) {
            if (cpu_cycles_waited == 0) {
                uint16_t addr = 0xFF00 | (uint16_t)rf.BC.r;
                uint8_t val = memory[addr];
                TraceLog(LOG_INFO, "Load Register A from 0xFF00 + C indirect (addr %d, val %d)", addr, val);

                rf.AF.l = val;

                // flags remain unmodified
            }
            if (++cpu_cycles_waited >= LD_A_CA_CYCLES) {
                opcode = NULL;
                cpu_cycles_waited = 0;
            }
        } else if(LD_CA_A(*opcode)) {
            if (cpu_cycles_waited == 0) {
                uint16_t addr = 0xFF00 | (uint16_t)rf.BC.r;
                TraceLog(LOG_INFO, "Load 0xFF00 + C indirect (addr %d) from Register A", addr);

                memory[addr] = rf.AF.l;

                // flags remain unmodified
            }
            if (++cpu_cycles_waited >= LD_CA_A_CYCLES) {
                opcode = NULL;
                cpu_cycles_waited = 0;
            }
        } else if(LD_A_IA(*opcode)) {
            if (cpu_cycles_waited == 0) {
                uint8_t n = *fetch_inst();
                uint16_t addr = 0xFF00 | (uint16_t)n;
                uint8_t val = memory[addr];
                TraceLog(LOG_INFO, "Load Register A from 0xFF00 + Immediate indirect (addr %d, val %d)", addr, val);

                rf.AF.l = val;

                // flags remain unmodified
            }
            if (++cpu_cycles_waited >= LD_A_IA_CYCLES) {
                opcode = NULL;
                cpu_cycles_waited = 0;
            }
        } else if(LD_IA_A(*opcode)) {
            if (cpu_cycles_waited == 0) {
                uint8_t n = *fetch_inst();
                uint16_t addr = 0xFF00 | (uint16_t)n;
                TraceLog(LOG_INFO, "Load 0xFF00 + Immediate indirect (addr %d) from Register A ", addr);

                memory[addr] = rf.AF.l;

                // flags remain unmodified
            }
            if (++cpu_cycles_waited >= LD_IA_A_CYCLES) {
                opcode = NULL;
                cpu_cycles_waited = 0;
            }
        } else if(LD_A_HLA_DEC(*opcode)) {
            if (cpu_cycles_waited == 0) {
                uint16_t addr = rf.HL.lr--;
                uint8_t val = memory[addr];
                TraceLog(LOG_INFO, "Load Register A from HL indirect (decrement) (addr %d, val %d)", addr, val);

                rf.AF.l = val;

                // flags remain unmodified
            }
            if (++cpu_cycles_waited >= LD_A_HLA_DEC_CYCLES) {
                opcode = NULL;
                cpu_cycles_waited = 0;
            }
        } else if(LD_HLA_A_DEC(*opcode)) {
            if (cpu_cycles_waited == 0) {
                uint16_t addr = rf.HL.lr--;
                TraceLog(LOG_INFO, "Load HL indirect (decrement) (addr %d) from Register A", addr);

                memory[addr] = rf.AF.l;

                // flags remain unmodified
            }
            if (++cpu_cycles_waited >= LD_HLA_A_DEC_CYCLES) {
                opcode = NULL;
                cpu_cycles_waited = 0;
            }
        } else if(LD_A_HLA_INC(*opcode)) {
            if (cpu_cycles_waited == 0) {
                uint16_t addr = rf.HL.lr++;
                uint8_t val = memory[addr];
                TraceLog(LOG_INFO, "Load Register A from HL indirect (increment) (addr %d, val %d)", addr, val);

                rf.AF.l = val;

                // flags remain unmodified
            }
            if (++cpu_cycles_waited >= LD_A_HLA_INC_CYCLES) {
                opcode = NULL;
                cpu_cycles_waited = 0;
            }
        } else if(LD_HLA_A_INC(*opcode)) {
            if (cpu_cycles_waited == 0) {
                uint16_t addr = rf.HL.lr++;
                TraceLog(LOG_INFO, "Load HL indirect (increment) (addr %d) from Register A", addr);

                memory[addr] = rf.AF.l;

                // flags remain unmodified
            }
            if (++cpu_cycles_waited >= LD_HLA_A_INC_CYCLES) {
                opcode = NULL;
                cpu_cycles_waited = 0;
            }
        } else if(LD_RP_II(*opcode)) {
            if (cpu_cycles_waited == 0) {
                uint8_t target = (*opcode&0x30)>>4;
                uint16_t nn = (uint16_t)*fetch_inst() | ((uint16_t)*fetch_inst() << 8);
                TraceLog(LOG_INFO, "Load Register Pair %d from Immediate Value %d", target, nn);

                switch (target) {
                    case 0: rf.BC.lr = nn; break; // BC
                    case 1: rf.DE.lr = nn; break; // DE
                    case 2: rf.HL.lr = nn; break; // HL
                    case 3: rf.SP = nn; break; // SP
                }

                // flags remain unmodified
            }
            if (++cpu_cycles_waited >= LD_RP_II_CYCLES) {
                opcode = NULL;
                cpu_cycles_waited = 0;
            }
        } else if(LD_IIA_SP(*opcode)) {
            if (cpu_cycles_waited == 0) {
                uint16_t nn = (uint16_t)*fetch_inst() | ((uint16_t)*fetch_inst() << 8);
                TraceLog(LOG_INFO, "Load Immediate Value indirect (addr %d) from SP", nn);
                
                memory[nn] = (uint8_t)(rf.SP&0x00FF); // lsbyte
                memory[nn+1] = (uint8_t)(rf.SP&0xFF00); // msbyte

                // flags remain unmodified
            }
            if (++cpu_cycles_waited >= LD_IIA_SP_CYCLES) {
                opcode = NULL;
                cpu_cycles_waited = 0;
            }
        } else if(LD_SP_HL(*opcode)) {
            if (cpu_cycles_waited == 0) {
                TraceLog(LOG_INFO, "Load SP from HL");
                
                rf.SP = rf.HL.lr;

                // flags remain unmodified
            }
            if (++cpu_cycles_waited >= LD_SP_HL_CYCLES) {
                opcode = NULL;
                cpu_cycles_waited = 0;
            }
        } else if(PUSH(*opcode)) {
            if (cpu_cycles_waited == 0) {
                uint8_t target = (*opcode&0x30)>>4;
                TraceLog(LOG_INFO, "Push to Stack from Register Pair %d", target);
                
                uint16_t val;
                switch (target) {
                    case 0: val = rf.BC.lr; break; // BC
                    case 1: val = rf.DE.lr; break; // DE
                    case 2: val = rf.HL.lr; break; // HL
                    case 3: val = rf.SP; break; // SP
                }

                memory[--rf.SP] = (uint8_t)(val&0x00FF); // lsbyte
                memory[--rf.SP] = (uint8_t)(val&0xFF00); // msbyte

                // flags remain unmodified
            }
            if (++cpu_cycles_waited >= PUSH_CYCLES) {
                opcode = NULL;
                cpu_cycles_waited = 0;
            }
        } else if(POP(*opcode)) {
            if (cpu_cycles_waited == 0) {
                uint8_t target = (*opcode&0x30)>>4;
                TraceLog(LOG_INFO, "Pop from Stack to Register Pair %d", target);

                uint8_t lsb = memory[rf.SP++]; // lsbyte
                uint8_t msb = memory[rf.SP++]; // msbyte
                uint16_t val = ((uint16_t)msb << 8) | lsb;

                switch (target) {
                    case 0: rf.BC.lr = val; break; // BC
                    case 1: rf.DE.lr = val; break; // DE
                    case 2: rf.HL.lr = val; break; // HL
                    case 3: rf.SP = val; break; // SP
                }

                // flags remain unmodified
            }
            if (++cpu_cycles_waited >= POP_CYCLES) {
                opcode = NULL;
                cpu_cycles_waited = 0;
            }
        } else if(LD_HL_SPE(*opcode)) {
            if (cpu_cycles_waited == 0) {
                int8_t e = *fetch_inst();
                int16_t val = e + rf.SP;
                TraceLog(LOG_INFO, "Load HL from SP + Immediate Value %d", e);

                rf.HL.lr = val;

                // compute flags
                uint8_t low_sp = rf.SP & 0xFF;
                uint8_t low_offset = (uint8_t)e;

                f_zero = false;
                f_sub = false;
                f_hcarry = ((low_sp & 0xF) + (low_offset & 0xF)) > 0xF;
                f_carry = ((low_sp & 0xFF) + (low_offset & 0xFF)) > 0xFF;
            }
            if (++cpu_cycles_waited >= LD_HL_SPE_CYCLES) {
                opcode = NULL;
                cpu_cycles_waited = 0;
            }
        } else if(ADD_HL(*opcode)) {
            TraceLog(LOG_INFO, "Add Register A with Register HL indirect", *opcode);
            if (cpu_cycles_waited == 0) {
                // get operands
                uint8_t num1, num2;
                num1 = rf.AF.l;
                num2 = memory[rf.HL.lr];

                // compute addition and set register A to result
                uint16_t result = num1 + num2;
                rf.AF.l = result & 0xFF;

                // compute flags
                f_zero = (result == 0);
                f_sub = false;
                f_hcarry = ((num1 & 0xF) + (num2 & 0xF)) > 0xF;
                f_carry = result > 0xFF;

                TraceLog(LOG_INFO, "%d + %d = %d", num1, num2, result & 0xFF);
            }
            if (++cpu_cycles_waited >= ADD_HL_CYCLES) {
                opcode = NULL;
                cpu_cycles_waited = 0;
            }
        } else if(ADD(*opcode)) {
            uint8_t target = *opcode & (0x07);

            // get operands
            uint8_t num1, num2;
            num1 = rf.AF.l;
            switch (target) {
                case 0: num2 = rf.BC.l; break; // B
                case 1: num2 = rf.BC.r; break; // C
                case 2: num2 = rf.DE.l; break; // D
                case 3: num2 = rf.DE.r; break; // E
                case 4: num2 = rf.HL.l; break; // H
                case 5: num2 = rf.HL.r; break; // L
                case 7: num2 = rf.AF.l; break;  // A 
            }

            char target_code;
            switch (target) {
                case 0: target_code = 'B'; break; // B
                case 1: target_code = 'C'; break; // C
                case 2: target_code = 'D'; break; // D
                case 3: target_code = 'E'; break; // E
                case 4: target_code = 'H'; break; // H
                case 5: target_code = 'L'; break; // L
                case 7: target_code = 'A'; break;  // A 
            }
            TraceLog(LOG_INFO, "Add Register A with Target Register %c", target_code);

            // compute addition and set register A to result
            uint16_t result = num1 + num2;
            rf.AF.l = result & 0xFF;

            // compute flags
            f_zero = (result == 0);
            f_sub = false;
            f_hcarry = ((num1 & 0xF) + (num2 & 0xF)) > 0xF;
            f_carry = result > 0xFF;

            TraceLog(LOG_INFO, "%d + %d = %d", num1, num2, result & 0xFF);

            opcode = NULL;
        } else if(ADDI(*opcode)) {
            TraceLog(LOG_INFO, "Add Register A with Immediate Value n");
            if (cpu_cycles_waited == 0) {
                uint8_t n = *fetch_inst();
                uint16_t result = rf.AF.l + n;
                TraceLog(LOG_INFO, "%d + %d = %d", rf.AF.l, n, result & 0xFF);
                rf.AF.l = result & 0xFF;

                // compute flags
                f_zero = (result == 0);
                f_sub = false;
                f_hcarry = ((rf.AF.l & 0xF) + (n & 0xF)) > 0xF;
                f_carry = result > 0xFF;

            }
            if (++cpu_cycles_waited >= ADDI_CYCLES) {
                opcode = NULL;
                cpu_cycles_waited = 0;
            }
        } else if(ADC_HL(*opcode)) {
            if (cpu_cycles_waited == 0) {
                TraceLog(LOG_INFO, "Add Register A with Register HL indirect and Carry Flag %d", carry);
                // get operands
                uint8_t num1, num2;
                num1 = rf.AF.l;
                num2 = memory[rf.HL.lr];

                uint8_t carry = f_carry ? 1 : 0;

                // compute addition and set register A to result
                uint16_t result = num1 + num2 + carry;
                rf.AF.l = result & 0xFF;

                // compute flags
                f_zero = (result == 0);
                f_sub = false;
                f_hcarry = ((num1 & 0xF) + (num2 & 0xF) + carry) > 0xF;
                f_carry = result > 0xFF;

                TraceLog(LOG_INFO, "%d + %d + %d = %d", num1, num2, carry, result & 0xFF);
            }
            if (++cpu_cycles_waited >= ADDC_HL_CYCLES) {
                opcode = NULL;
                cpu_cycles_waited = 0;
            }
        } else if(ADC(*opcode)) {
            uint8_t target = *opcode & (0x07);

            // get operands
            uint8_t num1, num2;
            num1 = rf.AF.l;
            switch (target) {
                case 0: num2 = rf.BC.l; break; // B
                case 1: num2 = rf.BC.r; break; // C
                case 2: num2 = rf.DE.l; break; // D
                case 3: num2 = rf.DE.r; break; // E
                case 4: num2 = rf.HL.l; break; // H
                case 5: num2 = rf.HL.r; break; // L
                case 7: num2 = rf.AF.l; break;  // A 
            }

            uint8_t carry = f_carry ? 1 : 0;

            char target_code;
            switch (target) {
                case 0: target_code = 'B'; break; // B
                case 1: target_code = 'C'; break; // C
                case 2: target_code = 'D'; break; // D
                case 3: target_code = 'E'; break; // E
                case 4: target_code = 'H'; break; // H
                case 5: target_code = 'L'; break; // L
                case 7: target_code = 'A'; break;  // A 
            }
            TraceLog(LOG_INFO, "Add Register A with Target Register %c and Carry Flag %d", target_code, carry);


            // compute addition and set register A to result
            uint16_t result = num1 + num2 + carry;
            rf.AF.l = result & 0xFF;

            // compute flags
            f_zero = (result == 0);
            f_sub = false;
            f_hcarry = ((num1 & 0xF) + (num2 & 0xF) + carry) > 0xF;
            f_carry = result > 0xFF;

            TraceLog(LOG_INFO, "%d + %d + %d = %d", num1, num2, carry, result & 0xFF);

            opcode = NULL;
        } else if(ADCI(*opcode)) {
            uint8_t carry = f_carry ? 1 : 0;
            TraceLog(LOG_INFO, "Add Register A with Immediate Value n and Carry Flag");
            if (cpu_cycles_waited == 0) {
                uint8_t n = *fetch_inst();
                uint16_t result = rf.AF.l + n + carry;
                TraceLog(LOG_INFO, "%d + %d + %d = %d", rf.AF.l, n, carry, result & 0xFF);
                rf.AF.l = result & 0xFF;

                // compute flags
                f_zero = (result == 0);
                f_sub = false;
                f_hcarry = ((rf.AF.l & 0xF) + (n & 0xF) + carry) > 0xF;
                f_carry = result > 0xFF;

            }
            if (++cpu_cycles_waited >= ADDCI_CYCLES) {
                opcode = NULL;
                cpu_cycles_waited = 0;
            }
        } else if(SUB_HL(*opcode)) {
            TraceLog(LOG_INFO, "Subtract Register HL indirect from Register A", *opcode);
            if (cpu_cycles_waited == 0) {
                uint8_t target = *opcode & (0x07);

                // get operands
                uint8_t num1, num2;
                num1 = rf.AF.l;
                num2 = memory[rf.HL.lr];

                // compute subtraction and set register A to result
                uint8_t result = num1 - num2;
                rf.AF.l = result;

                // compute flags
                f_zero = (result == 0);
                f_sub = true;
                f_hcarry = (num1 & 0xF) < (num2 & 0xF);
                f_carry = num1 < num2;

                TraceLog(LOG_INFO, "%d - %d = %d", num1, num2, result);
            }
            if (++cpu_cycles_waited >= SUB_HL_CYCLES) {
                opcode = NULL;
                cpu_cycles_waited = 0;
            }
        } else if(SUB(*opcode)) {
            uint8_t target = *opcode & (0x07);

            // get operands
            uint8_t num1, num2;
            num1 = rf.AF.l;
            switch (target) {
                case 0: num2 = rf.BC.l; break; // B
                case 1: num2 = rf.BC.r; break; // C
                case 2: num2 = rf.DE.l; break; // D
                case 3: num2 = rf.DE.r; break; // E
                case 4: num2 = rf.HL.l; break; // H
                case 5: num2 = rf.HL.r; break; // L
                case 7: num2 = rf.AF.l; break;  // A 
            }

            char target_code;
            switch (target) {
                case 0: target_code = 'B'; break; // B
                case 1: target_code = 'C'; break; // C
                case 2: target_code = 'D'; break; // D
                case 3: target_code = 'E'; break; // E
                case 4: target_code = 'H'; break; // H
                case 5: target_code = 'L'; break; // L
                case 7: target_code = 'A'; break;  // A 
            }
            TraceLog(LOG_INFO, "Subtract Target Register %c from Register A", target_code);

            // compute subtraction and set register A to result
            uint8_t result = num1 - num2;
            rf.AF.l = result;

            // compute flags
            f_zero = (result == 0);
            f_sub = true;
            f_hcarry = (num1 & 0xF) < (num2 & 0xF);
            f_carry = num1 < num2;

            TraceLog(LOG_INFO, "%d - %d = %d", num1, num2, result);

            opcode = NULL;
        } else if(SUBI(*opcode)) {
            TraceLog(LOG_INFO, "Subtract Immediate Value n from Register A");
            if (cpu_cycles_waited == 0) {
                uint8_t n = *fetch_inst();
                uint8_t result = rf.AF.l - n;
                TraceLog(LOG_INFO, "%d - %d = %d", rf.AF.l, n, result);
                rf.AF.l = result;

                // compute flags
                f_zero = (result == 0);
                f_sub = true;
                f_hcarry = (rf.AF.l & 0xF) < (n & 0xF);
                f_carry = rf.AF.l < n;

            }
            if (++cpu_cycles_waited >= SUBI_CYCLES) {
                opcode = NULL;
                cpu_cycles_waited = 0;
            }
        } else if(SBC_HL(*opcode)) {
            TraceLog(LOG_INFO, "Subtract Register HL and Carry Flag from Register A", *opcode);
            if (cpu_cycles_waited == 0) {

                // get operands
                uint8_t num1, num2;
                num1 = rf.AF.l;
                num2 = memory[rf.HL.lr];

                uint8_t carry = f_carry ? 1 : 0;

                // compute subtraction and set register A to result
                uint16_t result = num1 - num2 - carry;
                rf.AF.l = result;

                // compute flags
                f_zero = (result == 0);
                f_sub = true;
                f_hcarry = (num1 & 0xF) < ((num2 & 0xF) + carry);
                f_carry = num1 < (num2 + carry);

                TraceLog(LOG_INFO, "%d - %d - %d = %d", num1, num2, carry, result);
            }
            if (++cpu_cycles_waited >= SBC_HL_CYCLES) {
                opcode = NULL;
                cpu_cycles_waited = 0;
            }
        } else if(SBC(*opcode)) {
            uint8_t target = *opcode & (0x07);

            // get operands
            uint8_t num1, num2;
            num1 = rf.AF.l;
            switch (target) {
                case 0: num2 = rf.BC.l; break; // B
                case 1: num2 = rf.BC.r; break; // C
                case 2: num2 = rf.DE.l; break; // D
                case 3: num2 = rf.DE.r; break; // E
                case 4: num2 = rf.HL.l; break; // H
                case 5: num2 = rf.HL.r; break; // L
                case 7: num2 = rf.AF.l; break;  // A 
            }

            uint8_t carry = f_carry ? 1 : 0;

            char target_code;
            switch (target) {
                case 0: target_code = 'B'; break; // B
                case 1: target_code = 'C'; break; // C
                case 2: target_code = 'D'; break; // D
                case 3: target_code = 'E'; break; // E
                case 4: target_code = 'H'; break; // H
                case 5: target_code = 'L'; break; // L
                case 7: target_code = 'A'; break;  // A 
            }
            TraceLog(LOG_INFO, "Subtract Target Register %c and Carry Flag %d from Register A", target_code, carry);

            // compute subtraction and set register A to result
            uint16_t result = num1 - num2 - carry;
            rf.AF.l = result;

            // compute flags
            f_zero = (result == 0);
            f_sub = true;
            f_hcarry = (num1 & 0xF) < ((num2 & 0xF) + carry);
            f_carry = num1 < (num2 + carry);

            TraceLog(LOG_INFO, "%d - %d - %d = %d", num1, num2, carry, result);

            opcode = NULL;
        } else if(SBCI(*opcode)) {
            uint8_t carry = f_carry ? 1 : 0;
            TraceLog(LOG_INFO, "Subtract Immediate Value n and Carry Flag from Register A");
            if (cpu_cycles_waited == 0) {
                uint8_t n = *fetch_inst();
                uint8_t result = rf.AF.l - n - carry;
                TraceLog(LOG_INFO, "%d - %d - %d = %d", rf.AF.l, n, carry, result);
                rf.AF.l = result;

                // compute flags
                f_zero = (result == 0);
                f_sub = true;
                f_hcarry = (rf.AF.l & 0xF) < ((n & 0xF) + carry);
                f_carry = rf.AF.l < (n + carry);

            }
            if (++cpu_cycles_waited >= ADDCI_CYCLES) {
                opcode = NULL;
                cpu_cycles_waited = 0;
            }
        } else if(CP_HL(*opcode)) {
            TraceLog(LOG_INFO, "Compare Register A to Register HL indirect", *opcode);
            if (cpu_cycles_waited == 0) {
                // get operands
                uint8_t num1, num2;
                num1 = rf.AF.l;
                num2 = memory[rf.HL.lr];

                // compute subtraction
                uint8_t result = num1 - num2;

                // compute flags
                f_zero = (result == 0);
                f_sub = true;
                f_hcarry = (num1 & 0xF) < (num2 & 0xF);
                f_carry = num1 < num2;

                TraceLog(LOG_INFO, "%d - %d = %d", num1, num2, result);
            }
            if (++cpu_cycles_waited >= CP_HL_CYCLES) {
                opcode = NULL;
                cpu_cycles_waited = 0;
            }
        } else if(CP(*opcode)) {
            uint8_t target = *opcode & (0x07);

            // get operands
            uint8_t num1, num2;
            num1 = rf.AF.l;
            switch (target) {
                case 0: num2 = rf.BC.l; break; // B
                case 1: num2 = rf.BC.r; break; // C
                case 2: num2 = rf.DE.l; break; // D
                case 3: num2 = rf.DE.r; break; // E
                case 4: num2 = rf.HL.l; break; // H
                case 5: num2 = rf.HL.r; break; // L
                case 7: num2 = rf.AF.l; break;  // A 
            }

            char target_code;
            switch (target) {
                case 0: target_code = 'B'; break; // B
                case 1: target_code = 'C'; break; // C
                case 2: target_code = 'D'; break; // D
                case 3: target_code = 'E'; break; // E
                case 4: target_code = 'H'; break; // H
                case 5: target_code = 'L'; break; // L
                case 7: target_code = 'A'; break;  // A 
            }
            TraceLog(LOG_INFO, "Compare Register A to Target Register %c", target_code);

            // compute subtraction
            uint8_t result = num1 - num2;

            // compute flags
            f_zero = (result == 0);
            f_sub = true;
            f_hcarry = (num1 & 0xF) < (num2 & 0xF);
            f_carry = num1 < num2;

            TraceLog(LOG_INFO, "%d - %d = %d", num1, num2, result);

            opcode = NULL;
        } else if(CPI(*opcode)) {
            TraceLog(LOG_INFO, "Compare Register A to Immediate Value n");
            if (cpu_cycles_waited == 0) {
                uint8_t n = *fetch_inst();
                uint8_t result = rf.AF.l - n;
                TraceLog(LOG_INFO, "%d - %d = %d", rf.AF.l, n, result);

                // compute flags
                f_zero = (result == 0);
                f_sub = true;
                f_hcarry = (rf.AF.l & 0xF) < (n & 0xF);
                f_carry = rf.AF.l < n;

            }
            if (++cpu_cycles_waited >= CPI_CYCLES) {
                opcode = NULL;
                cpu_cycles_waited = 0;
            }
        } else if(INC_HL(*opcode)) {
            TraceLog(LOG_INFO, "Increment HL indirect", *opcode);
            if (cpu_cycles_waited == 0) {
                // get operand
                uint8_t num = memory[rf.HL.lr];

                // compute increment and set register A to result
                uint8_t result = num + 1;
                memory[rf.HL.lr] = result;

                // compute flags
                f_zero = (result == 0);
                f_sub = false;
                f_hcarry = ((num & 0xF) + 0x01) > 0xF;

                TraceLog(LOG_INFO, "%d + 1 = %d", num, result);
            }
            if (++cpu_cycles_waited >= INC_HL_CYCLES) {
                opcode = NULL;
                cpu_cycles_waited = 0;
            }
        } else if(INC(*opcode)) {
            uint8_t target = (*opcode & (0x38)) >> 3;

            // get operands
            uint8_t num;
            switch (target) {
                case 0: num = rf.BC.l; break; // B
                case 1: num = rf.BC.r; break; // C
                case 2: num = rf.DE.l; break; // D
                case 3: num = rf.DE.r; break; // E
                case 4: num = rf.HL.l; break; // H
                case 5: num = rf.HL.r; break; // L
                case 7: num = rf.AF.l; break;  // A 
            }

            char target_code;
            switch (target) {
                case 0: target_code = 'B'; break; // B
                case 1: target_code = 'C'; break; // C
                case 2: target_code = 'D'; break; // D
                case 3: target_code = 'E'; break; // E
                case 4: target_code = 'H'; break; // H
                case 5: target_code = 'L'; break; // L
                case 7: target_code = 'A'; break;  // A 
            }
            TraceLog(LOG_INFO, "Increment Target Register %c", target_code);

            // compute increment and set register A to result
            uint8_t result = num + 1;
            switch (target) {
                case 0: rf.BC.l = result; break; // B
                case 1: rf.BC.r = result; break; // C
                case 2: rf.DE.l = result; break; // D
                case 3: rf.DE.r = result; break; // E
                case 4: rf.HL.l = result; break; // H
                case 5: rf.HL.r = result; break; // L
                case 7: rf.AF.l = result; break;  // A 
            }

            // compute flags
            f_zero = (result == 0);
            f_sub = false;
            f_hcarry = ((num & 0xF) + 0x01) > 0xF;

            TraceLog(LOG_INFO, "%d + 1 = %d", num, result);

            opcode = NULL;
        } else if(DEC_HL(*opcode)) {
            TraceLog(LOG_INFO, "Decrement HL indirect", *opcode);
            if (cpu_cycles_waited == 0) {
                // get operand
                uint8_t num = memory[rf.HL.lr];

                // compute increment and set register A to result
                uint8_t result = num - 1;
                memory[rf.HL.lr] = result;

                // compute flags
                f_zero = (result == 0);
                f_sub = false;
                f_hcarry = ((num & 0xF) + 0x01) > 0xF;

                TraceLog(LOG_INFO, "%d - 1 = %d", num, result);
            }
            if (++cpu_cycles_waited >= DEC_HL_CYCLES) {
                opcode = NULL;
                cpu_cycles_waited = 0;
            }
        } else if(DEC(*opcode)) {
            uint8_t target = (*opcode & (0x38)) >> 3;

            // get operands
            uint8_t num;
            switch (target) {
                case 0: num = rf.BC.l; break; // B
                case 1: num = rf.BC.r; break; // C
                case 2: num = rf.DE.l; break; // D
                case 3: num = rf.DE.r; break; // E
                case 4: num = rf.HL.l; break; // H
                case 5: num = rf.HL.r; break; // L
                case 7: num = rf.AF.l; break;  // A 
            }

            char target_code;
            switch (target) {
                case 0: target_code = 'B'; break; // B
                case 1: target_code = 'C'; break; // C
                case 2: target_code = 'D'; break; // D
                case 3: target_code = 'E'; break; // E
                case 4: target_code = 'H'; break; // H
                case 5: target_code = 'L'; break; // L
                case 7: target_code = 'A'; break;  // A 
            }
            TraceLog(LOG_INFO, "Decrement Target Register %c", target_code);

            // compute decrement and set register A to result
            uint8_t result = num - 1;
            switch (target) {
                case 0: rf.BC.l = result; break; // B
                case 1: rf.BC.r = result; break; // C
                case 2: rf.DE.l = result; break; // D
                case 3: rf.DE.r = result; break; // E
                case 4: rf.HL.l = result; break; // H
                case 5: rf.HL.r = result; break; // L
                case 7: rf.AF.l = result; break;  // A 
            }

            // compute flags
            f_zero = (result == 0);
            f_sub = false;
            f_hcarry = num == 0x00; // only overflow on a decrement of 0

            TraceLog(LOG_INFO, "%d - 1 = %d", num, result);

            opcode = NULL;
        } else if(AND_HL(*opcode)) {
            TraceLog(LOG_INFO, "And HL indirect", *opcode);
            if (cpu_cycles_waited == 0) {
                // get operands
                uint8_t num1, num2;
                num1 = rf.AF.l;
                num2 = memory[rf.HL.lr];

                // compute and and set register A to result
                uint8_t result = num1 & num2;
                rf.AF.l = result;

                // compute flags
                f_zero = (result == 0);
                f_sub = false;
                f_hcarry = true;
                f_carry = false;

                TraceLog(LOG_INFO, "%d & %d = %d", num1, num2, result);
            }
            if (++cpu_cycles_waited >= AND_HL_CYCLES) {
                opcode = NULL;
                cpu_cycles_waited = 0;
            }
        } else if(AND(*opcode)) {
            uint8_t target = *opcode & (0x07);

            // get operands
            uint8_t num1, num2;
            num1 = rf.AF.l;
            switch (target) {
                case 0: num2 = rf.BC.l; break; // B
                case 1: num2 = rf.BC.r; break; // C
                case 2: num2 = rf.DE.l; break; // D
                case 3: num2 = rf.DE.r; break; // E
                case 4: num2 = rf.HL.l; break; // H
                case 5: num2 = rf.HL.r; break; // L
                case 7: num2 = rf.AF.l; break;  // A 
            }

            char target_code;
            switch (target) {
                case 0: target_code = 'B'; break; // B
                case 1: target_code = 'C'; break; // C
                case 2: target_code = 'D'; break; // D
                case 3: target_code = 'E'; break; // E
                case 4: target_code = 'H'; break; // H
                case 5: target_code = 'L'; break; // L
                case 7: target_code = 'A'; break;  // A 
            }
            TraceLog(LOG_INFO, "And Register A with Target Register %c", target_code);


            // compute and and set register A to result
            uint8_t result = num1 & num2;
            rf.AF.l = result;

            // compute flags
            f_zero = (result == 0);
            f_sub = false;
            f_hcarry = true;
            f_carry = false;

            TraceLog(LOG_INFO, "%d & %d = %d", num1, num2, result);

            opcode = NULL;
        } else if(ANDI(*opcode)) {
            TraceLog(LOG_INFO, "And Register A with Immediate Value n");
            if (cpu_cycles_waited == 0) {
                uint8_t n = *fetch_inst();
                uint8_t result = rf.AF.l & n;
                TraceLog(LOG_INFO, "%d & %d = %d", rf.AF.l, n, result);
                rf.AF.l = result;

                // compute flags
                f_zero = (result == 0);
                f_sub = false;
                f_hcarry = true;
                f_carry = false;

            }
            if (++cpu_cycles_waited >= ANDI_CYCLES) {
                opcode = NULL;
                cpu_cycles_waited = 0;
            }
        } else if(OR_HL(*opcode)) {
            TraceLog(LOG_INFO, "Or HL indirect", *opcode);
            if (cpu_cycles_waited == 0) {
                // get operands
                uint8_t num1, num2;
                num1 = rf.AF.l;
                num2 = memory[rf.HL.lr];

                // compute and and set register A to result
                uint8_t result = num1 | num2;
                rf.AF.l = result;

                // compute flags
                f_zero = (result == 0);
                f_sub = false;
                f_hcarry = false;
                f_carry = false;

                TraceLog(LOG_INFO, "%d | %d = %d", num1, num2, result);
            }
            if (++cpu_cycles_waited >= OR_HL_CYCLES) {
                opcode = NULL;
                cpu_cycles_waited = 0;
            }
        } else if(OR(*opcode)) {
            uint8_t target = *opcode & (0x07);

            // get operands
            uint8_t num1, num2;
            num1 = rf.AF.l;
            switch (target) {
                case 0: num2 = rf.BC.l; break; // B
                case 1: num2 = rf.BC.r; break; // C
                case 2: num2 = rf.DE.l; break; // D
                case 3: num2 = rf.DE.r; break; // E
                case 4: num2 = rf.HL.l; break; // H
                case 5: num2 = rf.HL.r; break; // L
                case 7: num2 = rf.AF.l; break;  // A 
            }

            char target_code;
            switch (target) {
                case 0: target_code = 'B'; break; // B
                case 1: target_code = 'C'; break; // C
                case 2: target_code = 'D'; break; // D
                case 3: target_code = 'E'; break; // E
                case 4: target_code = 'H'; break; // H
                case 5: target_code = 'L'; break; // L
                case 7: target_code = 'A'; break;  // A 
            }
            TraceLog(LOG_INFO, "Or Register A with Target Register %c", target_code);


            // compute and and set register A to result
            uint8_t result = num1 | num2;
            rf.AF.l = result;

            // compute flags
            f_zero = (result == 0);
            f_sub = false;
            f_hcarry = false;
            f_carry = false;

            TraceLog(LOG_INFO, "%d | %d = %d", num1, num2, result);

            opcode = NULL;
        } else if(ORI(*opcode)) {
            TraceLog(LOG_INFO, "Or Register A with Immediate Value n");
            if (cpu_cycles_waited == 0) {
                uint8_t n = *fetch_inst();
                uint8_t result = rf.AF.l | n;
                TraceLog(LOG_INFO, "%d | %d = %d", rf.AF.l, n, result);
                rf.AF.l = result;

                // compute flags
                f_zero = (result == 0);
                f_sub = false;
                f_hcarry = false;
                f_carry = false;

            }
            if (++cpu_cycles_waited >= ORI_CYCLES) {
                opcode = NULL;
                cpu_cycles_waited = 0;
            }
        } else if(XOR_HL(*opcode)) {
            TraceLog(LOG_INFO, "Xor HL indirect", *opcode);
            if (cpu_cycles_waited == 0) {
                // get operands
                uint8_t num1, num2;
                num1 = rf.AF.l;
                num2 = memory[rf.HL.lr];

                // compute and and set register A to result
                uint8_t result = num1 ^ num2;
                rf.AF.l = result;

                // compute flags
                f_zero = (result == 0);
                f_sub = false;
                f_hcarry = false;
                f_carry = false;

                TraceLog(LOG_INFO, "%d ^ %d = %d", num1, num2, result);
            }
            if (++cpu_cycles_waited >= XOR_HL_CYCLES) {
                opcode = NULL;
                cpu_cycles_waited = 0;
            }
        } else if(XOR(*opcode)) {
            uint8_t target = *opcode & (0x07);

            // get operands
            uint8_t num1, num2;
            num1 = rf.AF.l;
            switch (target) {
                case 0: num2 = rf.BC.l; break; // B
                case 1: num2 = rf.BC.r; break; // C
                case 2: num2 = rf.DE.l; break; // D
                case 3: num2 = rf.DE.r; break; // E
                case 4: num2 = rf.HL.l; break; // H
                case 5: num2 = rf.HL.r; break; // L
                case 7: num2 = rf.AF.l; break;  // A 
            }

            char target_code;
            switch (target) {
                case 0: target_code = 'B'; break; // B
                case 1: target_code = 'C'; break; // C
                case 2: target_code = 'D'; break; // D
                case 3: target_code = 'E'; break; // E
                case 4: target_code = 'H'; break; // H
                case 5: target_code = 'L'; break; // L
                case 7: target_code = 'A'; break;  // A 
            }
            TraceLog(LOG_INFO, "Xor Register A with Target Register %c", target_code);


            // compute and and set register A to result
            uint8_t result = num1 ^ num2;
            rf.AF.l = result;

            // compute flags
            f_zero = (result == 0);
            f_sub = false;
            f_hcarry = false;
            f_carry = false;

            TraceLog(LOG_INFO, "%d ^ %d = %d", num1, num2, result);

            opcode = NULL;
        } else if(XORI(*opcode)) {
            TraceLog(LOG_INFO, "Xor Register A with Immediate Value n");
            if (cpu_cycles_waited == 0) {
                uint8_t n = *fetch_inst();
                uint8_t result = rf.AF.l ^ n;
                TraceLog(LOG_INFO, "%d ^ %d = %d", rf.AF.l, n, result);
                rf.AF.l = result;

                // compute flags
                f_zero = (result == 0);
                f_sub = false;
                f_hcarry = false;
                f_carry = false;
            }
            if (++cpu_cycles_waited >= XORI_CYCLES) {
                opcode = NULL;
                cpu_cycles_waited = 0;
            }
        } else if(CCF(*opcode)) {
            TraceLog(LOG_INFO, "Complement Carry Flag");
            // zero flag remains unmodified
            f_sub = false;
            f_hcarry = false;
            f_carry = !f_carry;
            opcode = NULL;
        } else if(SCF(*opcode)) {
            TraceLog(LOG_INFO, "Set Carry Flag");
            // zero flag remains unmodified
            f_sub = false;
            f_hcarry = false;
            f_carry = true;
            opcode = NULL;
        } else if(DAA(*opcode)) {
            TraceLog(LOG_INFO, "Decimal Adjust Accumulator", *opcode);
            // for representing decimal numbers in binary (basically hex)
            // properly adjusts numbers to be represented properly such that 42 decimal is 0x42 ex.
            // based on blog post from ollien https://blog.ollien.com/posts/gb-daa/

            uint8_t offset = 0;
            bool should_carry = false;
            if ((!f_sub && (rf.AF.l & 0xF) > 0x09) || f_hcarry) {
                offset |= 0x06;
            }
        
            if ((!f_sub && rf.AF.l > 0x99) || f_carry) {
                offset |= 0x60;
                should_carry = true;
            }

            uint8_t result;
            if (!f_sub) {
                result = rf.AF.l + offset;
            } else {
                result = rf.AF.l - offset;
            }
            rf.AF.l = result;

            f_zero = (result == 0);
            f_hcarry = false;
            f_carry = should_carry;

            opcode = NULL;
        } else if(CPL(*opcode)) {
            TraceLog(LOG_INFO, "Complement Accumulator", *opcode);
            rf.AF.l = ~rf.AF.l;

            f_sub = true;
            f_hcarry = true;

            opcode = NULL;
        } else if(INC_RP(*opcode)) {
            if (cpu_cycles_waited == 0) {
                uint8_t target = (*opcode & (0x30)) >> 4;

                // get operands
                uint16_t num;
                switch (target) {
                    case 0: num = rf.BC.lr; break; // BC
                    case 1: num = rf.DE.lr; break; // DE
                    case 2: num = rf.HL.lr; break; // HL
                    case 3: num = rf.SP; break; // SP
                }

                char* target_code;
                switch (target) {
                    case 0: target_code = "BC"; break; // BC
                    case 1: target_code = "DE"; break; // DE
                    case 2: target_code = "HL"; break; // HL
                    case 3: target_code = "SP"; break; // SP
                }
                TraceLog(LOG_INFO, "Increment Target Register Pair %s", target_code);

                // compute increment and set register A to result
                uint16_t result = num + 1;
                switch (target) {
                    case 0: rf.BC.lr = result; break; // BC
                    case 1: rf.DE.lr = result; break; // DE
                    case 2: rf.HL.lr = result; break; // HL
                    case 3: rf.SP = result; break; // SP
                }

                // flags unmodified

                TraceLog(LOG_INFO, "%d + 1 = %d", num, result);

            }
            if (++cpu_cycles_waited >= INC_RP_CYCLES) {
                opcode = NULL;
                cpu_cycles_waited = 0;
            }
        } else if(DEC_RP(*opcode)) {
            if (cpu_cycles_waited == 0) {
                uint8_t target = (*opcode & (0x30)) >> 4;

                // get operands
                uint16_t num;
                switch (target) {
                    case 0: num = rf.BC.lr; break; // BC
                    case 1: num = rf.DE.lr; break; // DE
                    case 2: num = rf.HL.lr; break; // HL
                    case 3: num = rf.SP; break; // SP
                }

                char* target_code;
                switch (target) {
                    case 0: target_code = "BC"; break; // BC
                    case 1: target_code = "DE"; break; // DE
                    case 2: target_code = "HL"; break; // HL
                    case 3: target_code = "SP"; break; // SP
                }
                TraceLog(LOG_INFO, "Decrement Target Register Pair %s", target_code);

                // compute decrement and set register A to result
                uint16_t result = num - 1;
                switch (target) {
                    case 0: rf.BC.lr = result; break; // BC
                    case 1: rf.DE.lr = result; break; // DE
                    case 2: rf.HL.lr = result; break; // HL
                    case 3: rf.SP = result; break; // SP
                }

                // flags unmodified

                TraceLog(LOG_INFO, "%d - 1 = %d", num, result);

            }
            if (++cpu_cycles_waited >= DEC_RP_CYCLES) {
                opcode = NULL;
                cpu_cycles_waited = 0;
            }
        } else if(ADD_HL_RP(*opcode)) {
            if (cpu_cycles_waited == 0) {
                uint8_t target = (*opcode & (0x30)) >> 4;

                // get operands
                uint16_t num1, num2;
                num1 = rf.HL.lr;
                switch (target) {
                    case 0: num2 = rf.BC.lr; break; // BC
                    case 1: num2 = rf.DE.lr; break; // DE
                    case 2: num2 = rf.HL.lr; break; // HL
                    case 3: num2 = rf.SP; break; // SP
                }

                char* target_code;
                switch (target) {
                    case 0: target_code = "BC"; break; // BC
                    case 1: target_code = "DE"; break; // DE
                    case 2: target_code = "HL"; break; // HL
                    case 3: target_code = "SP"; break; // SP
                }
                TraceLog(LOG_INFO, "Add Register HL and Target Register Pair %s", target_code);

                // compute increment and set register A to result
                uint32_t result = num1 + num2;
                switch (target) {
                    case 0: rf.BC.lr = result & 0xFFFF; break; // BC
                    case 1: rf.DE.lr = result & 0xFFFF; break; // DE
                    case 2: rf.HL.lr = result & 0xFFFF; break; // HL
                    case 3: rf.SP = result & 0xFFFF; break; // SP
                }

                // zero flag unmodified
                f_sub = false;
                f_hcarry = ((num1 & 0x0FFF) + (num2 & 0x0FFF)) > 0x0FFF;
                f_carry = result > 0xFFFF;

                TraceLog(LOG_INFO, "%d + %d = %d", num1, num2, result);

            }
            if (++cpu_cycles_waited >= ADD_HL_RP_CYCLES) {
                opcode = NULL;
                cpu_cycles_waited = 0;
            }
        } else if(ADD_SPE(*opcode)) {
            TraceLog(LOG_INFO, "Add SP and Immediate Value e");
            if (cpu_cycles_waited == 0) {
                uint16_t sp = rf.SP;
                int8_t e = *fetch_inst();
                uint16_t result = sp + e;

                rf.SP = result;
                TraceLog(LOG_INFO, "%d + %d = %d", sp, e, result);

                // Only look at lower bytes for carry calculations
                uint8_t low_sp = sp & 0xFF;

                // set flags
                f_zero = false;
                f_sub = false;
                f_hcarry = ((low_sp & 0xF) + (e & 0xF)) > 0xF;
                f_carry = ((low_sp & 0xFF) + (e & 0xFF)) > 0xFF;
            }
            if (++cpu_cycles_waited >= ADD_SPE_CYCLES) {
                opcode = NULL;
                cpu_cycles_waited = 0;
            }
        } else if(RLCA(*opcode)) {
            TraceLog(LOG_INFO, "Rotate Left Circular Accumulator");
            uint8_t bit7 = (rf.AF.l & 0x80) >> 7;
            rf.AF.l = (rf.AF.l << 1) | bit7; // shift left 1, msb becomes lsb

            // set flags
            f_zero = false;
            f_sub = false;
            f_hcarry = false;
            f_carry = (bit7 > 0); // carry bit is == msb prior to operation

            opcode = NULL;
        } else if(RRCA(*opcode)) {
            TraceLog(LOG_INFO, "Rotate Right Circular Accumulator");
            uint8_t bit0 = (rf.AF.l & 0x01) << 7;
            rf.AF.l = (rf.AF.l >> 1) | bit0; // shift right 1, lsb becomes msb

            // set flags
            f_zero = false;
            f_sub = false;
            f_hcarry = false;
            f_carry = (bit0 > 0); // carry bit is == msb prior to operation

            opcode = NULL;
        } else if(RLA(*opcode)) {
            TraceLog(LOG_INFO, "Rotate Left Circular Accumulator");
            uint8_t bit7 = (rf.AF.l & 0x80) >> 7;
            uint8_t old_carry = f_carry ? 1 : 0;
            rf.AF.l = (rf.AF.l << 1) | old_carry; // shift left 1, old carry becomes lsb

            // set flags
            f_zero = false;
            f_sub = false;
            f_hcarry = false;
            f_carry = (bit7 > 0); // carry bit is == msb prior to operation

            opcode = NULL;
        } else if(RRA(*opcode)) {
            TraceLog(LOG_INFO, "Rotate Right Circular Accumulator");
            uint8_t bit0 = (rf.AF.l & 0x01);
            uint8_t old_carry = f_carry ? 0x80 : 0x00;
            rf.AF.l = (rf.AF.l >> 1) | old_carry; // shift right 1, old carry becomes msb

            // set flags
            f_zero = false;
            f_sub = false;
            f_hcarry = false;
            f_carry = (bit0 > 0); // carry bit is == msb prior to operation

            opcode = NULL;
        } else if (*opcode == 0xCB) {
            // CB Prefixed instuction, fetch next byte for "actual" opcode
            if (cb_opcode == NULL) {
                cb_opcode = fetch_inst();
            }

            if(SET_HL(*cb_opcode)) {
                TraceLog(LOG_INFO, "Set Bit HL indirect");
                if (cpu_cycles_waited == 0) {
                    uint8_t bit_target = (*cb_opcode&0x38) >> 3;

                    memory[rf.HL.lr] = memory[rf.HL.lr] | (1 << bit_target);

                    // flags remain unmodified
                }
                if (++cpu_cycles_waited >= SET_HL_CYCLES) {
                    opcode = NULL; cb_opcode = NULL;
                    cpu_cycles_waited = 0;
                }
            } else if(SET(*cb_opcode)) {
                if (cpu_cycles_waited == 0) {
                    uint8_t bit_target = (*cb_opcode&0x38) >> 3;
                    uint8_t reg_target = (*cb_opcode&0x07);

                    switch (reg_target) {
                        case 0: rf.BC.l = rf.BC.l | (1 << bit_target); break;// B
                        case 1: rf.BC.r = rf.BC.r | (1 << bit_target); break;// C
                        case 2: rf.DE.l = rf.DE.l | (1 << bit_target); break;// D
                        case 3: rf.DE.r = rf.DE.r | (1 << bit_target); break;// E
                        case 4: rf.HL.l = rf.HL.l | (1 << bit_target); break;// H
                        case 5: rf.HL.r = rf.HL.r | (1 << bit_target); break;// L
                        case 7: rf.AF.l = rf.AF.l | (1 << bit_target); break;// A
                    }

                    TraceLog(LOG_INFO, "Set Bit %d in Target Register %d", bit_target, reg_target);

                    // flags remain unmodified
                }
                if (++cpu_cycles_waited >= SET_CYCLES) {
                    opcode = NULL; cb_opcode = NULL;
                    cpu_cycles_waited = 0;
                }
            } else if(RLC_HL(*cb_opcode)) {
                TraceLog(LOG_INFO, "RLC_HL", *opcode);
                if (cpu_cycles_waited == 0) {
                    uint8_t num = memory[rf.HL.lr];
                    
                    uint8_t bit7 = (num & 0x80) >> 7;
                    // shift left 1, msb becomes lsb
                    uint8_t result = (num << 1) | bit7;
                    memory[rf.HL.lr] = result;

                    // set flags
                    f_zero = (result == 0);
                    f_sub = false;
                    f_hcarry = false;
                    f_carry = (bit7 > 0); // carry bit is == msb prior to operation
                }
                if (++cpu_cycles_waited >= RLC_HL_CYCLES) {
                    opcode = NULL; cb_opcode = NULL;
                    cpu_cycles_waited = 0;
                }
            } else if(RLC(*cb_opcode)) {
                if (cpu_cycles_waited == 0) {
                    uint8_t target = (*cb_opcode&0x07);
                    TraceLog(LOG_INFO, "Rotate Left Circular Target Register %d", target);
                    
                    uint8_t num;
                    switch (target) {
                        case 0: num = rf.BC.l; break; // B
                        case 1: num = rf.BC.r; break; // C
                        case 2: num = rf.DE.l; break; // D
                        case 3: num = rf.DE.r; break; // E
                        case 4: num = rf.HL.l; break; // H
                        case 5: num = rf.HL.r; break; // L
                        case 7: num = rf.AF.l; break; // A
                    }
                    
                    uint8_t bit7 = (num & 0x80) >> 7;
                    // shift left 1, msb becomes lsb
                    uint8_t result = (num << 1) | bit7;
                    switch (target) {
                        case 0: rf.BC.l = result; break; // B
                        case 1: rf.BC.r = result; break; // C
                        case 2: rf.DE.l = result; break; // D
                        case 3: rf.DE.r = result; break; // E
                        case 4: rf.HL.l = result; break; // H
                        case 5: rf.HL.r = result; break; // L
                        case 7: rf.AF.l = result; break; // A
                    }

                    // set flags
                    f_zero = (result == 0);
                    f_sub = false;
                    f_hcarry = false;
                    f_carry = (bit7 > 0); // carry bit is == msb prior to operation
                }
                if (++cpu_cycles_waited >= RLC_CYCLES) {
                    opcode = NULL; cb_opcode = NULL;
                    cpu_cycles_waited = 0;
                }
            } else if(RRC_HL(*cb_opcode)) {
                TraceLog(LOG_INFO, "RRC_HL", *opcode);
                if (cpu_cycles_waited == 0) {
                    uint8_t num = memory[rf.HL.lr];
                    
                    uint8_t bit0 = (num & 0x01) << 7;
                    uint8_t result = (num >> 1) | bit0; // shift right 1, lsb becomes msb
                    memory[rf.HL.lr] = result;

                    // set flags
                    f_zero = (result == 0);
                    f_sub = false;
                    f_hcarry = false;
                    f_carry = (bit0 > 0); // carry bit is == msb prior to operation
                }
                if (++cpu_cycles_waited >= RRC_HL_CYCLES) {
                    opcode = NULL; cb_opcode = NULL;
                    cpu_cycles_waited = 0;
                }
            } else if(RRC(*cb_opcode)) {
                if (cpu_cycles_waited == 0) {
                    uint8_t target = (*cb_opcode&0x07);
                    TraceLog(LOG_INFO, "Rotate Right Circular Target Register %d", target);
                    
                    uint8_t num;
                    switch (target) {
                        case 0: num = rf.BC.l; break; // B
                        case 1: num = rf.BC.r; break; // C
                        case 2: num = rf.DE.l; break; // D
                        case 3: num = rf.DE.r; break; // E
                        case 4: num = rf.HL.l; break; // H
                        case 5: num = rf.HL.r; break; // L
                        case 7: num = rf.AF.l; break; // A
                    }
                    
                    uint8_t bit0 = (num & 0x01) << 7;
                    uint8_t result = (num >> 1) | bit0; // shift right 1, lsb becomes msb
                    switch (target) {
                        case 0: rf.BC.l = result; break; // B
                        case 1: rf.BC.r = result; break; // C
                        case 2: rf.DE.l = result; break; // D
                        case 3: rf.DE.r = result; break; // E
                        case 4: rf.HL.l = result; break; // H
                        case 5: rf.HL.r = result; break; // L
                        case 7: rf.AF.l = result; break; // A
                    }

                    // set flags
                    f_zero = (result == 0);
                    f_sub = false;
                    f_hcarry = false;
                    f_carry = (bit0 > 0); // carry bit is == msb prior to operation
                }
                if (++cpu_cycles_waited >= RRC_CYCLES) {
                    opcode = NULL; cb_opcode = NULL;
                    cpu_cycles_waited = 0;
                }
            } else if(RL_HL(*cb_opcode)) {
                TraceLog(LOG_INFO, "RL_HL", *opcode);
                if (cpu_cycles_waited == 0) {
                    uint8_t num = memory[rf.HL.lr];

                    uint8_t bit7 = (num & 0x80) >> 7;
                    uint8_t old_carry = f_carry ? 1 : 0;
                    uint8_t result = (num << 1) | old_carry; // shift left 1, old carry becomes lsb
                    memory[rf.HL.lr] = result;

                    // set flags
                    f_zero = (result == 0);
                    f_sub = false;
                    f_hcarry = false;
                    f_carry = (bit7 > 0); // carry bit is == msb prior to operation
                }
                if (++cpu_cycles_waited >= RL_HL_CYCLES) {
                    opcode = NULL; cb_opcode = NULL;
                    cpu_cycles_waited = 0;
                }
            } else if(RL(*cb_opcode)) {
                if (cpu_cycles_waited == 0) {
                    uint8_t target = (*cb_opcode&0x07);
                    TraceLog(LOG_INFO, "Rotate Left Target Register %d", target);

                    uint8_t num;
                    switch (target) {
                        case 0: num = rf.BC.l; break; // B
                        case 1: num = rf.BC.r; break; // C
                        case 2: num = rf.DE.l; break; // D
                        case 3: num = rf.DE.r; break; // E
                        case 4: num = rf.HL.l; break; // H
                        case 5: num = rf.HL.r; break; // L
                        case 7: num = rf.AF.l; break; // A
                    }

                    uint8_t bit7 = (num & 0x80) >> 7;
                    uint8_t old_carry = f_carry ? 1 : 0;
                    uint8_t result = (num << 1) | old_carry; // shift left 1, old carry becomes lsb
                    switch (target) {
                        case 0: rf.BC.l = result; break; // B
                        case 1: rf.BC.r = result; break; // C
                        case 2: rf.DE.l = result; break; // D
                        case 3: rf.DE.r = result; break; // E
                        case 4: rf.HL.l = result; break; // H
                        case 5: rf.HL.r = result; break; // L
                        case 7: rf.AF.l = result; break; // A
                    }

                    // set flags
                    f_zero = (result == 0);
                    f_sub = false;
                    f_hcarry = false;
                    f_carry = (bit7 > 0); // carry bit is == msb prior to operation
                }
                if (++cpu_cycles_waited >= RL_CYCLES) {
                    opcode = NULL; cb_opcode = NULL;
                    cpu_cycles_waited = 0;
                }
            } else if(RR_HL(*cb_opcode)) {
                TraceLog(LOG_INFO, "RR_HL", *opcode);
                if (cpu_cycles_waited == 0) {
                    uint8_t num = memory[rf.HL.lr];

                    uint8_t bit0 = (num & 0x01);
                    uint8_t old_carry = f_carry ? 0x80 : 0x00;
                    uint8_t result = (num >> 1) | old_carry; // shift right 1, old carry becomes msb
                    memory[rf.HL.lr] = result;

                    // set flags
                    f_zero = (result == 0);
                    f_sub = false;
                    f_hcarry = false;
                    f_carry = (bit0 > 0); // carry bit is == msb prior to operation
                }
                if (++cpu_cycles_waited >= RR_HL_CYCLES) {
                    opcode = NULL; cb_opcode = NULL;
                    cpu_cycles_waited = 0;
                }
            } else if(RR(*cb_opcode)) {
                if (cpu_cycles_waited == 0) {
                    uint8_t target = (*cb_opcode&0x07);
                    TraceLog(LOG_INFO, "Rotate Right Target Register %d", target);

                    uint8_t num;
                    switch (target) {
                        case 0: num = rf.BC.l; break; // B
                        case 1: num = rf.BC.r; break; // C
                        case 2: num = rf.DE.l; break; // D
                        case 3: num = rf.DE.r; break; // E
                        case 4: num = rf.HL.l; break; // H
                        case 5: num = rf.HL.r; break; // L
                        case 7: num = rf.AF.l; break; // A
                    }

                    uint8_t bit0 = (num & 0x01);
                    uint8_t old_carry = f_carry ? 0x80 : 0x00;
                    uint8_t result = (num >> 1) | old_carry; // shift right 1, old carry becomes msb
                    switch (target) {
                        case 0: rf.BC.l = result; break; // B
                        case 1: rf.BC.r = result; break; // C
                        case 2: rf.DE.l = result; break; // D
                        case 3: rf.DE.r = result; break; // E
                        case 4: rf.HL.l = result; break; // H
                        case 5: rf.HL.r = result; break; // L
                        case 7: rf.AF.l = result; break; // A
                    }

                    // set flags
                    f_zero = (result == 0);
                    f_sub = false;
                    f_hcarry = false;
                    f_carry = (bit0 > 0); // carry bit is == msb prior to operation
                }
                if (++cpu_cycles_waited >= RR_CYCLES) {
                    opcode = NULL; cb_opcode = NULL;
                    cpu_cycles_waited = 0;
                }
            } else if(SLA_HL(*cb_opcode)) {
                TraceLog(LOG_INFO, "SLA_HL", *opcode);
                if (cpu_cycles_waited == 0) {
                    uint8_t num = memory[rf.HL.lr];

                    uint8_t result = num << 1;
                    memory[rf.HL.lr] = result;

                    f_zero = (result==0);
                    f_sub = false;
                    f_hcarry = false;
                    f_carry = ((num&0x80) > 0);
                }
                if (++cpu_cycles_waited >= SLA_HL_CYCLES) {
                    opcode = NULL; cb_opcode = NULL;
                    cpu_cycles_waited = 0;
                }
            } else if(SLA(*cb_opcode)) {
                if (cpu_cycles_waited == 0) {
                    uint8_t target = (*cb_opcode&0x07);
                    TraceLog(LOG_INFO, "Shift Left Arithmetic Target Register %d", target);

                    uint8_t num;
                    switch (target) {
                        case 0: num = rf.BC.l; break; // B
                        case 1: num = rf.BC.r; break; // C
                        case 2: num = rf.DE.l; break; // D
                        case 3: num = rf.DE.r; break; // E
                        case 4: num = rf.HL.l; break; // H
                        case 5: num = rf.HL.r; break; // L
                        case 7: num = rf.AF.l; break; // A
                    }

                    uint8_t result = num << 1;
                    switch (target) {
                        case 0: rf.BC.l = result; break; // B
                        case 1: rf.BC.r = result; break; // C
                        case 2: rf.DE.l = result; break; // D
                        case 3: rf.DE.r = result; break; // E
                        case 4: rf.HL.l = result; break; // H
                        case 5: rf.HL.r = result; break; // L
                        case 7: rf.AF.l = result; break; // A
                    }

                    f_zero = (result==0);
                    f_sub = false;
                    f_hcarry = false;
                    f_carry = ((num&0x80) > 0);
                }
                if (++cpu_cycles_waited >= SLA_CYCLES) {
                    opcode = NULL; cb_opcode = NULL;
                    cpu_cycles_waited = 0;
                }
            } else if(SRA_HL(*cb_opcode)) {
                TraceLog(LOG_INFO, "SRA_HL", *opcode);
                if (cpu_cycles_waited == 0) {
                    uint8_t num = memory[rf.HL.lr];

                    uint8_t result = (num >> 1) | (num&0x80); // maintain leftmost bit for sign
                    memory[rf.HL.lr] = result;

                    f_zero = (result==0);
                    f_sub = false;
                    f_hcarry = false;
                    f_carry = ((num&0x01) > 0);
                }
                if (++cpu_cycles_waited >= SRA_HL_CYCLES) {
                    opcode = NULL; cb_opcode = NULL;
                    cpu_cycles_waited = 0;
                }
            } else if(SRA(*cb_opcode)) {
                if (cpu_cycles_waited == 0) {
                    uint8_t target = (*cb_opcode&0x07);
                    TraceLog(LOG_INFO, "Shift Right Arithmetic Target Register %d", target);

                    uint8_t num;
                    switch (target) {
                        case 0: num = rf.BC.l; break; // B
                        case 1: num = rf.BC.r; break; // C
                        case 2: num = rf.DE.l; break; // D
                        case 3: num = rf.DE.r; break; // E
                        case 4: num = rf.HL.l; break; // H
                        case 5: num = rf.HL.r; break; // L
                        case 7: num = rf.AF.l; break; // A
                    }

                    uint8_t result = (num >> 1) | (num&0x80); // maintain leftmost bit for sign
                    switch (target) {
                        case 0: rf.BC.l = result; break; // B
                        case 1: rf.BC.r = result; break; // C
                        case 2: rf.DE.l = result; break; // D
                        case 3: rf.DE.r = result; break; // E
                        case 4: rf.HL.l = result; break; // H
                        case 5: rf.HL.r = result; break; // L
                        case 7: rf.AF.l = result; break; // A
                    }

                    f_zero = (result==0);
                    f_sub = false;
                    f_hcarry = false;
                    f_carry = ((num&0x01) > 0);
                }
                if (++cpu_cycles_waited >= SRA_CYCLES) {
                    opcode = NULL; cb_opcode = NULL;
                    cpu_cycles_waited = 0;
                }
            } else if(SWAP_HL(*cb_opcode)) {
                TraceLog(LOG_INFO, "SWAP_HL", *opcode);
                if (cpu_cycles_waited == 0) {
                    uint8_t num = memory[rf.HL.lr];

                    uint8_t result = (num << 4) | (num >> 4); // maintain leftmost bit for sign
                    result = memory[rf.HL.lr];

                    f_zero = (result==0);
                    f_sub = false;
                    f_hcarry = false;
                    f_carry = false;
                }
                if (++cpu_cycles_waited >= SWAP_HL_CYCLES) {
                    opcode = NULL; cb_opcode = NULL;
                    cpu_cycles_waited = 0;
                }
            } else if(SWAP(*cb_opcode)) {
                if (cpu_cycles_waited == 0) {
                    uint8_t target = (*cb_opcode&0x07);
                    TraceLog(LOG_INFO, "Swap Nibbles Target Register %d", target);

                    uint8_t num;
                    switch (target) {
                        case 0: num = rf.BC.l; break; // B
                        case 1: num = rf.BC.r; break; // C
                        case 2: num = rf.DE.l; break; // D
                        case 3: num = rf.DE.r; break; // E
                        case 4: num = rf.HL.l; break; // H
                        case 5: num = rf.HL.r; break; // L
                        case 7: num = rf.AF.l; break; // A
                    }

                    uint8_t result = (num << 4) | (num >> 4); // maintain leftmost bit for sign
                    switch (target) {
                        case 0: rf.BC.l = result; break; // B
                        case 1: rf.BC.r = result; break; // C
                        case 2: rf.DE.l = result; break; // D
                        case 3: rf.DE.r = result; break; // E
                        case 4: rf.HL.l = result; break; // H
                        case 5: rf.HL.r = result; break; // L
                        case 7: rf.AF.l = result; break; // A
                    }

                    f_zero = (result==0);
                    f_sub = false;
                    f_hcarry = false;
                    f_carry = false;
                }
                if (++cpu_cycles_waited >= SWAP_CYCLES) {
                    opcode = NULL; cb_opcode = NULL;
                    cpu_cycles_waited = 0;
                }
            } else if(SRL_HL(*cb_opcode)) {
                TraceLog(LOG_INFO, "SRL_HL", *opcode);
                if (cpu_cycles_waited == 0) {
                    uint8_t num = memory[rf.HL.lr];

                    uint8_t result = (num >> 1); // maintain leftmost bit for sign
                    memory[rf.HL.lr] = result;

                    f_zero = (result==0);
                    f_sub = false;
                    f_hcarry = false;
                    f_carry = ((num&0x01) > 0);
                }
                if (++cpu_cycles_waited >= SRL_HL_CYCLES) {
                    opcode = NULL; cb_opcode = NULL;
                    cpu_cycles_waited = 0;
                }
            } else if(SRL(*cb_opcode)) {
                if (cpu_cycles_waited == 0) {
                    uint8_t target = (*cb_opcode&0x07);
                    TraceLog(LOG_INFO, "Shift Right Logical Target Register %d", target);

                    uint8_t num;
                    switch (target) {
                        case 0: num = rf.BC.l; break; // B
                        case 1: num = rf.BC.r; break; // C
                        case 2: num = rf.DE.l; break; // D
                        case 3: num = rf.DE.r; break; // E
                        case 4: num = rf.HL.l; break; // H
                        case 5: num = rf.HL.r; break; // L
                        case 7: num = rf.AF.l; break; // A
                    }

                    uint8_t result = (num >> 1); // maintain leftmost bit for sign
                    switch (target) {
                        case 0: rf.BC.l = result; break; // B
                        case 1: rf.BC.r = result; break; // C
                        case 2: rf.DE.l = result; break; // D
                        case 3: rf.DE.r = result; break; // E
                        case 4: rf.HL.l = result; break; // H
                        case 5: rf.HL.r = result; break; // L
                        case 7: rf.AF.l = result; break; // A
                    }

                    f_zero = (result==0);
                    f_sub = false;
                    f_hcarry = false;
                    f_carry = ((num&0x01) > 0);
                }
                if (++cpu_cycles_waited >= SRL_CYCLES) {
                    opcode = NULL; cb_opcode = NULL;
                    cpu_cycles_waited = 0;
                }
            } else if(BIT_HL(*cb_opcode)) {
                TraceLog(LOG_INFO, "BIT_HL", *opcode);
                if (cpu_cycles_waited == 0) {
                    uint8_t bit_target = (*cb_opcode&0x38) >> 3;

                    uint8_t num = memory[rf.HL.lr];

                    f_zero = (num&(1<<bit_target));
                    f_sub = false;
                    f_hcarry = true;
                    // carry flag unmodified
                }
                if (++cpu_cycles_waited >= BIT_HL_CYCLES) {
                    opcode = NULL; cb_opcode = NULL;
                    cpu_cycles_waited = 0;
                }
            } else if(BIT(*cb_opcode)) {
                if (cpu_cycles_waited == 0) {
                    uint8_t bit_target = (*cb_opcode&0x38) >> 3;
                    uint8_t reg_target = (*cb_opcode&0x07);
                    TraceLog(LOG_INFO, "Test Bit %d in Target Register %d", bit_target, reg_target);

                    uint8_t num;
                    switch (reg_target) {
                        case 0: num = rf.BC.l; break; // B
                        case 1: num = rf.BC.r; break; // C
                        case 2: num = rf.DE.l; break; // D
                        case 3: num = rf.DE.r; break; // E
                        case 4: num = rf.HL.l; break; // H
                        case 5: num = rf.HL.r; break; // L
                        case 7: num = rf.AF.l; break; // A
                    }

                    f_zero = (num&(1<<bit_target));
                    f_sub = false;
                    f_hcarry = true;
                    // carry flag unmodified
                }
                if (++cpu_cycles_waited >= BIT_CYCLES) {
                    opcode = NULL; cb_opcode = NULL;
                    cpu_cycles_waited = 0;
                }
            } else if(RES_HL(*cb_opcode)) {
                TraceLog(LOG_INFO, "RES_HL", *opcode);
                if (cpu_cycles_waited == 0) {
                    uint8_t bit_target = (*cb_opcode&0x38) >> 3;

                    memory[rf.HL.lr] = memory[rf.HL.lr] & ~(1 << bit_target);

                    // flags remain unmodified
                }
                if (++cpu_cycles_waited >= RES_HL_CYCLES) {
                    opcode = NULL; cb_opcode = NULL;
                    cpu_cycles_waited = 0;
                }
            } else if(RES(*cb_opcode)) {
                if (cpu_cycles_waited == 0) {
                    uint8_t bit_target = (*cb_opcode&0x38) >> 3;
                    uint8_t reg_target = (*cb_opcode&0x07);

                    switch (reg_target) {
                        case 0: rf.BC.l = rf.BC.l & ~(1 << bit_target); break;// B
                        case 1: rf.BC.r = rf.BC.r & ~(1 << bit_target); break;// C
                        case 2: rf.DE.l = rf.DE.l & ~(1 << bit_target); break;// D
                        case 3: rf.DE.r = rf.DE.r & ~(1 << bit_target); break;// E
                        case 4: rf.HL.l = rf.HL.l & ~(1 << bit_target); break;// H
                        case 5: rf.HL.r = rf.HL.r & ~(1 << bit_target); break;// L
                        case 7: rf.AF.l = rf.AF.l & ~(1 << bit_target); break;// A
                    }

                    TraceLog(LOG_INFO, "Reset Bit %d in Target Register %d", bit_target, reg_target);

                    // flags remain unmodified
                }
                if (++cpu_cycles_waited >= RES_CYCLES) {
                    opcode = NULL; cb_opcode = NULL;
                    cpu_cycles_waited = 0;
                }
            } else {
                // undefined opcode
                TraceLog(LOG_INFO, "BAD OPCODE???");
                opcode = NULL; cb_opcode = NULL;
            }
        } else if(JP_II(*opcode)) {
            if (cpu_cycles_waited == 0) {
                // DMG is little endian, so read lsbyte then msbyte for 16-bit address
                uint16_t nn = (uint16_t)*fetch_inst() | ((uint16_t)*fetch_inst() << 8);
                TraceLog(LOG_INFO, "Jump Uncoditionally to Immediate Addr %d", nn);
                rf.PC = nn;
            }
            if (++cpu_cycles_waited >= JP_II_CYCLES) {
                opcode = NULL;
                cpu_cycles_waited = 0;
            }
        } else if(JP_HL(*opcode)) {
            TraceLog(LOG_INFO, "Jump Uncoditionally to Addr in Register HL %d", rf.HL.lr);
            rf.PC = rf.HL.lr;
            opcode = NULL;
        } else if(JPC(*opcode)) {
            if (cpu_cycles_waited == 0) {
                // DMG is little endian, so read lsbyte then msbyte for 16-bit address
                uint16_t nn = (uint16_t)*fetch_inst() | ((uint16_t)*fetch_inst() << 8);
                uint8_t condition = (*opcode&0x18)>>3;

                switch (condition) {
                    case 0: jump_cond = !f_zero;
                        TraceLog(LOG_INFO, "Jump Not Zero to Immediate Addr %d", nn);
                        break;
                    case 1: jump_cond = f_zero;
                        TraceLog(LOG_INFO, "Jump Zero to Immediate Addr %d", nn);
                        break;
                    case 2: jump_cond = !f_carry;
                        TraceLog(LOG_INFO, "Jump Not Carry to Immediate Addr %d", nn);
                        break;
                    case 3: jump_cond = f_carry;
                        TraceLog(LOG_INFO, "Jump Carry to Immediate Addr %d", nn);
                        break;
                }

                if (jump_cond) {
                    rf.PC = nn;
                }
            }
            if (jump_cond) {
                if (++cpu_cycles_waited >= JPC_TRUE_CYCLES) {
                    opcode = NULL;
                    cpu_cycles_waited = 0;
                    jump_cond = false;
                }
            } else {
                if (++cpu_cycles_waited >= JPC_FALSE_CYCLES) {
                    opcode = NULL;
                    cpu_cycles_waited = 0;
                    jump_cond = false;
                }
            }
        } else if(JR(*opcode)) {
            if (cpu_cycles_waited == 0) {
                // Signed operand for relative jump
                int8_t e = (int8_t)*fetch_inst();
                TraceLog(LOG_INFO, "Jump Uncoditionally to Relative Addr PC + %d", e);
                rf.PC = rf.PC + e;
            }
            if (++cpu_cycles_waited >= JR_CYCLES) {
                opcode = NULL;
                cpu_cycles_waited = 0;
            }
        } else if(JRC(*opcode)) {
            if (cpu_cycles_waited == 0) {
                // Signed operand for relative jump
                int8_t e = (int8_t)*fetch_inst();
                uint8_t condition = (*opcode&0x18)>>3;

                switch (condition) {
                    case 0: jump_cond = !f_zero;
                        TraceLog(LOG_INFO, "Jump Not Zero to Relative Addr PC + %d", e);
                        break;
                    case 1: jump_cond = f_zero;
                        TraceLog(LOG_INFO, "Jump Zero to Relative Addr PC + %d", e);
                        break;
                    case 2: jump_cond = !f_carry;
                        TraceLog(LOG_INFO, "Jump Not Carry to Relative Addr PC + %d", e);
                        break;
                    case 3: jump_cond = f_carry;
                        TraceLog(LOG_INFO, "Jump Carry to Relative Addr PC + %d", e);
                        break;
                }

                if (jump_cond) {
                    rf.PC = rf.PC + e;
                }
            }
            if (jump_cond) {
                if (++cpu_cycles_waited >= JRC_TRUE_CYCLES) {
                    opcode = NULL;
                    cpu_cycles_waited = 0;
                    jump_cond = false;
                }
            } else {
                if (++cpu_cycles_waited >= JRC_FALSE_CYCLES) {
                    opcode = NULL;
                    cpu_cycles_waited = 0;
                    jump_cond = false;
                }
            }
        } else if(CALL(*opcode)) {
            TraceLog(LOG_INFO, "CALL");
            if (cpu_cycles_waited == 0) {
                // DMG is little endian, so read lsbyte then msbyte for 16-bit address
                uint16_t nn = (uint16_t)*fetch_inst() | ((uint16_t)*fetch_inst() << 8);
                
                // push to stack
                memory[--rf.SP] = (uint8_t)(nn&0x00FF); // lsbyte
                memory[--rf.SP] = (uint8_t)(nn&0xFF00); // msbyte

                // jump
                rf.PC = nn;
            }
            if (++cpu_cycles_waited >= CALL_CYCLES) {
                opcode = NULL;
                cpu_cycles_waited = 0;
            }
        } else if(CALLC(*opcode)) {
            TraceLog(LOG_INFO, "CALLC", *opcode);
            if (cpu_cycles_waited == 0) {
                // DMG is little endian, so read lsbyte then msbyte for 16-bit address
                uint16_t nn = (uint16_t)*fetch_inst() | ((uint16_t)*fetch_inst() << 8);
                uint8_t condition = (*opcode&0x18)>>3;

                switch (condition) {
                    case 0: jump_cond = !f_zero;
                        TraceLog(LOG_INFO, "Jump Not Zero to Immediate Addr %d", nn);
                        break;
                    case 1: jump_cond = f_zero;
                        TraceLog(LOG_INFO, "Jump Zero to Immediate Addr %d", nn);
                        break;
                    case 2: jump_cond = !f_carry;
                        TraceLog(LOG_INFO, "Jump Not Carry to Immediate Addr %d", nn);
                        break;
                    case 3: jump_cond = f_carry;
                        TraceLog(LOG_INFO, "Jump Carry to Immediate Addr %d", nn);
                        break;
                }

                if (jump_cond) {
                    // push to stack
                    memory[--rf.SP] = (uint8_t)(nn&0x00FF); // lsbyte
                    memory[--rf.SP] = (uint8_t)(nn&0xFF00); // msbyte
                    // jump
                    rf.PC = nn;
                }
            }
            if (jump_cond) {
                if (++cpu_cycles_waited >= CALLC_TRUE_CYCLES) {
                    opcode = NULL;
                    cpu_cycles_waited = 0;
                    jump_cond = false;
                }
            } else {
                if (++cpu_cycles_waited >= CALLC_FALSE_CYCLES) {
                    opcode = NULL;
                    cpu_cycles_waited = 0;
                    jump_cond = false;
                }
            }
        } else if(RET(*opcode)) {
            TraceLog(LOG_INFO, "RET", *opcode);
            if (cpu_cycles_waited == 0) {
                uint8_t lsb = memory[rf.SP++]; // lsbyte
                uint8_t msb = memory[rf.SP++]; // msbyte
                uint16_t val = ((uint16_t)msb << 8) | lsb;

                // jump
                rf.PC = val;
            }
            if (++cpu_cycles_waited >= RET_CYCLES) {
                opcode = NULL; cb_opcode = NULL;
                cpu_cycles_waited = 0;
            }
        } else if(RETC(*opcode)) {
            TraceLog(LOG_INFO, "RETC", *opcode);
            if (cpu_cycles_waited == 0) {
                // DMG is little endian, so read lsbyte then msbyte for 16-bit address
                uint8_t condition = (*opcode&0x18)>>3;

                switch (condition) {
                    case 0: jump_cond = !f_zero;
                        TraceLog(LOG_INFO, "Jump Not Zero to Immediate Addr %d", nn);
                        break;
                    case 1: jump_cond = f_zero;
                        TraceLog(LOG_INFO, "Jump Zero to Immediate Addr %d", nn);
                        break;
                    case 2: jump_cond = !f_carry;
                        TraceLog(LOG_INFO, "Jump Not Carry to Immediate Addr %d", nn);
                        break;
                    case 3: jump_cond = f_carry;
                        TraceLog(LOG_INFO, "Jump Carry to Immediate Addr %d", nn);
                        break;
                }

                if (jump_cond) {
                    // pop froms stack
                    uint8_t lsb = memory[rf.SP++]; // lsbyte
                    uint8_t msb = memory[rf.SP++]; // msbyte
                    uint16_t val = ((uint16_t)msb << 8) | lsb;

                    // jump
                    rf.PC = val;
                }
            }
            if (jump_cond) {
                if (++cpu_cycles_waited >= RETC_TRUE_CYCLES) {
                    opcode = NULL;
                    cpu_cycles_waited = 0;
                    jump_cond = false;
                }
            } else {
                if (++cpu_cycles_waited >= RETC_FALSE_CYCLES) {
                    opcode = NULL;
                    cpu_cycles_waited = 0;
                    jump_cond = false;
                }
            }
        } else if(RETI(*opcode)) {
            TraceLog(LOG_INFO, "RETI", *opcode);
            if (cpu_cycles_waited == 0) {
                // pop from stack
                uint8_t lsb = memory[rf.SP++]; // lsbyte
                uint8_t msb = memory[rf.SP++]; // msbyte
                uint16_t val = ((uint16_t)msb << 8) | lsb;

                // enable interrupts
                rf.IME = true;

                // jump
                rf.PC = val;
            }
            if (++cpu_cycles_waited >= RETI_CYCLES) {
                opcode = NULL; cb_opcode = NULL;
                cpu_cycles_waited = 0;
            }
        } else if(RST(*opcode)) {
            TraceLog(LOG_INFO, "RST");
            if (cpu_cycles_waited == 0) {
                uint8_t n = (*opcode&0x18)>>3;

                // push to stack
                memory[--rf.SP] = (uint8_t)(nn&0x00FF); // lsbyte
                memory[--rf.SP] = (uint8_t)(nn&0xFF00); // msbyte

                // jump
                rf.PC = 0x0000 | (uint16_t)n;
            }
            if (++cpu_cycles_waited >= RST_CYCLES) {
                opcode = NULL;
                cpu_cycles_waited = 0;
            }
        } else if(STOP(*opcode)) {
            TraceLog(LOG_INFO, "STOP", *opcode);
            cpu_stopped = true;
            opcode = NULL;
        } else if(DI(*opcode)) {
            TraceLog(LOG_INFO, "DI", *opcode);
            rf.IME = false;
            to_enable_ime = false;
            opcode = NULL;
        } else if(EI(*opcode)) {
            TraceLog(LOG_INFO, "EI", *opcode);
            to_enable_ime = true;
            opcode = NULL;
        } else {
            // undefined opcode
            TraceLog(LOG_INFO, "BAD OPCODE???");
            opcode = NULL;
        }
        write_flags(); // update flag register
    }

    // if EI on last instruction, enable IME
    if (to_enable_ime) {
        rf.IME = true;
        to_enable_ime = false;
    }

    // then, if execution is finished, fetch the next instruction
    if !cpu_stopped && (opcode == NULL) {
        // previous execution is done
        opcode = fetch_inst();
    }
}
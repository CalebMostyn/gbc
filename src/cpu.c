#include "cpu.h"
#include "instructions.h"
#include <stdint.h>
#include <stddef.h>
#include "raylib.h"

register_file rf;
bool f_zero, f_sub, f_carry, f_hcarry = false;

uint8_t memory[0x10000] = {
0xCB, 0x00, 0xCB, 0x01, 0xCB, 0x02, 0xCB, 0x03, 0xCB, 0x04, 0xCB, 0x05, 0xCB, 0x06, 0xCB, 0x07, 0xCB, 0x08, 0xCB, 0x09, 0xCB, 0x0A, 0xCB, 0x0B, 0xCB, 0x0C, 0xCB, 0x0D, 0xCB, 0x0E, 0xCB, 0x0F,
0xCB, 0x10, 0xCB, 0x11, 0xCB, 0x12, 0xCB, 0x13, 0xCB, 0x14, 0xCB, 0x15, 0xCB, 0x16, 0xCB, 0x17, 0xCB, 0x18, 0xCB, 0x19, 0xCB, 0x1A, 0xCB, 0x1B, 0xCB, 0x1C, 0xCB, 0x1D, 0xCB, 0x1E, 0xCB, 0x1F,
0xCB, 0x20, 0xCB, 0x21, 0xCB, 0x22, 0xCB, 0x23, 0xCB, 0x24, 0xCB, 0x25, 0xCB, 0x26, 0xCB, 0x27, 0xCB, 0x28, 0xCB, 0x29, 0xCB, 0x2A, 0xCB, 0x2B, 0xCB, 0x2C, 0xCB, 0x2D, 0xCB, 0x2E, 0xCB, 0x2F,
0xCB, 0x30, 0xCB, 0x31, 0xCB, 0x32, 0xCB, 0x33, 0xCB, 0x34, 0xCB, 0x35, 0xCB, 0x36, 0xCB, 0x37, 0xCB, 0x38, 0xCB, 0x39, 0xCB, 0x3A, 0xCB, 0x3B, 0xCB, 0x3C, 0xCB, 0x3D, 0xCB, 0x3E, 0xCB, 0x3F,
0xCB, 0x40, 0xCB, 0x41, 0xCB, 0x42, 0xCB, 0x43, 0xCB, 0x44, 0xCB, 0x45, 0xCB, 0x46, 0xCB, 0x47, 0xCB, 0x48, 0xCB, 0x49, 0xCB, 0x4A, 0xCB, 0x4B, 0xCB, 0x4C, 0xCB, 0x4D, 0xCB, 0x4E, 0xCB, 0x4F,
0xCB, 0x50, 0xCB, 0x51, 0xCB, 0x52, 0xCB, 0x53, 0xCB, 0x54, 0xCB, 0x55, 0xCB, 0x56, 0xCB, 0x57, 0xCB, 0x58, 0xCB, 0x59, 0xCB, 0x5A, 0xCB, 0x5B, 0xCB, 0x5C, 0xCB, 0x5D, 0xCB, 0x5E, 0xCB, 0x5F,
0xCB, 0x60, 0xCB, 0x61, 0xCB, 0x62, 0xCB, 0x63, 0xCB, 0x64, 0xCB, 0x65, 0xCB, 0x66, 0xCB, 0x67, 0xCB, 0x68, 0xCB, 0x69, 0xCB, 0x6A, 0xCB, 0x6B, 0xCB, 0x6C, 0xCB, 0x6D, 0xCB, 0x6E, 0xCB, 0x6F,
0xCB, 0x70, 0xCB, 0x71, 0xCB, 0x72, 0xCB, 0x73, 0xCB, 0x74, 0xCB, 0x75, 0xCB, 0x76, 0xCB, 0x77, 0xCB, 0x78, 0xCB, 0x79, 0xCB, 0x7A, 0xCB, 0x7B, 0xCB, 0x7C, 0xCB, 0x7D, 0xCB, 0x7E, 0xCB, 0x7F,
0xCB, 0x80, 0xCB, 0x81, 0xCB, 0x82, 0xCB, 0x83, 0xCB, 0x84, 0xCB, 0x85, 0xCB, 0x86, 0xCB, 0x87, 0xCB, 0x88, 0xCB, 0x89, 0xCB, 0x8A, 0xCB, 0x8B, 0xCB, 0x8C, 0xCB, 0x8D, 0xCB, 0x8E, 0xCB, 0x8F,
0xCB, 0x90, 0xCB, 0x91, 0xCB, 0x92, 0xCB, 0x93, 0xCB, 0x94, 0xCB, 0x95, 0xCB, 0x96, 0xCB, 0x97, 0xCB, 0x98, 0xCB, 0x99, 0xCB, 0x9A, 0xCB, 0x9B, 0xCB, 0x9C, 0xCB, 0x9D, 0xCB, 0x9E, 0xCB, 0x9F,
0xCB, 0xA0, 0xCB, 0xA1, 0xCB, 0xA2, 0xCB, 0xA3, 0xCB, 0xA4, 0xCB, 0xA5, 0xCB, 0xA6, 0xCB, 0xA7, 0xCB, 0xA8, 0xCB, 0xA9, 0xCB, 0xAA, 0xCB, 0xAB, 0xCB, 0xAC, 0xCB, 0xAD, 0xCB, 0xAE, 0xCB, 0xAF,
0xCB, 0xB0, 0xCB, 0xB1, 0xCB, 0xB2, 0xCB, 0xB3, 0xCB, 0xB4, 0xCB, 0xB5, 0xCB, 0xB6, 0xCB, 0xB7, 0xCB, 0xB8, 0xCB, 0xB9, 0xCB, 0xBA, 0xCB, 0xBB, 0xCB, 0xBC, 0xCB, 0xBD, 0xCB, 0xBE, 0xCB, 0xBF,
0xCB, 0xC0, 0xCB, 0xC1, 0xCB, 0xC2, 0xCB, 0xC3, 0xCB, 0xC4, 0xCB, 0xC5, 0xCB, 0xC6, 0xCB, 0xC7, 0xCB, 0xC8, 0xCB, 0xC9, 0xCB, 0xCA, 0xCB, 0xCB, 0xCB, 0xCC, 0xCB, 0xCD, 0xCB, 0xCE, 0xCB, 0xCF,
0xCB, 0xD0, 0xCB, 0xD1, 0xCB, 0xD2, 0xCB, 0xD3, 0xCB, 0xD4, 0xCB, 0xD5, 0xCB, 0xD6, 0xCB, 0xD7, 0xCB, 0xD8, 0xCB, 0xD9, 0xCB, 0xDA, 0xCB, 0xDB, 0xCB, 0xDC, 0xCB, 0xDD, 0xCB, 0xDE, 0xCB, 0xDF,
0xCB, 0xE0, 0xCB, 0xE1, 0xCB, 0xE2, 0xCB, 0xE3, 0xCB, 0xE4, 0xCB, 0xE5, 0xCB, 0xE6, 0xCB, 0xE7, 0xCB, 0xE8, 0xCB, 0xE9, 0xCB, 0xEA, 0xCB, 0xEB, 0xCB, 0xEC, 0xCB, 0xED, 0xCB, 0xEE, 0xCB, 0xEF,
0xCB, 0xF0, 0xCB, 0xF1, 0xCB, 0xF2, 0xCB, 0xF3, 0xCB, 0xF4, 0xCB, 0xF5, 0xCB, 0xF6, 0xCB, 0xF7, 0xCB, 0xF8, 0xCB, 0xF9, 0xCB, 0xFA, 0xCB, 0xFB, 0xCB, 0xFC, 0xCB, 0xFD, 0xCB, 0xFE, 0xCB, 0xFF
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

uint8_t* opcode = NULL;
uint8_t* cb_opcode = NULL;
int cpu_cycles_waited = 0; // for emulating semi-accurate instruction timing
void clock_cpu() {
    // TODO: CPU functionality

    // first, execute the previously fetched instruction
    if (opcode != NULL) {
        // instruction has been fetched
        if(NOP(*opcode)) {
            TraceLog(LOG_INFO, "NOP", *opcode);
            opcode = NULL;
        } else if(HALT(*opcode)) {
            TraceLog(LOG_INFO, "HALT", *opcode);
            opcode = NULL;
        } else if(LD_R_HLA(*opcode)) {
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
        } else if(ADD_HL(*opcode)) {
            TraceLog(LOG_INFO, "Add Register A with Register HL indirect", *opcode);
            opcode = NULL;
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
            uint8_t carry = f_carry ? 1 : 0;
            TraceLog(LOG_INFO, "Add Register A with Register HL indirect and Carry Flag %d", carry);
            opcode = NULL;
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
            opcode = NULL;
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
            opcode = NULL;
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
            opcode = NULL;
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
            TraceLog(LOG_INFO, "INC_HL", *opcode);
            opcode = NULL;
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
            TraceLog(LOG_INFO, "DEC_HL", *opcode);
            opcode = NULL;
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
            TraceLog(LOG_INFO, "AND_HL", *opcode);
            opcode = NULL;
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
            TraceLog(LOG_INFO, "OR_HL", *opcode);
            opcode = NULL;
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
            TraceLog(LOG_INFO, "XOR_HL", *opcode);
            opcode = NULL;
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
                TraceLog(LOG_INFO, "SET_HL", *opcode);
                opcode = NULL; cb_opcode = NULL;
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
                opcode = NULL; cb_opcode = NULL;
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
                opcode = NULL; cb_opcode = NULL;
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
                opcode = NULL; cb_opcode = NULL;
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
                opcode = NULL; cb_opcode = NULL;
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
                opcode = NULL; cb_opcode = NULL;
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
                opcode = NULL; cb_opcode = NULL;
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
                opcode = NULL; cb_opcode = NULL;
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
                opcode = NULL; cb_opcode = NULL;
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
                opcode = NULL; cb_opcode = NULL;
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
                opcode = NULL; cb_opcode = NULL;
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
            TraceLog(LOG_INFO, "JP_II", *opcode);
            opcode = NULL;
        } else if(JP_HL(*opcode)) {
            TraceLog(LOG_INFO, "JP_HL", *opcode);
            opcode = NULL;
        } else if(JPC(*opcode)) {
            TraceLog(LOG_INFO, "JPC", *opcode);
            opcode = NULL;
        } else if(JR(*opcode)) {
            TraceLog(LOG_INFO, "JR", *opcode);
            opcode = NULL;
        } else if(JRC(*opcode)) {
            TraceLog(LOG_INFO, "JRC", *opcode);
            opcode = NULL;
        } else if(CALL(*opcode)) {
            TraceLog(LOG_INFO, "CALL", *opcode);
            opcode = NULL;
        } else if(CALLC(*opcode)) {
            TraceLog(LOG_INFO, "CALLC", *opcode);
            opcode = NULL;
        } else if(RET(*opcode)) {
            TraceLog(LOG_INFO, "RET", *opcode);
            opcode = NULL;
        } else if(RETC(*opcode)) {
            TraceLog(LOG_INFO, "RETC", *opcode);
            opcode = NULL;
        } else if(RETI(*opcode)) {
            TraceLog(LOG_INFO, "RETI", *opcode);
            opcode = NULL;
        } else if(RST(*opcode)) {
            TraceLog(LOG_INFO, "RST", *opcode);
            opcode = NULL;
        } else if(STOP(*opcode)) {
            TraceLog(LOG_INFO, "STOP", *opcode);
            opcode = NULL;
        } else if(DI(*opcode)) {
            TraceLog(LOG_INFO, "DI", *opcode);
            opcode = NULL;
        } else if(EI(*opcode)) {
            TraceLog(LOG_INFO, "EI", *opcode);
            opcode = NULL;
        } else {
            // undefined opcode
            TraceLog(LOG_INFO, "BAD OPCODE???");
            opcode = NULL;
        }
        write_flags(); // update flag register
    }
    // then, if execution is finished, fetch the next instruction
    if (opcode == NULL) {
        // previous execution is done
        opcode = fetch_inst();
    }
}
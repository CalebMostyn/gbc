#include "cpu.h"
#include "instructions.h"
#include <stdint.h>
#include <stddef.h>
#include "raylib.h"

register_file rf;
bool f_zero, f_sub, f_carry, f_hcarry = false;

uint8_t inst_mem[] = {
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
0xC6, 0xFF, 0xC6, 0xFF, 0xCE, 0x01
};

// fetches a single word from instruction memory
// returns a pointer to word and increments PC
uint8_t* fetch_inst() {
    return &inst_mem[rf.PC++ % 500]; // % 500 is a placeholder
}

// Writes flags to register F
//    ┌-> Carry
//  ┌-+> Subtraction
//  | |
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

#ifdef _DEBUG
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
#endif

            // compute addition and set register A to result
            uint16_t result = num1 + num2;
            rf.AF.l = result & 0xFF;

            // compute flags
            f_zero = (result == 0);
            f_sub = false;
            f_hcarry = ((num1 & 0xF) + (num2 & 0xF)) > 0xF;
            f_carry = result > 0xFF;

#ifdef _DEBUG
            TraceLog(LOG_INFO, "%d + %d = %d", num1, num2, result & 0xFF);
#endif

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
#ifdef _DEBUG
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
#endif

            // compute addition and set register A to result
            uint16_t result = num1 + num2 + carry;
            rf.AF.l = result & 0xFF;

            // compute flags
            f_zero = (result == 0);
            f_sub = false;
            f_hcarry = ((num1 & 0xF) + (num2 & 0xF)) > 0xF;
            f_carry = result > 0xFF;

#ifdef _DEBUG
            TraceLog(LOG_INFO, "%d + %d + %d = %d", num1, num2, carry, result & 0xFF);
#endif

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
                f_hcarry = ((rf.AF.l & 0xF) + (n & 0xF)) > 0xF;
                f_carry = result > 0xFF;

            }
            if (++cpu_cycles_waited >= ADDCI_CYCLES) {
                opcode = NULL;
                cpu_cycles_waited = 0;
            }
        } else if(SUB_HL(*opcode)) {
            TraceLog(LOG_INFO, "SUB_HL", *opcode);
            opcode = NULL;
        } else if(SUB(*opcode)) {
            TraceLog(LOG_INFO, "SUB", *opcode);
            opcode = NULL;
        } else if(SUBI(*opcode)) {
            TraceLog(LOG_INFO, "SUBI", *opcode);
            opcode = NULL;
        } else if(SBC_HL(*opcode)) {
            TraceLog(LOG_INFO, "SBC_HL", *opcode);
            opcode = NULL;
        } else if(SBC(*opcode)) {
            TraceLog(LOG_INFO, "SBC", *opcode);
            opcode = NULL;
        } else if(SBCI(*opcode)) {
            TraceLog(LOG_INFO, "SBCI", *opcode);
            opcode = NULL;
        } else if(CP_HL(*opcode)) {
            TraceLog(LOG_INFO, "CP_HL", *opcode);
            opcode = NULL;
        } else if(CP(*opcode)) {
            TraceLog(LOG_INFO, "CP", *opcode);
            opcode = NULL;
        } else if(CPI(*opcode)) {
            TraceLog(LOG_INFO, "CPI", *opcode);
            opcode = NULL;
        } else if(INC_HL(*opcode)) {
            TraceLog(LOG_INFO, "INC_HL", *opcode);
            opcode = NULL;
        } else if(INC(*opcode)) {
            TraceLog(LOG_INFO, "INC", *opcode);
            opcode = NULL;
        } else if(DEC_HL(*opcode)) {
            TraceLog(LOG_INFO, "DEC_HL", *opcode);
            opcode = NULL;
        } else if(DEC(*opcode)) {
            TraceLog(LOG_INFO, "DEC", *opcode);
            opcode = NULL;
        } else if(AND_HL(*opcode)) {
            TraceLog(LOG_INFO, "AND_HL", *opcode);
            opcode = NULL;
        } else if(AND(*opcode)) {
            TraceLog(LOG_INFO, "AND", *opcode);
            opcode = NULL;
        } else if(ANDI(*opcode)) {
            TraceLog(LOG_INFO, "ANDI", *opcode);
            opcode = NULL;
        } else if(OR_HL(*opcode)) {
            TraceLog(LOG_INFO, "OR_HL", *opcode);
            opcode = NULL;
        } else if(OR(*opcode)) {
            TraceLog(LOG_INFO, "OR", *opcode);
            opcode = NULL;
        } else if(ORI(*opcode)) {
            TraceLog(LOG_INFO, "ORI", *opcode);
            opcode = NULL;
        } else if(XOR_HL(*opcode)) {
            TraceLog(LOG_INFO, "XOR_HL", *opcode);
            opcode = NULL;
        } else if(XOR(*opcode)) {
            TraceLog(LOG_INFO, "XOR", *opcode);
            opcode = NULL;
        } else if(XORI(*opcode)) {
            TraceLog(LOG_INFO, "XORI", *opcode);
            opcode = NULL;
        } else if(CCF(*opcode)) {
            TraceLog(LOG_INFO, "CCF", *opcode);
            opcode = NULL;
        } else if(SCF(*opcode)) {
            TraceLog(LOG_INFO, "SCF", *opcode);
            opcode = NULL;
        } else if(DAA(*opcode)) {
            TraceLog(LOG_INFO, "DAA", *opcode);
            opcode = NULL;
        } else if(CPL(*opcode)) {
            TraceLog(LOG_INFO, "CPL", *opcode);
            opcode = NULL;
        } else if(INC_RP(*opcode)) {
            TraceLog(LOG_INFO, "INC_RP", *opcode);
            opcode = NULL;
        } else if(DEC_RP(*opcode)) {
            TraceLog(LOG_INFO, "DEC_RP", *opcode);
            opcode = NULL;
        } else if(ADD_HL_RP(*opcode)) {
            TraceLog(LOG_INFO, "ADD_HL_RP", *opcode);
            opcode = NULL;
        } else if(ADD_SPE(*opcode)) {
            TraceLog(LOG_INFO, "ADD_SPE", *opcode);
            opcode = NULL;
        } else if(RLCA(*opcode)) {
            TraceLog(LOG_INFO, "RLCA", *opcode);
            opcode = NULL;
        } else if(RRCA(*opcode)) {
            TraceLog(LOG_INFO, "RRCA", *opcode);
            opcode = NULL;
        } else if(RLA(*opcode)) {
            TraceLog(LOG_INFO, "RLA", *opcode);
            opcode = NULL;
        } else if(RRA(*opcode)) {
            TraceLog(LOG_INFO, "RRA", *opcode);
            opcode = NULL;
        } else if (*opcode == 0xCB) {
            // CB Prefixed instuction, fetch next byte for "actual" opcode
            opcode = fetch_inst();

            if(SET_HL(*opcode)) {
                TraceLog(LOG_INFO, "SET_HL", *opcode);
                opcode = NULL;
            } else if(SET(*opcode)) {
                TraceLog(LOG_INFO, "SET", *opcode);
                opcode = NULL;
            } else if(RLC(*opcode)) {
                TraceLog(LOG_INFO, "RLC", *opcode);
                opcode = NULL;
            } else if(RLC_HL(*opcode)) {
                TraceLog(LOG_INFO, "RLC_HL", *opcode);
                opcode = NULL;
            } else if(RRC(*opcode)) {
                TraceLog(LOG_INFO, "RRC", *opcode);
                opcode = NULL;
            } else if(RRC_HL(*opcode)) {
                TraceLog(LOG_INFO, "RRC_HL", *opcode);
                opcode = NULL;
            } else if(RL(*opcode)) {
                TraceLog(LOG_INFO, "RL", *opcode);
                opcode = NULL;
            } else if(RL_HL(*opcode)) {
                TraceLog(LOG_INFO, "RL_HL", *opcode);
                opcode = NULL;
            } else if(RR(*opcode)) {
                TraceLog(LOG_INFO, "RR", *opcode);
                opcode = NULL;
            } else if(RR_HL(*opcode)) {
                TraceLog(LOG_INFO, "RR_HL", *opcode);
                opcode = NULL;
            } else if(SLA(*opcode)) {
                TraceLog(LOG_INFO, "SLA", *opcode);
                opcode = NULL;
            } else if(SLA_HL(*opcode)) {
                TraceLog(LOG_INFO, "SLA_HL", *opcode);
                opcode = NULL;
            } else if(SRA(*opcode)) {
                TraceLog(LOG_INFO, "SRA", *opcode);
                opcode = NULL;
            } else if(SRA_HL(*opcode)) {
                TraceLog(LOG_INFO, "SRA_HL", *opcode);
                opcode = NULL;
            } else if(SWAP(*opcode)) {
                TraceLog(LOG_INFO, "SWAP", *opcode);
                opcode = NULL;
            } else if(SWAP_HL(*opcode)) {
                TraceLog(LOG_INFO, "SWAP_HL", *opcode);
                opcode = NULL;
            } else if(SRL(*opcode)) {
                TraceLog(LOG_INFO, "SRL", *opcode);
                opcode = NULL;
            } else if(SRL_HL(*opcode)) {
                TraceLog(LOG_INFO, "SRL_HL", *opcode);
                opcode = NULL;
            } else if(BIT(*opcode)) {
                TraceLog(LOG_INFO, "BIT", *opcode);
                opcode = NULL;
            } else if(BIT_HL(*opcode)) {
                TraceLog(LOG_INFO, "BIT_HL", *opcode);
                opcode = NULL;
            } else if(RES(*opcode)) {
                TraceLog(LOG_INFO, "RES", *opcode);
                opcode = NULL;
            } else if(RES_HL(*opcode)) {
                TraceLog(LOG_INFO, "RES_HL", *opcode);
                opcode = NULL;
            } else {
                // undefined opcode
                TraceLog(LOG_INFO, "BAD OPCODE???");
                opcode = NULL;
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
    }
    write_flags(); // update flag register
    // then, if execution is finished, fetch the next instruction
    if (opcode == NULL) {
        // previous execution is done
        opcode = fetch_inst();
    }
}
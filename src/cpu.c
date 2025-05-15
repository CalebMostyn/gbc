#include "cpu.h"
#include "instructions.h"
#include <stdint.h>
#include <stddef.h>
#include "raylib.h"

register_file rf;

uint8_t inst_mem[] = {
    // // 8-bit load
    0x40, // LD_R_R (LD B,B)
    0x06, // LD_R_I (LD B,d8)
    0x46, // LD_R_HLA (LD B,(HL))
    0x70, // LD_HLA_R (LD (HL),B)
    0x36, // LD_HLA_I (LD (HL),d8)
    0x0A, // LD_A_BCA
    0x1A, // LD_A_DEA
    0x02, // LD_BCA_A
    0x12, // LD_DEA_A
    0xFA, // LD_A_IIA
    0xEA, // LD_IIA_A
    0xF2, // LD_A_CA
    0xE2, // LD_CA_A
    0xF0, // LD_A_IA
    0xE0, // LD_IA_A
    0x3A, // LD_A_HLA_DEC
    0x32, // LD_HLA_A_DEC
    0x2A, // LD_A_HLA_INC
    0x22, // LD_HLA_A_INC

    // 16-bit load
    0x01, // LD_RP_II (LD BC,d16)
    0x08, // LD_IIA_SP
    0xF9, // LD_SP_HL
    0xC5, // PUSH (PUSH BC)
    0xC1, // POP (POP BC)
    0xF8, // LD_HL_SPE

    // 8-bit arithmetic
    0x80, // ADD A,B
    0x86, // ADD_HL (ADD A,(HL))
    0xC6, // ADDI
    0x88, // ADC A,B
    0x8E, // ADC_HL
    0xCE, // ADCI
    0x90, // SUB B
    0x96, // SUB_HL
    0xD6, // SUBI
    0x98, // SBC A,B
    0x9E, // SBC_HL 
    0xDE, // SBCI
    0xB8, // CP B
    0xBE, // CP HL
    0xFE, // CPI
    0x04, // INC B
    0x34, // INC_HL
    0x05, // DEC B
    0x35, // DEC_HL
    0xA0, // AND B
    0xA6, // AND_HL
    0xE6, // ANDI
    0xB0, // OR B
    0xB6, // OR_HL
    0xF6, // ORI
    0xA8, // XOR B
    0xAE, // XOR_HL
    0xEE, // XORI
    0x3F, // CCF
    0x37, // SCF
    0x27, // DAA
    0x2F, // CPL

    // 16-bit arithmetic
    0x03, // INC_RP (INC BC)
    0x0B, // DEC_RP (DEC BC)
    0x09, // ADD_HL_RP (ADD HL,BC)
    0xE8, // ADD_SPE

    // rotate, shift, bit
    0x07, // RLCA
    0x0F, // RRCA
    0x17, // RLA
    0x1F, // RRA

    // CB-prefixed instructions
    0xCB, 0x00, // RLC B
    0xCB, 0x08, // RRC B
    0xCB, 0x10, // RL B
    0xCB, 0x18, // RR B
    0xCB, 0x20, // SLA B
    0xCB, 0x28, // SRA B
    0xCB, 0x30, // SWAP B
    0xCB, 0x38, // SRL B
    0xCB, 0x40, // BIT 0,B
    0xCB, 0x80, // RES 0,B
    0xCB, 0xC0, // SET 0,B

    // control flow
    0xC3, // JP_II
    0xE9, // JP_HL
    0xC2, // JPC (JP NZ,nn)
    0x18, // JR
    0x20, // JRC (JR NZ,r8)
    0xCD, // CALL
    0xC4, // CALLC (CALL NZ,nn)
    0xC9, // RET
    0xC0, // RETC (RET NZ)
    0xD9, // RETI
    0xC7, // RST 00h

    // misc
    0x76, // HALT
    0x10, // STOP
    0xF3, // DI
    0xFB, // EI
    0x00  // NOP
};

// fetches a single word from instruction memory
// returns a pointer to word and increments PC
uint8_t* fetch_inst() {
    return &inst_mem[rf.PC++ % 500]; // % 500 is a placeholder
}

uint8_t* opcode = NULL;
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
            TraceLog(LOG_INFO, "ADD_HL", *opcode);
            opcode = NULL;
        } else if(ADD(*opcode)) {
            TraceLog(LOG_INFO, "ADD", *opcode);
            opcode = NULL;
        } else if(ADDI(*opcode)) {
            TraceLog(LOG_INFO, "ADDI", *opcode);
            opcode = NULL;
        } else if(ADC_HL(*opcode)) {
            TraceLog(LOG_INFO, "ADC_HL", *opcode);
            opcode = NULL;
        } else if(ADC(*opcode)) {
            TraceLog(LOG_INFO, "ADC", *opcode);
            opcode = NULL;
        } else if(ADCI(*opcode)) {
            TraceLog(LOG_INFO, "ADCI", *opcode);
            opcode = NULL;
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
    // then, if execution is finished, fetch the next instruction
    if (opcode == NULL) {
        // previous execution is done
        opcode = fetch_inst();
    }
}
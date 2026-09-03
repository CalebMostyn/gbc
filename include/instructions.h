#ifndef INSTRUCTION_H
#define INSTRUCTION_H

// 8-bit load
// Loads register from other register
// 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x47,
// 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4F,
// 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x57,
// 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5F,
// 0x60, 0x61, 0x62, 0x63, 0x64, 0x65,  0x67,
// 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D,  0x6F,
// 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7F
#define LD_R_R(x) ((x&(0xC0))==(0x40))
// Loads register from immediate
// 0x06, 0x0E, 0x16, 0x1E, 0x26, 0x2E, 0x3E
#define LD_R_I(x) ((x&(0xC7))==(0x06))
#define LD_R_I_CYCLES 2
// Loads register from mem address in HL register
// 0x46, 0x4E, 0x56, 0x5E, 0x66, 0x6E, 0x7E
#define LD_R_HLA(x) ((x&(0xC7))==(0x46))
#define LD_R_HLA_CYCLES 2
// Loads mem address in HL register from register
// 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x77
#define LD_HLA_R(x) ((x&(0xF8))==(0x70))
#define LD_HLA_R_CYCLES 2
// Loads mem address in HL register from immediate
#define LD_HLA_I(x) (x==(0x36))
#define LD_HLA_I_CYCLES 3
// Loads register A from mem address in BC register
#define LD_A_BCA(x) (x==(0x0A))
#define LD_A_BCA_CYCLES 2
// Loads register A from mem address in DE register
#define LD_A_DEA(x) (x==(0x1A))
#define LD_A_DEA_CYCLES 2
// Loads mem address in BC register from register A
#define LD_BCA_A(x) (x==(0x02))
#define LD_BCA_A_CYCLES 2
// Loads mem address in DE register from register A
#define LD_DEA_A(x) (x==(0x12))
#define LD_DEA_A_CYCLES 2
// Loads register A from mem address in 2 immediate bytes
#define LD_A_IIA(x) (x==(0xFA))
#define LD_A_IIA_CYCLES 4
// Loads mem address in 2 immediate bytes from register A
#define LD_IIA_A(x) (x==(0xEA))
#define LD_IIA_A_CYCLES 4
// Loads register A from mem address at 0xFF00 + register C
#define LD_A_CA(x) (x==(0xF2))
#define LD_A_CA_CYCLES 2
// Loads mem address at 0xFF00 + register C from register A
#define LD_CA_A(x) (x==(0xE2))
#define LD_CA_A_CYCLES 2
// Loads register A from mem address at 0xFF00 + immediate
#define LD_A_IA(x) (x==(0xF0))
#define LD_A_IA_CYCLES 3
// Loads mem address 0xFF00 + immediate from register A
#define LD_IA_A(x) (x==(0xE0))
#define LD_IA_A_CYCLES 3
// Loads register A from mem address in HL register and decrement HL
#define LD_A_HLA_DEC(x) (x==(0x3A))
#define LD_A_HLA_DEC_CYCLES 2
// Loads mem address in HL register from register A and decrement HL
#define LD_HLA_A_DEC(x) (x==(0x32))
#define LD_HLA_A_DEC_CYCLES 2
// Loads register A from mem address in HL register and increment HL
#define LD_A_HLA_INC(x) (x==(0x2A))
#define LD_A_HLA_INC_CYCLES 2
// Loads mem address in HL register from register A and increment HL
#define LD_HLA_A_INC(x) (x==(0x22))
#define LD_HLA_A_INC_CYCLES 2

//16-bit load
// Loads 16-bit register from two immediate bytes
// 0x01, 0x11, 0x21, 0x31
#define LD_RP_II(x) ((x&(0xCF))==(0x01))
#define LD_RP_II_CYCLES 3
// Loads mem address at immediate bytes from SP
#define LD_IIA_SP(x) (x==(0x08))
#define LD_IIA_SP_CYCLES 5
// Loads SP from register HL
#define LD_SP_HL(x) (x==(0xF9))
#define LD_SP_HL_CYCLES 2
// Push to stack from register pair
// 0xC5, 0xD5, 0xE5, 0xF5
#define PUSH(x) ((x&(0xCF))==(0xC5))
#define PUSH_CYCLES 4
// Pop from stack to register pair
// 0xC1, 0xD1, 0xE1, 0xF1
#define POP(x) ((x&(0xCF))==(0xC1))
#define POP_CYCLES 3
// Load HL from SP + immediate byte
#define LD_HL_SPE(x) (x==(0xF8))
#define LD_HL_SPE_CYCLES 3

// 8-bit arithmetic
#define ADD(x) ((x&(0xF8))==(0x80))
#define ADD_HL(x) (x==(0x86))
#define ADD_HL_CYCLES 2
#define ADDI(x) (x==(0xC6))
#define ADDI_CYCLES 2
#define ADC(x) ((x&(0xF8))==(0x88))
#define ADC_HL(x) (x==(0x8E))
#define ADC_HL_CYCLES 2
#define ADCI(x) (x==(0xCE))
#define ADDCI_CYCLES 2
#define SUB(x) ((x&(0xF8))==(0x90))
#define SUB_HL(x) (x==(0x96))
#define SUB_HL_CYCLES 2
#define SUBI(x) (x==(0xD6))
#define SUBI_CYCLES 2
#define SBC(x) ((x&(0xF8))==(0x98))
#define SBC_HL(x) (x==(0x9E))
#define SBC_HL_CYCLES 2
#define SBCI(x) (x==(0xDE))
#define CP(x) ((x&(0xF8))==(0xB8))
#define CP_HL(x) (x==(0xBE))
#define CP_HL_CYCLES 2
#define CPI(x) (x==(0xFE))
#define CPI_CYCLES 2
#define INC(x) ((x&(0xC7))==(0x04))
#define INC_HL(x) (x==(0x34))
#define INC_HL_CYCLES 3
#define DEC(x) ((x&(0xC7))==(0x05))
#define DEC_HL(x) (x==(0x35))
#define DEC_HL_CYCLES 3
#define AND(x) ((x&(0xF8))==(0xA0))
#define AND_HL(x) (x==(0xA6))
#define AND_HL_CYCLES 2
#define ANDI(x) (x==(0xE6))
#define ANDI_CYCLES 2
#define OR(x) ((x&(0xF8))==(0xB0))
#define OR_HL(x) (x==(0xB6))
#define OR_HL_CYCLES 2
#define ORI(x) (x==(0xF6))
#define ORI_CYCLES 2
#define XOR(x) ((x&(0xF8))==(0xA8))
#define XOR_HL(x) (x==(0xAE))
#define XOR_HL_CYCLES 2
#define XORI(x) (x==(0xEE))
#define XORI_CYCLES 2
#define CCF(x) (x==(0x3F))
#define SCF(x) (x==(0x37))
#define DAA(x) (x==(0x27))
#define CPL(x) (x==(0x2F))

// 16-bit arithmetic
#define INC_RP(x) ((x&(0xCF))==(0x03))
#define INC_RP_CYCLES 2
#define DEC_RP(x) ((x&(0xCF))==(0x0B))
#define DEC_RP_CYCLES 2
#define ADD_HL_RP(x) ((x&(0xCF))==(0x09))
#define ADD_HL_RP_CYCLES 2
#define ADD_SPE(x) (x==(0xE8))
#define ADD_SPE_CYCLES 4

// rotate, shift, bit
#define RLCA(x) (x==(0x07))
#define RRCA(x) (x==(0x0F))
#define RLA(x) (x==(0x17))
#define RRA(x) (x==(0x1F))
#define RLC(x) ((x&(0xF8))==(0x00))
#define RLC_CYCLES 2
#define RLC_HL(x) (x==(0x06))
#define RLC_HL_CYCLES 4
#define RRC(x) ((x&(0xF8))==(0x08))
#define RRC_CYCLES 2
#define RRC_HL(x) (x==(0x0E))
#define RRC_HL_CYCLES 4
#define RL(x) ((x&(0xF8))==(0x10))
#define RL_CYCLES 2
#define RL_HL(x) (x==(0x16))
#define RL_HL_CYCLES 4
#define RR(x) ((x&(0xF8))==(0x18))
#define RR_CYCLES 2
#define RR_HL(x) (x==(0x1E))
#define RR_HL_CYCLES 4
#define SLA(x) ((x&(0xF8))==(0x20))
#define SLA_CYCLES 2
#define SLA_HL(x) (x==(0x26))
#define SLA_HL_CYCLES 4
#define SRA(x) ((x&(0xF8))==(0x28))
#define SRA_CYCLES 2
#define SRA_HL(x) (x==(0x2E))
#define SRA_HL_CYCLES 4
#define SWAP(x) ((x&(0xF8))==(0x30))
#define SWAP_CYCLES 2
#define SWAP_HL(x) (x==(0x36))
#define SWAP_HL_CYCLES 4
#define SRL(x) ((x&(0xF8))==(0x38))
#define SRL_CYCLES 2
#define SRL_HL(x) (x==(0x3E))
#define SRL_HL_CYCLES 4
#define BIT(x) ((x&(0xC0))==(0x40))
#define BIT_CYCLES 2
#define BIT_HL(x) ((x&(0xC7))==(0x46))
#define BIT_HL_CYCLES 3
#define RES(x) ((x&(0xC0))==(0x80))
#define RES_CYCLES 2
#define RES_HL(x) ((x&(0xC7))==(0x86))
#define RES_HL_CYCLES 4
#define SET(x) ((x&(0xC0))==(0xC0))
#define SET_CYCLES 2
#define SET_HL(x) ((x&(0xC7))==(0xC6))
#define SET_HL_CYCLES 4

// control flow
#define JP_II(x) (x==(0xC3))
#define JP_II_CYCLES 4
#define JP_HL(x) (x==(0xE9))
#define JPC(x) ((x&(0xE7))==(0xC2))
#define JPC_TRUE_CYCLES 4
#define JPC_FALSE_CYCLES 3
#define JR(x) (x==(0x18))
#define JR_CYCLES 3
#define JRC(x) ((x&(0xE7))==(0x20))
#define JRC_TRUE_CYCLES 3
#define JRC_FALSE_CYCLES 2
#define CALL(x) (x==(0xCD))
#define CALL_CYCLES 6
#define CALLC(x) ((x&(0xE7))==(0xC4))
#define CALLC_TRUE_CYCLES 6
#define CALLC_FALSE_CYCLES 3
#define RET(x) (x==(0xC9))
#define RET_CYCLES 4
#define RETC(x) ((x&(0xE7))==(0xC0))
#define RETC_TRUE_CYCLES 5
#define RETC_FALSE_CYCLES 2
#define RETI(x) (x==(0xD9))
#define RETI_CYCLES 4
#define RST(x) ((x&(0xC7))==(0xC7))
#define RST_CYCLES 4

// misc
#define HALT(x) (x==(0x76))
#define STOP(x) (x==(0x10))
#define DI(x) (x==(0xF3))
#define EI(x) (x==(0xFB))
#define NOP(x) (x==(0x00))

#endif // INSTRUCTIONS_H

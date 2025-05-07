#ifndef INSTRUCTION_H
#define INSTRUCTION_H

// 8-bit load
#define LD_R_R(x) ((x&(0xC0))==(0x40))
#define LD_R_I(x) ((x&(0xC7))==(0x06))
#define LD_R_HLA(x) ((x&(0xC7))==(0x46))
#define LD_HLA_R(x) ((x&(0xF8))==(0x70))
#define LD_HLA_I(x) (x==(0x36))
#define LD_A_BCA(x) (x==(0x0A))
#define LD_A_DEA(x) (x==(0x1A))
#define LD_BCA_A(x) (x==(0x02))
#define LD_DEA_A(x) (x==(0x12))
#define LD_A_IIA(x) (x==(0xFA))
#define LD_IIA_A(x) (x==(0xEA))
#define LD_A_CA(x) (x==(0xF2))
#define LD_CA_A(x) (x==(0xE2))
#define LD_A_IA(x) (x==(0xF0))
#define LD_IA_A(x) (x==(0xE0))
#define LD_A_HLA_DEC(x) (x==(0x3A))
#define LD_HLA_A_DEC(x) (x==(0x32))
#define LD_A_HLA_INC(x) (x==(0x2A))
#define LD_HLA_A_INC(x) (x==(0x22))

//16-bit load
#define LD_RP_II(x) ((x&(0xCF))==(0x01))
#define LD_IIA_SP(x) (x==(0x08))
#define LD_SP_HL(x) (x==(0xF9))
#define PUSH(x) ((x&(0xCF))==(0xC5))
#define POP(x) ((x&(0xCF))==(0xC1))
#define LD_HL_SPE(x) (x==(0xF8))

#endif // INSTRUCTIONS_H

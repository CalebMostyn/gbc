#include "munit.h"
#include "helpers.h"
#include "cpu.h"

static MunitResult test_and_register() {
    // Bitwise AND register with register A and store in register A
    // 0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA7
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA7};
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    // A & B
    rf.BC.l = 0x42;
    rf.AF.l = 0xFF;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 2);
    munit_assert_int(rf.AF.l, ==, 0x42);
    // Half carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00100000);
    // A & C
    rf.BC.r = 0x43;
    rf.AF.l = 0xFF;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(rf.AF.l, ==, 0x43);
    // Half carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00100000);
    // A & D
    rf.DE.l = 0x44;
    rf.AF.l = 0xFF;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 4);
    munit_assert_int(rf.AF.l, ==, 0x44);
    // Half carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00100000);
    // A & E
    rf.DE.r = 0x45;
    rf.AF.l = 0xFF;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(rf.AF.l, ==, 0x45);
    // Half carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00100000);
    // A & H
    rf.HL.l = 0x46;
    rf.AF.l = 0xFF;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 6);
    munit_assert_int(rf.AF.l, ==, 0x46);
    // Half carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00100000);
    // A & L
    rf.HL.r = 0x47;
    rf.AF.l = 0xFF;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 7);
    munit_assert_int(rf.AF.l, ==, 0x47);
    // Half carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00100000);
    // A & A
    rf.AF.l = 0x48;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 8);
    munit_assert_int(rf.AF.l, ==, 0x48);
    // Half carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00100000);

    return MUNIT_OK;
}

static MunitResult test_and_flags() {
    // Bitwise AND register with register A and store in register A
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {0xA0};
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);

    rf.BC.l = 0xF0;
    rf.AF.l = 0x0F;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 2);
    munit_assert_int(rf.AF.l, ==, 0x00);
    // Zero and half carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10100000);

    return MUNIT_OK;
}

static MunitResult test_and_with_hl_indirect() {
    // Bitwise AND value from mem address in HL register
    // with register A and store in register A, 0xA6
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {0xA6};
    memcpy(memory, instructions, sizeof(instructions));

    // load test value in memory, set HL to test memory addr
    memory[0xBEEF] = 0x42;
    rf.HL.lr = 0xBEEF;
    rf.AF.l = 0xFF;
    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 2);
    munit_assert_int(rf.AF.l, ==, 0x42);
    // Half carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00100000);

    return MUNIT_OK;
}

static MunitResult test_and_immediate() {
    // Bitwise AND immediate with register A
    // and store in register A, 0xE6
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {0xE6, 0x42};
    memcpy(memory, instructions, sizeof(instructions));

    // load test value in memory, set HL to test memory addr
    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    rf.AF.l = 0xFF;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3); // PC += 2 from immediate
    munit_assert_int(rf.AF.l, ==, 0x42);
    // Half carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00100000);

    return MUNIT_OK;
}

static MunitResult test_or_register() {
    // Bitwise OR register with register A and store in register A
    // 0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB7
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB7};
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    // A | B
    rf.BC.l = 0x40;
    rf.AF.l = 0x02;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 2);
    munit_assert_int(rf.AF.l, ==, 0x42);
    // A | C
    rf.BC.r = 0x40;
    rf.AF.l = 0x03;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(rf.AF.l, ==, 0x43);
    // A | D
    rf.DE.l = 0x40;
    rf.AF.l = 0x04;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 4);
    munit_assert_int(rf.AF.l, ==, 0x44);
    // A | E
    rf.DE.r = 0x40;
    rf.AF.l = 0x05;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(rf.AF.l, ==, 0x45);
    // A | H
    rf.HL.l = 0x40;
    rf.AF.l = 0x06;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 6);
    munit_assert_int(rf.AF.l, ==, 0x46);
    // A | L
    rf.HL.r = 0x40;
    rf.AF.l = 0x07;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 7);
    munit_assert_int(rf.AF.l, ==, 0x47);
    // A | A
    rf.AF.l = 0x48;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 8);
    munit_assert_int(rf.AF.l, ==, 0x48);

    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    return MUNIT_OK;
}

static MunitResult test_or_flags() {
    // Bitwise OR register with register A and store in register A
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {0xB0};
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    // OR 0 with 0
    clock_cpu();
    munit_assert_int(rf.PC, ==, 2);
    munit_assert_int(rf.AF.l, ==, 0x00);
    // Zero flag set
    munit_assert_int(rf.AF.r, ==, 0b10000000);

    return MUNIT_OK;
}

static MunitResult test_or_with_hl_indirect() {
    // Bitwise OR value from mem address in HL register
    // with register A and store in register A, 0xB6
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {0xB6};
    memcpy(memory, instructions, sizeof(instructions));

    // load test value in memory, set HL to test memory addr
    memory[0xBEEF] = 0x40;
    rf.HL.lr = 0xBEEF;
    rf.AF.l = 0x02;
    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 2);
    munit_assert_int(rf.AF.l, ==, 0x42);

    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    return MUNIT_OK;
}

static MunitResult test_or_immediate() {
    // Bitwise OR immediate with register A
    // and store in register A, 0xF6
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {0xF6, 0x40};
    memcpy(memory, instructions, sizeof(instructions));

    // load test value in memory, set HL to test memory addr
    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    rf.AF.l = 0x02;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3); // PC += 2 from immediate
    munit_assert_int(rf.AF.l, ==, 0x42);

    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    return MUNIT_OK;
}

static MunitResult test_xor_register() {
    // Bitwise XOR register with register A and store in register A
    // 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAF
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD,0xAF};
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    // A ^ B
    rf.BC.l = 0x60;
    rf.AF.l = 0x22;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 2);
    munit_assert_int(rf.AF.l, ==, 0x42);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    // A ^ C
    rf.BC.r = 0x60;
    rf.AF.l = 0x23;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(rf.AF.l, ==, 0x43);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    // A ^ D
    rf.DE.l = 0x60;
    rf.AF.l = 0x24;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 4);
    munit_assert_int(rf.AF.l, ==, 0x44);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    // A ^ E
    rf.DE.r = 0x60;
    rf.AF.l = 0x25;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(rf.AF.l, ==, 0x45);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    // A ^ H
    rf.HL.l = 0x60;
    rf.AF.l = 0x26;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 6);
    munit_assert_int(rf.AF.l, ==, 0x46);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    // A ^ L
    rf.HL.r = 0x60;
    rf.AF.l = 0x27;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 7);
    munit_assert_int(rf.AF.l, ==, 0x47);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    // A ^ A
    clock_cpu();
    munit_assert_int(rf.PC, ==, 8);
    munit_assert_int(rf.AF.l, ==, 0x00);
    // Zero flag set
    munit_assert_int(rf.AF.r, ==, 0b10000000);

    return MUNIT_OK;
}

static MunitResult test_xor_with_hl_indirect() {
    // Bitwise XOR value from mem address in HL register
    // with register A and store in register A, 0xAE
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {0xAE};
    memcpy(memory, instructions, sizeof(instructions));

    // load test value in memory, set HL to test memory addr
    memory[0xBEEF] = 0x60;
    rf.HL.lr = 0xBEEF;
    rf.AF.l = 0x22;
    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 2);
    munit_assert_int(rf.AF.l, ==, 0x42);

    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    return MUNIT_OK;
}

static MunitResult test_xor_immediate() {
    // Bitwise XOR immediate with register A
    // and store in register A, 0xEE
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {0xEE, 0x60};
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    rf.AF.l = 0x22;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3); // PC += 2 from immediate
    munit_assert_int(rf.AF.l, ==, 0x42);

    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    return MUNIT_OK;
}

static MunitResult test_complement_carry_flag() {
    // NOTs carry flag, 0x3F
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {0x3F, 0x3F};
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    // flip from 0 to 1
    clock_cpu();
    munit_assert_int(rf.PC, ==, 2);
    munit_assert_int(rf.AF.r, ==, 0b00010000);
    // flip from 1 to 0
    clock_cpu();
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(rf.AF.r, ==, 0b00000000);

    return MUNIT_OK;
}

static MunitResult test_set_carry_flag() {
    // Sets carry flag to true, 0x37
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {0x37};
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    clock_cpu();
    munit_assert_int(rf.PC, ==, 2);
    munit_assert_int(rf.AF.r, ==, 0b00010000);

    return MUNIT_OK;
}

static MunitResult test_decimal_adjust_accumulator() {
    // Converts register A to 'binary-coded decimal', 0x27
    // TODO: I still don't understand this instruction
    // and its really complicated, probably needs more tests
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {0x27};
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    rf.AF.l = 0x3C; // decimal 42
    clock_cpu();
    munit_assert_int(rf.PC, ==, 2);
    munit_assert_int(rf.AF.l, ==, 0x42);

    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    return MUNIT_OK;
}

static MunitResult test_complement_accumulator() {
    // Bitwise NOTs register A, 0x2F
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {0x2F};
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    rf.AF.l = 0b10101010; // decimal 42
    clock_cpu();
    munit_assert_int(rf.PC, ==, 2);
    munit_assert_int(rf.AF.l, ==, 0b01010101);

    // Subtraction and half carry flags set
    munit_assert_int(rf.AF.r, ==, 0b01100000);

    return MUNIT_OK;
}

static MunitResult test_bit_test_0_register() {
    // Test if bit 0 in register is 0
    // 0xCB40, 0xCB41, 0xCB42, 0xCB43, 0xCB44, 0xCB45, 0xCB47
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0x40,
        0xCB, 0x40,
        0xCB, 0x41,
        0xCB, 0x41,
        0xCB, 0x42,
        0xCB, 0x42,
        0xCB, 0x43,
        0xCB, 0x43,
        0xCB, 0x44,
        0xCB, 0x44,
        0xCB, 0x45,
        0xCB, 0x45,
        0xCB, 0x47,
        0xCB, 0x47
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    // Test register B, bit 0
    rf.BC.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3);
    // Zero and half carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10100000);
    rf.BC.l = 0b00000001;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 5);
    // Half carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00100000);

    // Test register C, bit 0
    rf.BC.r = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 7);
    // Zero and half carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10100000);
    rf.BC.r = 0b00000001;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 9);
    // Half carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00100000);

    // Test register D, bit 0
    rf.DE.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 11);
    // Zero and half carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10100000);
    rf.DE.l = 0b00000001;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 13);
    // Half carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00100000);

    // Test register E, bit 0
    rf.DE.r = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 15);
    // Zero and half carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10100000);
    rf.DE.r = 0b00000001;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 17);
    // Half carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00100000);

    // Test register H, bit 0
    rf.HL.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 19);
    // Zero and half carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10100000);
    rf.HL.l = 0b00000001;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 21);
    // Half carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00100000);

    // Test register L, bit 0
    rf.HL.r = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 23);
    // Zero and half carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10100000);
    rf.HL.r = 0b00000001;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 25);
    // Half carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00100000);

    // Test register A, bit 0
    rf.AF.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 27);
    // Zero and half carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10100000);
    rf.AF.l = 0b00000001;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 29);
    // Half carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00100000);

    return MUNIT_OK;
}

static MunitResult test_bit_test_0_hl_indirect() {
    // Test if bit 0 in mem value at mem address
    // in register HL is 0, 0xCB46
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0x46,
        0xCB, 0x46,
    };
    memcpy(memory, instructions, sizeof(instructions));

    rf.HL.lr = 0xBEEF;
    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    memory[0xBEEF] = 0b00000000;
    clock_cpu(); clock_cpu(); clock_cpu(); // execute (takes 3 cycles)
    munit_assert_int(rf.PC, ==, 3);
    // Zero and half carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10100000);
    memory[0xBEEF] = 0b00000001;
    clock_cpu(); clock_cpu(); clock_cpu(); // execute (takes 3 cycles)
    munit_assert_int(rf.PC, ==, 5);
    // Half carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00100000);

    return MUNIT_OK;
}

static MunitResult test_bit_test_1_register() {
    // Test if bit 1 in register is 0
    // 0xCB48, 0xCB49, 0xCB4A, 0xCB4B, 0xCB4C, 0xCB4D, 0xCB4F
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0x48,
        0xCB, 0x48,
        0xCB, 0x49,
        0xCB, 0x49,
        0xCB, 0x4a,
        0xCB, 0x4a,
        0xCB, 0x4b,
        0xCB, 0x4b,
        0xCB, 0x4c,
        0xCB, 0x4c,
        0xCB, 0x4d,
        0xCB, 0x4d,
        0xCB, 0x4f,
        0xCB, 0x4f
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    // Test register B, bit 1
    rf.BC.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3);
    // Zero and half carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10100000);
    rf.BC.l = 0b00000010;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 5);
    // Half carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00100000);

    // Test register C, bit 1
    rf.BC.r = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 7);
    // Zero and half carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10100000);
    rf.BC.r = 0b00000010;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 9);
    // Half carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00100000);

    // Test register D, bit 1
    rf.DE.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 11);
    // Zero and half carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10100000);
    rf.DE.l = 0b00000010;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 13);
    // Half carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00100000);

    // Test register E, bit 1
    rf.DE.r = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 15);
    // Zero and half carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10100000);
    rf.DE.r = 0b00000010;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 17);
    // Half carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00100000);

    // Test register H, bit 1
    rf.HL.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 19);
    // Zero and half carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10100000);
    rf.HL.l = 0b00000010;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 21);
    // Half carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00100000);

    // Test register L, bit 1
    rf.HL.r = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 23);
    // Zero and half carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10100000);
    rf.HL.r = 0b00000010;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 25);
    // Half carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00100000);

    // Test register A, bit 1
    rf.AF.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 27);
    // Zero and half carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10100000);
    rf.AF.l = 0b00000010;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 29);
    // Half carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00100000);

    return MUNIT_OK;
}

static MunitResult test_bit_test_1_hl_indirect() {
    // Test if bit 1 in mem value at mem address
    // in register HL is 0, 0xCB4E
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0x4e,
        0xCB, 0x4e,
    };
    memcpy(memory, instructions, sizeof(instructions));

    rf.HL.lr = 0xBEEF;
    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    memory[0xBEEF] = 0b00000000;
    clock_cpu(); clock_cpu(); clock_cpu(); // execute (takes 3 cycles)
    munit_assert_int(rf.PC, ==, 3);
    // Zero and half carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10100000);
    memory[0xBEEF] = 0b00000010;
    clock_cpu(); clock_cpu(); clock_cpu(); // execute (takes 3 cycles)
    munit_assert_int(rf.PC, ==, 5);
    // Half carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00100000);

    return MUNIT_OK;
}

static MunitResult test_bit_test_2_register() {
    // Test if bit 2 in register is 0
    // 0xCB50, 0xCB51, 0xCB52, 0xCB53, 0xCB54, 0xCB55, 0xCB57
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0x50,
        0xCB, 0x50,
        0xCB, 0x51,
        0xCB, 0x51,
        0xCB, 0x52,
        0xCB, 0x52,
        0xCB, 0x53,
        0xCB, 0x53,
        0xCB, 0x54,
        0xCB, 0x54,
        0xCB, 0x55,
        0xCB, 0x55,
        0xCB, 0x57,
        0xCB, 0x57
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    // Test register B, bit 2
    rf.BC.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3);
    // Zero and half carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10100000);
    rf.BC.l = 0b00000100;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 5);
    // Half carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00100000);

    // Test register C, bit 2
    rf.BC.r = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 7);
    // Zero and half carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10100000);
    rf.BC.r = 0b00000100;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 9);
    // Half carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00100000);

    // Test register D, bit 2
    rf.DE.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 11);
    // Zero and half carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10100000);
    rf.DE.l = 0b00000100;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 13);
    // Half carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00100000);

    // Test register E, bit 2
    rf.DE.r = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 15);
    // Zero and half carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10100000);
    rf.DE.r = 0b00000100;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 17);
    // Half carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00100000);

    // Test register H, bit 2
    rf.HL.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 19);
    // Zero and half carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10100000);
    rf.HL.l = 0b00000100;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 21);
    // Half carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00100000);

    // Test register L, bit 2
    rf.HL.r = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 23);
    // Zero and half carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10100000);
    rf.HL.r = 0b00000100;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 25);
    // Half carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00100000);

    // Test register A, bit 2
    rf.AF.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 27);
    // Zero and half carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10100000);
    rf.AF.l = 0b00000100;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 29);
    // Half carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00100000);

    return MUNIT_OK;
}

static MunitResult test_bit_test_2_hl_indirect() {
    // Test if bit 2 in mem value at mem address
    // in register HL is 0, 0xCB56
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0x56,
        0xCB, 0x56,
    };
    memcpy(memory, instructions, sizeof(instructions));

    rf.HL.lr = 0xBEEF;
    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    memory[0xBEEF] = 0b00000000;
    clock_cpu(); clock_cpu(); clock_cpu(); // execute (takes 3 cycles)
    munit_assert_int(rf.PC, ==, 3);
    // Zero and half carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10100000);
    memory[0xBEEF] = 0b00000100;
    clock_cpu(); clock_cpu(); clock_cpu(); // execute (takes 3 cycles)
    munit_assert_int(rf.PC, ==, 5);
    // Half carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00100000);

    return MUNIT_OK;
}

static MunitResult test_bit_test_3_register() {
    // Test if bit 3 in register is 0
    // 0xCB58, 0xCB59, 0xCB5A, 0xCB5B, 0xCB5C, 0xCB5D, 0xCB5F
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0x58,
        0xCB, 0x58,
        0xCB, 0x59,
        0xCB, 0x59,
        0xCB, 0x5a,
        0xCB, 0x5a,
        0xCB, 0x5b,
        0xCB, 0x5b,
        0xCB, 0x5c,
        0xCB, 0x5c,
        0xCB, 0x5d,
        0xCB, 0x5d,
        0xCB, 0x5f,
        0xCB, 0x5f
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    // Test register B, bit 3
    rf.BC.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3);
    // Zero and half carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10100000);
    rf.BC.l = 0b00001000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 5);
    // Half carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00100000);

    // Test register C, bit 3
    rf.BC.r = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 7);
    // Zero and half carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10100000);
    rf.BC.r = 0b00001000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 9);
    // Half carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00100000);

    // Test register D, bit 3
    rf.DE.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 11);
    // Zero and half carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10100000);
    rf.DE.l = 0b00001000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 13);
    // Half carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00100000);

    // Test register E, bit 3
    rf.DE.r = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 15);
    // Zero and half carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10100000);
    rf.DE.r = 0b00001000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 17);
    // Half carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00100000);

    // Test register H, bit 3
    rf.HL.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 19);
    // Zero and half carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10100000);
    rf.HL.l = 0b00001000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 21);
    // Half carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00100000);

    // Test register L, bit 3
    rf.HL.r = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 23);
    // Zero and half carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10100000);
    rf.HL.r = 0b00001000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 25);
    // Half carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00100000);

    // Test register A, bit 3
    rf.AF.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 27);
    // Zero and half carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10100000);
    rf.AF.l = 0b00001000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 29);
    // Half carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00100000);

    return MUNIT_OK;
}

static MunitResult test_bit_test_3_hl_indirect() {
    // Test if bit 3 in mem value at mem address
    // in register HL is 0, 0xCB5E
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0x5e,
        0xCB, 0x5e,
    };
    memcpy(memory, instructions, sizeof(instructions));

    rf.HL.lr = 0xBEEF;
    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    memory[0xBEEF] = 0b00000000;
    clock_cpu(); clock_cpu(); clock_cpu(); // execute (takes 3 cycles)
    munit_assert_int(rf.PC, ==, 3);
    // Zero and half carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10100000);
    memory[0xBEEF] = 0b00001000;
    clock_cpu(); clock_cpu(); clock_cpu(); // execute (takes 3 cycles)
    munit_assert_int(rf.PC, ==, 5);
    // Half carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00100000);

    return MUNIT_OK;
}

static MunitResult test_bit_test_4_register() {
    // Test if bit 4 in register is 0
    // 0xCB60, 0xCB61, 0xCB62, 0xCB63, 0xCB64, 0xCB65, 0xCB67
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0x60,
        0xCB, 0x60,
        0xCB, 0x61,
        0xCB, 0x61,
        0xCB, 0x62,
        0xCB, 0x62,
        0xCB, 0x63,
        0xCB, 0x63,
        0xCB, 0x64,
        0xCB, 0x64,
        0xCB, 0x65,
        0xCB, 0x65,
        0xCB, 0x67,
        0xCB, 0x67
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    // Test register B, bit 4
    rf.BC.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3);
    // Zero and half carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10100000);
    rf.BC.l = 0b00010000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 5);
    // Half carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00100000);

    // Test register C, bit 4
    rf.BC.r = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 7);
    // Zero and half carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10100000);
    rf.BC.r = 0b00010000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 9);
    // Half carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00100000);

    // Test register D, bit 4
    rf.DE.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 11);
    // Zero and half carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10100000);
    rf.DE.l = 0b00010000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 13);
    // Half carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00100000);

    // Test register E, bit 4
    rf.DE.r = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 15);
    // Zero and half carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10100000);
    rf.DE.r = 0b00010000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 17);
    // Half carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00100000);

    // Test register H, bit 4
    rf.HL.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 19);
    // Zero and half carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10100000);
    rf.HL.l = 0b00010000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 21);
    // Half carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00100000);

    // Test register L, bit 4
    rf.HL.r = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 23);
    // Zero and half carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10100000);
    rf.HL.r = 0b00010000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 25);
    // Half carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00100000);

    // Test register A, bit 4
    rf.AF.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 27);
    // Zero and half carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10100000);
    rf.AF.l = 0b00010000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 29);
    // Half carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00100000);

    return MUNIT_OK;
}

static MunitResult test_bit_test_4_hl_indirect() {
    // Test if bit 4 in mem value at mem address
    // in register HL is 0, 0xCB66
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0x66,
        0xCB, 0x66,
    };
    memcpy(memory, instructions, sizeof(instructions));

    rf.HL.lr = 0xBEEF;
    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    memory[0xBEEF] = 0b00000000;
    clock_cpu(); clock_cpu(); clock_cpu(); // execute (takes 3 cycles)
    munit_assert_int(rf.PC, ==, 3);
    // Zero and half carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10100000);
    memory[0xBEEF] = 0b00010000;
    clock_cpu(); clock_cpu(); clock_cpu(); // execute (takes 3 cycles)
    munit_assert_int(rf.PC, ==, 5);
    // Half carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00100000);

    return MUNIT_OK;
}

static MunitResult test_bit_test_5_register() {
    // Test if bit 5 in register is 0
    // 0xCB68, 0xCB69, 0xCB6A, 0xCB6B, 0xCB6C, 0xCB6D, 0xCB6F
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0x68,
        0xCB, 0x68,
        0xCB, 0x69,
        0xCB, 0x69,
        0xCB, 0x6a,
        0xCB, 0x6a,
        0xCB, 0x6b,
        0xCB, 0x6b,
        0xCB, 0x6c,
        0xCB, 0x6c,
        0xCB, 0x6d,
        0xCB, 0x6d,
        0xCB, 0x6f,
        0xCB, 0x6f
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    // Test register B, bit 5
    rf.BC.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3);
    // Zero and half carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10100000);
    rf.BC.l = 0b00100000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 5);
    // Half carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00100000);

    // Test register C, bit 5
    rf.BC.r = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 7);
    // Zero and half carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10100000);
    rf.BC.r = 0b00100000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 9);
    // Half carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00100000);

    // Test register D, bit 5
    rf.DE.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 11);
    // Zero and half carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10100000);
    rf.DE.l = 0b00100000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 13);
    // Half carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00100000);

    // Test register E, bit 5
    rf.DE.r = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 15);
    // Zero and half carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10100000);
    rf.DE.r = 0b00100000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 17);
    // Half carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00100000);

    // Test register H, bit 5
    rf.HL.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 19);
    // Zero and half carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10100000);
    rf.HL.l = 0b00100000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 21);
    // Half carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00100000);

    // Test register L, bit 5
    rf.HL.r = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 23);
    // Zero and half carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10100000);
    rf.HL.r = 0b00100000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 25);
    // Half carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00100000);

    // Test register A, bit 5
    rf.AF.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 27);
    // Zero and half carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10100000);
    rf.AF.l = 0b00100000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 29);
    // Half carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00100000);

    return MUNIT_OK;
}

static MunitResult test_bit_test_5_hl_indirect() {
    // Test if bit 5 in mem value at mem address
    // in register HL is 0, 0xCB6E
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0x6e,
        0xCB, 0x6e,
    };
    memcpy(memory, instructions, sizeof(instructions));

    rf.HL.lr = 0xBEEF;
    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    memory[0xBEEF] = 0b00000000;
    clock_cpu(); clock_cpu(); clock_cpu(); // execute (takes 3 cycles)
    munit_assert_int(rf.PC, ==, 3);
    // Zero and half carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10100000);
    memory[0xBEEF] = 0b00100000;
    clock_cpu(); clock_cpu(); clock_cpu(); // execute (takes 3 cycles)
    munit_assert_int(rf.PC, ==, 5);
    // Half carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00100000);

    return MUNIT_OK;
}

static MunitResult test_bit_test_6_register() {
    // Test if bit 6 in register is 0
    // 0xCB70, 0xCB71, 0xCB72, 0xCB73, 0xCB74, 0xCB75, 0xCB77
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0x70,
        0xCB, 0x70,
        0xCB, 0x71,
        0xCB, 0x71,
        0xCB, 0x72,
        0xCB, 0x72,
        0xCB, 0x73,
        0xCB, 0x73,
        0xCB, 0x74,
        0xCB, 0x74,
        0xCB, 0x75,
        0xCB, 0x75,
        0xCB, 0x77,
        0xCB, 0x77
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    // Test register B, bit 6
    rf.BC.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3);
    // Zero and half carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10100000);
    rf.BC.l = 0b01000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 5);
    // Half carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00100000);

    // Test register C, bit 6
    rf.BC.r = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 7);
    // Zero and half carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10100000);
    rf.BC.r = 0b01000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 9);
    // Half carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00100000);

    // Test register D, bit 6
    rf.DE.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 11);
    // Zero and half carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10100000);
    rf.DE.l = 0b01000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 13);
    // Half carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00100000);

    // Test register E, bit 6
    rf.DE.r = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 15);
    // Zero and half carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10100000);
    rf.DE.r = 0b01000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 17);
    // Half carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00100000);

    // Test register H, bit 6
    rf.HL.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 19);
    // Zero and half carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10100000);
    rf.HL.l = 0b01000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 21);
    // Half carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00100000);

    // Test register L, bit 6
    rf.HL.r = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 23);
    // Zero and half carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10100000);
    rf.HL.r = 0b01000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 25);
    // Half carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00100000);

    // Test register A, bit 6
    rf.AF.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 27);
    // Zero and half carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10100000);
    rf.AF.l = 0b01000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 29);
    // Half carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00100000);

    return MUNIT_OK;
}

static MunitResult test_bit_test_6_hl_indirect() {
    // Test if bit 6 in mem value at mem address
    // in register HL is 0, 0xCB76
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0x76,
        0xCB, 0x76,
    };
    memcpy(memory, instructions, sizeof(instructions));

    rf.HL.lr = 0xBEEF;
    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    memory[0xBEEF] = 0b00000000;
    clock_cpu(); clock_cpu(); clock_cpu(); // execute (takes 3 cycles)
    munit_assert_int(rf.PC, ==, 3);
    // Zero and half carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10100000);
    memory[0xBEEF] = 0b01000000;
    clock_cpu(); clock_cpu(); clock_cpu(); // execute (takes 3 cycles)
    munit_assert_int(rf.PC, ==, 5);
    // Half carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00100000);

    return MUNIT_OK;
}

static MunitResult test_bit_test_7_register() {
    // Test if bit 7 in register is 0
    // 0xCB78, 0xCB79, 0xCB7A, 0xCB7B, 0xCB7C, 0xCB7D, 0xCB7F
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0x78,
        0xCB, 0x78,
        0xCB, 0x79,
        0xCB, 0x79,
        0xCB, 0x7a,
        0xCB, 0x7a,
        0xCB, 0x7b,
        0xCB, 0x7b,
        0xCB, 0x7c,
        0xCB, 0x7c,
        0xCB, 0x7d,
        0xCB, 0x7d,
        0xCB, 0x7f,
        0xCB, 0x7f
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    // Test register B, bit 7
    rf.BC.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3);
    // Zero and half carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10100000);
    rf.BC.l = 0b10000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 5);
    // Half carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00100000);

    // Test register C, bit 7
    rf.BC.r = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 7);
    // Zero and half carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10100000);
    rf.BC.r = 0b10000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 9);
    // Half carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00100000);

    // Test register D, bit 7
    rf.DE.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 11);
    // Zero and half carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10100000);
    rf.DE.l = 0b10000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 13);
    // Half carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00100000);

    // Test register E, bit 7
    rf.DE.r = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 15);
    // Zero and half carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10100000);
    rf.DE.r = 0b10000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 17);
    // Half carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00100000);

    // Test register H, bit 7
    rf.HL.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 19);
    // Zero and half carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10100000);
    rf.HL.l = 0b10000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 21);
    // Half carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00100000);

    // Test register L, bit 7
    rf.HL.r = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 23);
    // Zero and half carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10100000);
    rf.HL.r = 0b10000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 25);
    // Half carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00100000);

    // Test register A, bit 7
    rf.AF.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 27);
    // Zero and half carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10100000);
    rf.AF.l = 0b10000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 29);
    // Half carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00100000);

    return MUNIT_OK;
}

static MunitResult test_bit_test_7_hl_indirect() {
    // Test if bit 7 in mem value at mem address
    // in register HL is 0, 0xCB7E
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0x7e,
        0xCB, 0x7e,
    };
    memcpy(memory, instructions, sizeof(instructions));

    rf.HL.lr = 0xBEEF;
    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    memory[0xBEEF] = 0b00000000;
    clock_cpu(); clock_cpu(); clock_cpu(); // execute (takes 3 cycles)
    munit_assert_int(rf.PC, ==, 3);
    // Zero and half carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10100000);
    memory[0xBEEF] = 0b10000000;
    clock_cpu(); clock_cpu(); clock_cpu(); // execute (takes 3 cycles)
    munit_assert_int(rf.PC, ==, 5);
    // Half carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00100000);

    return MUNIT_OK;
}

static MunitResult test_reset_bit_0_register() {
    // Set bit 0 in register to 0
    // 0xCB80, 0xCB81, 0xCB82, 0xCB83, 0xCB84, 0xCB85, 0xCB87
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0x80,
        0xCB, 0x80,
        0xCB, 0x81,
        0xCB, 0x81,
        0xCB, 0x82,
        0xCB, 0x82,
        0xCB, 0x83,
        0xCB, 0x83,
        0xCB, 0x84,
        0xCB, 0x84,
        0xCB, 0x85,
        0xCB, 0x85,
        0xCB, 0x87,
        0xCB, 0x87
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    // Reset register B, bit 0
    rf.BC.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(rf.BC.l, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.BC.l = 0b00000001;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(rf.BC.l, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Reset register C, bit 0
    rf.BC.r = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 7);
    munit_assert_int(rf.BC.r, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.BC.r = 0b00000001;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 9);
    munit_assert_int(rf.BC.r, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Reset register D, bit 0
    rf.DE.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 11);
    munit_assert_int(rf.DE.l, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.DE.l = 0b00000001;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 13);
    munit_assert_int(rf.DE.l, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Reset register E, bit 0
    rf.DE.r = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 15);
    munit_assert_int(rf.DE.r, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.DE.r = 0b00000001;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 17);
    munit_assert_int(rf.DE.r, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Reset register H, bit 0
    rf.HL.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 19);
    munit_assert_int(rf.HL.l, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.HL.l = 0b00000001;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 21);
    munit_assert_int(rf.HL.l, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Reset register L, bit 0
    rf.HL.r = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 23);
    munit_assert_int(rf.HL.r, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.HL.r = 0b00000001;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 25);
    munit_assert_int(rf.HL.r, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Reset register A, bit 0
    rf.AF.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 27);
    munit_assert_int(rf.AF.l, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.AF.l = 0b00000001;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 29);
    munit_assert_int(rf.AF.l, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    return MUNIT_OK;
}

static MunitResult test_reset_bit_0_hl_indirect() {
    // Set bit 0 in mem value at mem address
    // in register HL to 0, 0xCB86
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0x86,
        0xCB, 0x86,
    };
    memcpy(memory, instructions, sizeof(instructions));

    rf.HL.lr = 0xBEEF;
    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    memory[0xBEEF] = 0b00000000;
    // execute (takes 4 cycles)
    clock_cpu(); clock_cpu();
    clock_cpu(); clock_cpu();
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(memory[0xBEEF], ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    memory[0xBEEF] = 0b00000001;
    // execute (takes 4 cycles)
    clock_cpu(); clock_cpu();
    clock_cpu(); clock_cpu();
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(memory[0xBEEF], ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    return MUNIT_OK;
}

static MunitResult test_reset_bit_1_register() {
    // Set bit 1 in register to 0
    // 0xCB88, 0xCB89, 0xCB8A, 0xCB8B, 0xCB8C, 0xCB8D, 0xCB8F
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0x88,
        0xCB, 0x88,
        0xCB, 0x89,
        0xCB, 0x89,
        0xCB, 0x8a,
        0xCB, 0x8a,
        0xCB, 0x8b,
        0xCB, 0x8b,
        0xCB, 0x8c,
        0xCB, 0x8c,
        0xCB, 0x8d,
        0xCB, 0x8d,
        0xCB, 0x8f,
        0xCB, 0x8f
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    // Reset register B, bit 1
    rf.BC.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(rf.BC.l, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.BC.l = 0b00000010;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(rf.BC.l, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Reset register C, bit 1
    rf.BC.r = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 7);
    munit_assert_int(rf.BC.r, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.BC.r = 0b00000010;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 9);
    munit_assert_int(rf.BC.r, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Reset register D, bit 1
    rf.DE.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 11);
    munit_assert_int(rf.DE.l, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.DE.l = 0b00000010;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 13);
    munit_assert_int(rf.DE.l, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Reset register E, bit 1
    rf.DE.r = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 15);
    munit_assert_int(rf.DE.r, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.DE.r = 0b00000010;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 17);
    munit_assert_int(rf.DE.r, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Reset register H, bit 1
    rf.HL.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 19);
    munit_assert_int(rf.HL.l, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.HL.l = 0b00000010;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 21);
    munit_assert_int(rf.HL.l, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Reset register L, bit 1
    rf.HL.r = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 23);
    munit_assert_int(rf.HL.r, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.HL.r = 0b00000010;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 25);
    munit_assert_int(rf.HL.r, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Reset register A, bit 1
    rf.AF.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 27);
    munit_assert_int(rf.AF.l, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.AF.l = 0b00000010;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 29);
    munit_assert_int(rf.AF.l, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    return MUNIT_OK;
}

static MunitResult test_reset_bit_1_hl_indirect() {
    // Set bit 1 in mem value at mem address
    // in register HL to 0, 0xCB8E
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0x8e,
        0xCB, 0x8e,
    };
    memcpy(memory, instructions, sizeof(instructions));

    rf.HL.lr = 0xBEEF;
    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    memory[0xBEEF] = 0b00000000;
    // execute (takes 4 cycles)
    clock_cpu(); clock_cpu();
    clock_cpu(); clock_cpu();
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(memory[0xBEEF], ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    memory[0xBEEF] = 0b00000010;
    // execute (takes 4 cycles)
    clock_cpu(); clock_cpu();
    clock_cpu(); clock_cpu();
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(memory[0xBEEF], ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    return MUNIT_OK;
}

static MunitResult test_reset_bit_2_register() {
    // Set bit 2 in register to 0
    // 0xCB90, 0xCB91, 0xCB92, 0xCB93, 0xCB94, 0xCB95, 0xCB97
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0x90,
        0xCB, 0x90,
        0xCB, 0x91,
        0xCB, 0x91,
        0xCB, 0x92,
        0xCB, 0x92,
        0xCB, 0x93,
        0xCB, 0x93,
        0xCB, 0x94,
        0xCB, 0x94,
        0xCB, 0x95,
        0xCB, 0x95,
        0xCB, 0x97,
        0xCB, 0x97
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    // Reset register B, bit 2
    rf.BC.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(rf.BC.l, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.BC.l = 0b00000100;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(rf.BC.l, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Reset register C, bit 2
    rf.BC.r = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 7);
    munit_assert_int(rf.BC.r, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.BC.r = 0b00000100;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 9);
    munit_assert_int(rf.BC.r, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Reset register D, bit 2
    rf.DE.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 11);
    munit_assert_int(rf.DE.l, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.DE.l = 0b00000100;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 13);
    munit_assert_int(rf.DE.l, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Reset register E, bit 2
    rf.DE.r = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 15);
    munit_assert_int(rf.DE.r, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.DE.r = 0b00000100;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 17);
    munit_assert_int(rf.DE.r, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Reset register H, bit 2
    rf.HL.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 19);
    munit_assert_int(rf.HL.l, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.HL.l = 0b00000100;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 21);
    munit_assert_int(rf.HL.l, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Reset register L, bit 2
    rf.HL.r = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 23);
    munit_assert_int(rf.HL.r, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.HL.r = 0b00000100;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 25);
    munit_assert_int(rf.HL.r, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Reset register A, bit 2
    rf.AF.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 27);
    munit_assert_int(rf.AF.l, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.AF.l = 0b00000100;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 29);
    munit_assert_int(rf.AF.l, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    return MUNIT_OK;
}

static MunitResult test_reset_bit_2_hl_indirect() {
    // Set bit 2 in mem value at mem address
    // in register HL to 0, 0xCB96
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0x96,
        0xCB, 0x96,
    };
    memcpy(memory, instructions, sizeof(instructions));

    rf.HL.lr = 0xBEEF;
    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    memory[0xBEEF] = 0b00000000;
    // execute (takes 4 cycles)
    clock_cpu(); clock_cpu();
    clock_cpu(); clock_cpu();
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(memory[0xBEEF], ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    memory[0xBEEF] = 0b00000100;
    // execute (takes 4 cycles)
    clock_cpu(); clock_cpu();
    clock_cpu(); clock_cpu();
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(memory[0xBEEF], ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    return MUNIT_OK;
}

static MunitResult test_reset_bit_3_register() {
    // Set bit 3 in register to 0
    // 0xCB98, 0xCB99, 0xCB9A, 0xCB9B, 0xCB9C, 0xCB9D, 0xCB9F
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0x98,
        0xCB, 0x98,
        0xCB, 0x99,
        0xCB, 0x99,
        0xCB, 0x9a,
        0xCB, 0x9a,
        0xCB, 0x9b,
        0xCB, 0x9b,
        0xCB, 0x9c,
        0xCB, 0x9c,
        0xCB, 0x9d,
        0xCB, 0x9d,
        0xCB, 0x9f,
        0xCB, 0x9f
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    // Reset register B, bit 3
    rf.BC.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(rf.BC.l, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.BC.l = 0b00001000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(rf.BC.l, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Reset register C, bit 3
    rf.BC.r = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 7);
    munit_assert_int(rf.BC.r, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.BC.r = 0b00001000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 9);
    munit_assert_int(rf.BC.r, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Reset register D, bit 3
    rf.DE.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 11);
    munit_assert_int(rf.DE.l, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.DE.l = 0b00001000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 13);
    munit_assert_int(rf.DE.l, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Reset register E, bit 3
    rf.DE.r = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 15);
    munit_assert_int(rf.DE.r, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.DE.r = 0b00001000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 17);
    munit_assert_int(rf.DE.r, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Reset register H, bit 3
    rf.HL.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 19);
    munit_assert_int(rf.HL.l, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.HL.l = 0b00001000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 21);
    munit_assert_int(rf.HL.l, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Reset register L, bit 3
    rf.HL.r = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 23);
    munit_assert_int(rf.HL.r, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.HL.r = 0b00001000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 25);
    munit_assert_int(rf.HL.r, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Reset register A, bit 3
    rf.AF.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 27);
    munit_assert_int(rf.AF.l, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.AF.l = 0b00001000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 29);
    munit_assert_int(rf.AF.l, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    return MUNIT_OK;
}

static MunitResult test_reset_bit_3_hl_indirect() {
    // Set bit 3 in mem value at mem address
    // in register HL to 0, 0xCB9E
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0x9e,
        0xCB, 0x9e,
    };
    memcpy(memory, instructions, sizeof(instructions));

    rf.HL.lr = 0xBEEF;
    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    memory[0xBEEF] = 0b00000000;
    // execute (takes 4 cycles)
    clock_cpu(); clock_cpu();
    clock_cpu(); clock_cpu();
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(memory[0xBEEF], ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    memory[0xBEEF] = 0b00001000;
    // execute (takes 4 cycles)
    clock_cpu(); clock_cpu();
    clock_cpu(); clock_cpu();
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(memory[0xBEEF], ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    return MUNIT_OK;
}

static MunitResult test_reset_bit_4_register() {
    // Set bit 4 in register to 0
    // 0xCBA0, 0xCBA1, 0xCBA2, 0xCBA3, 0xCBA4, 0xCBA5, 0xCBA7
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0xa0,
        0xCB, 0xa0,
        0xCB, 0xa1,
        0xCB, 0xa1,
        0xCB, 0xa2,
        0xCB, 0xa2,
        0xCB, 0xa3,
        0xCB, 0xa3,
        0xCB, 0xa4,
        0xCB, 0xa4,
        0xCB, 0xa5,
        0xCB, 0xa5,
        0xCB, 0xa7,
        0xCB, 0xa7
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    // Reset register B, bit 4
    rf.BC.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(rf.BC.l, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.BC.l = 0b00010000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(rf.BC.l, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Reset register C, bit 4
    rf.BC.r = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 7);
    munit_assert_int(rf.BC.r, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.BC.r = 0b00010000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 9);
    munit_assert_int(rf.BC.r, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Reset register D, bit 4
    rf.DE.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 11);
    munit_assert_int(rf.DE.l, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.DE.l = 0b00010000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 13);
    munit_assert_int(rf.DE.l, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Reset register E, bit 4
    rf.DE.r = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 15);
    munit_assert_int(rf.DE.r, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.DE.r = 0b00010000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 17);
    munit_assert_int(rf.DE.r, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Reset register H, bit 4
    rf.HL.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 19);
    munit_assert_int(rf.HL.l, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.HL.l = 0b00010000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 21);
    munit_assert_int(rf.HL.l, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Reset register L, bit 4
    rf.HL.r = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 23);
    munit_assert_int(rf.HL.r, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.HL.r = 0b00010000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 25);
    munit_assert_int(rf.HL.r, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Reset register A, bit 4
    rf.AF.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 27);
    munit_assert_int(rf.AF.l, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.AF.l = 0b00010000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 29);
    munit_assert_int(rf.AF.l, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    return MUNIT_OK;
}

static MunitResult test_reset_bit_4_hl_indirect() {
    // Set bit 4 in mem value at mem address
    // in register HL to 0, 0xCBA6
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0xa6,
        0xCB, 0xa6,
    };
    memcpy(memory, instructions, sizeof(instructions));

    rf.HL.lr = 0xBEEF;
    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    memory[0xBEEF] = 0b00000000;
    // execute (takes 4 cycles)
    clock_cpu(); clock_cpu();
    clock_cpu(); clock_cpu();
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(memory[0xBEEF], ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    memory[0xBEEF] = 0b00010000;
    // execute (takes 4 cycles)
    clock_cpu(); clock_cpu();
    clock_cpu(); clock_cpu();
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(memory[0xBEEF], ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    return MUNIT_OK;
}

static MunitResult test_reset_bit_5_register() {
    // Set bit 5 in register to 0
    // 0xCBA8, 0xCBA9, 0xCBAA, 0xCBAB, 0xCBAC, 0xCBAD, 0xCBAF
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0xa8,
        0xCB, 0xa8,
        0xCB, 0xa9,
        0xCB, 0xa9,
        0xCB, 0xaa,
        0xCB, 0xaa,
        0xCB, 0xab,
        0xCB, 0xab,
        0xCB, 0xac,
        0xCB, 0xac,
        0xCB, 0xad,
        0xCB, 0xad,
        0xCB, 0xaf,
        0xCB, 0xaf
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    // Reset register B, bit 5
    rf.BC.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(rf.BC.l, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.BC.l = 0b00100000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(rf.BC.l, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Reset register C, bit 5
    rf.BC.r = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 7);
    munit_assert_int(rf.BC.r, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.BC.r = 0b00100000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 9);
    munit_assert_int(rf.BC.r, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Reset register D, bit 5
    rf.DE.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 11);
    munit_assert_int(rf.DE.l, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.DE.l = 0b00100000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 13);
    munit_assert_int(rf.DE.l, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Reset register E, bit 5
    rf.DE.r = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 15);
    munit_assert_int(rf.DE.r, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.DE.r = 0b00100000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 17);
    munit_assert_int(rf.DE.r, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Reset register H, bit 5
    rf.HL.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 19);
    munit_assert_int(rf.HL.l, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.HL.l = 0b00100000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 21);
    munit_assert_int(rf.HL.l, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Reset register L, bit 5
    rf.HL.r = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 23);
    munit_assert_int(rf.HL.r, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.HL.r = 0b00100000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 25);
    munit_assert_int(rf.HL.r, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Reset register A, bit 5
    rf.AF.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 27);
    munit_assert_int(rf.AF.l, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.AF.l = 0b00100000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 29);
    munit_assert_int(rf.AF.l, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    return MUNIT_OK;
}

static MunitResult test_reset_bit_5_hl_indirect() {
    // Set bit 5 in mem value at mem address
    // in register HL to 0, 0xCBAE
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0xae,
        0xCB, 0xae,
    };
    memcpy(memory, instructions, sizeof(instructions));

    rf.HL.lr = 0xBEEF;
    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    memory[0xBEEF] = 0b00000000;
    // execute (takes 4 cycles)
    clock_cpu(); clock_cpu();
    clock_cpu(); clock_cpu();
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(memory[0xBEEF], ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    memory[0xBEEF] = 0b00100000;
    // execute (takes 4 cycles)
    clock_cpu(); clock_cpu();
    clock_cpu(); clock_cpu();
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(memory[0xBEEF], ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    return MUNIT_OK;
}

static MunitResult test_reset_bit_6_register() {
    // Set bit 6 in register to 0
    // 0xCBB0, 0xCBB1, 0xCBB2, 0xCBB3, 0xCBB4, 0xCBB5, 0xCBB7
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0xb0,
        0xCB, 0xb0,
        0xCB, 0xb1,
        0xCB, 0xb1,
        0xCB, 0xb2,
        0xCB, 0xb2,
        0xCB, 0xb3,
        0xCB, 0xb3,
        0xCB, 0xb4,
        0xCB, 0xb4,
        0xCB, 0xb5,
        0xCB, 0xb5,
        0xCB, 0xb7,
        0xCB, 0xb7
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    // Reset register B, bit 6
    rf.BC.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(rf.BC.l, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.BC.l = 0b01000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(rf.BC.l, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Reset register C, bit 6
    rf.BC.r = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 7);
    munit_assert_int(rf.BC.r, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.BC.r = 0b01000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 9);
    munit_assert_int(rf.BC.r, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Reset register D, bit 6
    rf.DE.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 11);
    munit_assert_int(rf.DE.l, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.DE.l = 0b01000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 13);
    munit_assert_int(rf.DE.l, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Reset register E, bit 6
    rf.DE.r = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 15);
    munit_assert_int(rf.DE.r, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.DE.r = 0b01000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 17);
    munit_assert_int(rf.DE.r, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Reset register H, bit 6
    rf.HL.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 19);
    munit_assert_int(rf.HL.l, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.HL.l = 0b01000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 21);
    munit_assert_int(rf.HL.l, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Reset register L, bit 6
    rf.HL.r = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 23);
    munit_assert_int(rf.HL.r, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.HL.r = 0b01000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 25);
    munit_assert_int(rf.HL.r, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Reset register A, bit 6
    rf.AF.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 27);
    munit_assert_int(rf.AF.l, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.AF.l = 0b01000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 29);
    munit_assert_int(rf.AF.l, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    return MUNIT_OK;
}

static MunitResult test_reset_bit_6_hl_indirect() {
    // Set bit 6 in mem value at mem address
    // in register HL to 0, 0xCBB6
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0xb6,
        0xCB, 0xb6,
    };
    memcpy(memory, instructions, sizeof(instructions));

    rf.HL.lr = 0xBEEF;
    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    memory[0xBEEF] = 0b00000000;
    // execute (takes 4 cycles)
    clock_cpu(); clock_cpu();
    clock_cpu(); clock_cpu();
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(memory[0xBEEF], ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    memory[0xBEEF] = 0b01000000;
    // execute (takes 4 cycles)
    clock_cpu(); clock_cpu();
    clock_cpu(); clock_cpu();
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(memory[0xBEEF], ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    return MUNIT_OK;
}

static MunitResult test_reset_bit_7_register() {
    // Set bit 7 in register to 0
    // 0xCBB8, 0xCBB9, 0xCBBA, 0xCBBB, 0xCBBC, 0xCBBD, 0xCBBF
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0xb8,
        0xCB, 0xb8,
        0xCB, 0xb9,
        0xCB, 0xb9,
        0xCB, 0xba,
        0xCB, 0xba,
        0xCB, 0xbb,
        0xCB, 0xbb,
        0xCB, 0xbc,
        0xCB, 0xbc,
        0xCB, 0xbd,
        0xCB, 0xbd,
        0xCB, 0xbf,
        0xCB, 0xbf
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    // Reset register B, bit 7
    rf.BC.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(rf.BC.l, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.BC.l = 0b10000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(rf.BC.l, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Reset register C, bit 7
    rf.BC.r = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 7);
    munit_assert_int(rf.BC.r, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.BC.r = 0b10000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 9);
    munit_assert_int(rf.BC.r, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Reset register D, bit 7
    rf.DE.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 11);
    munit_assert_int(rf.DE.l, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.DE.l = 0b10000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 13);
    munit_assert_int(rf.DE.l, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Reset register E, bit 7
    rf.DE.r = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 15);
    munit_assert_int(rf.DE.r, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.DE.r = 0b10000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 17);
    munit_assert_int(rf.DE.r, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Reset register H, bit 7
    rf.HL.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 19);
    munit_assert_int(rf.HL.l, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.HL.l = 0b10000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 21);
    munit_assert_int(rf.HL.l, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Reset register L, bit 7
    rf.HL.r = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 23);
    munit_assert_int(rf.HL.r, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.HL.r = 0b10000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 25);
    munit_assert_int(rf.HL.r, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Reset register A, bit 7
    rf.AF.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 27);
    munit_assert_int(rf.AF.l, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.AF.l = 0b10000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 29);
    munit_assert_int(rf.AF.l, ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    return MUNIT_OK;
}

static MunitResult test_reset_bit_7_hl_indirect() {
    // Set bit 7 in mem value at mem address
    // in register HL to 0, 0xCBBE
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0xbe,
        0xCB, 0xbe,
    };
    memcpy(memory, instructions, sizeof(instructions));

    rf.HL.lr = 0xBEEF;
    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    memory[0xBEEF] = 0b00000000;
    // execute (takes 4 cycles)
    clock_cpu(); clock_cpu();
    clock_cpu(); clock_cpu();
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(memory[0xBEEF], ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    memory[0xBEEF] = 0b10000000;
    // execute (takes 4 cycles)
    clock_cpu(); clock_cpu();
    clock_cpu(); clock_cpu();
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(memory[0xBEEF], ==, 0);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    return MUNIT_OK;
}

static MunitResult test_set_bit_0_register() {
    // Set bit 0 in register to 1
    // 0xCBC0, 0xCBC1, 0xCBC2, 0xCBC3, 0xCBC4, 0xCBC5, 0xCBC7
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0xc0,
        0xCB, 0xc0,
        0xCB, 0xc1,
        0xCB, 0xc1,
        0xCB, 0xc2,
        0xCB, 0xc2,
        0xCB, 0xc3,
        0xCB, 0xc3,
        0xCB, 0xc4,
        0xCB, 0xc4,
        0xCB, 0xc5,
        0xCB, 0xc5,
        0xCB, 0xc7,
        0xCB, 0xc7
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    // Set register B, bit 0
    rf.BC.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(rf.BC.l, ==, 0b00000001);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.BC.l = 0b00000001;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(rf.BC.l, ==, 0b00000001);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Set register C, bit 0
    rf.BC.r = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 7);
    munit_assert_int(rf.BC.r, ==, 0b00000001);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.BC.r = 0b00000001;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 9);
    munit_assert_int(rf.BC.r, ==, 0b00000001);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Set register D, bit 0
    rf.DE.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 11);
    munit_assert_int(rf.DE.l, ==, 0b00000001);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.DE.l = 0b00000001;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 13);
    munit_assert_int(rf.DE.l, ==, 0b00000001);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Set register E, bit 0
    rf.DE.r = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 15);
    munit_assert_int(rf.DE.r, ==, 0b00000001);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.DE.r = 0b00000001;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 17);
    munit_assert_int(rf.DE.r, ==, 0b00000001);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Set register H, bit 0
    rf.HL.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 19);
    munit_assert_int(rf.HL.l, ==, 0b00000001);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.HL.l = 0b00000001;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 21);
    munit_assert_int(rf.HL.l, ==, 0b00000001);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Set register L, bit 0
    rf.HL.r = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 23);
    munit_assert_int(rf.HL.r, ==, 0b00000001);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.HL.r = 0b00000001;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 25);
    munit_assert_int(rf.HL.r, ==, 0b00000001);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Set register A, bit 0
    rf.AF.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 27);
    munit_assert_int(rf.AF.l, ==, 0b00000001);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.AF.l = 0b00000001;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 29);
    munit_assert_int(rf.AF.l, ==, 0b00000001);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    return MUNIT_OK;
}

static MunitResult test_set_bit_0_hl_indirect() {
    // Set bit 0 in mem value at mem address
    // in register HL to 1, 0xCBC6
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0xc6,
        0xCB, 0xc6,
    };
    memcpy(memory, instructions, sizeof(instructions));

    rf.HL.lr = 0xBEEF;
    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    memory[0xBEEF] = 0b00000000;
    // execute (takes 4 cycles)
    clock_cpu(); clock_cpu();
    clock_cpu(); clock_cpu();
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(memory[0xBEEF], ==, 0b00000001);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    memory[0xBEEF] = 0b00000001;
    // execute (takes 4 cycles)
    clock_cpu(); clock_cpu();
    clock_cpu(); clock_cpu();
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(memory[0xBEEF], ==, 0b00000001);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    return MUNIT_OK;
}

static MunitResult test_set_bit_1_register() {
    // Set bit 1 in register to 1
    // 0xCBC8, 0xCBC9, 0xCBCA, 0xCBCB, 0xCBCC, 0xCBCD, 0xCBCF
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0xc8,
        0xCB, 0xc8,
        0xCB, 0xc9,
        0xCB, 0xc9,
        0xCB, 0xca,
        0xCB, 0xca,
        0xCB, 0xcb,
        0xCB, 0xcb,
        0xCB, 0xcc,
        0xCB, 0xcc,
        0xCB, 0xcd,
        0xCB, 0xcd,
        0xCB, 0xcf,
        0xCB, 0xcf
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    // Set register B, bit 1
    rf.BC.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(rf.BC.l, ==, 0b00000010);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.BC.l = 0b00000010;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(rf.BC.l, ==, 0b00000010);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Set register C, bit 1
    rf.BC.r = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 7);
    munit_assert_int(rf.BC.r, ==, 0b00000010);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.BC.r = 0b00000010;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 9);
    munit_assert_int(rf.BC.r, ==, 0b00000010);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Set register D, bit 1
    rf.DE.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 11);
    munit_assert_int(rf.DE.l, ==, 0b00000010);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.DE.l = 0b00000010;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 13);
    munit_assert_int(rf.DE.l, ==, 0b00000010);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Set register E, bit 1
    rf.DE.r = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 15);
    munit_assert_int(rf.DE.r, ==, 0b00000010);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.DE.r = 0b00000010;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 17);
    munit_assert_int(rf.DE.r, ==, 0b00000010);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Set register H, bit 1
    rf.HL.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 19);
    munit_assert_int(rf.HL.l, ==, 0b00000010);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.HL.l = 0b00000010;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 21);
    munit_assert_int(rf.HL.l, ==, 0b00000010);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Set register L, bit 1
    rf.HL.r = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 23);
    munit_assert_int(rf.HL.r, ==, 0b00000010);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.HL.r = 0b00000010;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 25);
    munit_assert_int(rf.HL.r, ==, 0b00000010);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Set register A, bit 1
    rf.AF.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 27);
    munit_assert_int(rf.AF.l, ==, 0b00000010);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.AF.l = 0b00000010;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 29);
    munit_assert_int(rf.AF.l, ==, 0b00000010);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    return MUNIT_OK;
}

static MunitResult test_set_bit_1_hl_indirect() {
    // Set bit 1 in mem value at mem address
    // in register HL to 1, 0xCBCE
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0xce,
        0xCB, 0xce,
    };
    memcpy(memory, instructions, sizeof(instructions));

    rf.HL.lr = 0xBEEF;
    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    memory[0xBEEF] = 0b00000000;
    // execute (takes 4 cycles)
    clock_cpu(); clock_cpu();
    clock_cpu(); clock_cpu();
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(memory[0xBEEF], ==, 0b00000010);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    memory[0xBEEF] = 0b00000010;
    // execute (takes 4 cycles)
    clock_cpu(); clock_cpu();
    clock_cpu(); clock_cpu();
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(memory[0xBEEF], ==, 0b00000010);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    return MUNIT_OK;
}

static MunitResult test_set_bit_2_register() {
    // Set bit 2 in register to 1
    // 0xCBD0, 0xCBD1, 0xCBD2, 0xCBD3, 0xCBD4, 0xCBD5, 0xCBD7
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0xd0,
        0xCB, 0xd0,
        0xCB, 0xd1,
        0xCB, 0xd1,
        0xCB, 0xd2,
        0xCB, 0xd2,
        0xCB, 0xd3,
        0xCB, 0xd3,
        0xCB, 0xd4,
        0xCB, 0xd4,
        0xCB, 0xd5,
        0xCB, 0xd5,
        0xCB, 0xd7,
        0xCB, 0xd7
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    // Set register B, bit 2
    rf.BC.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(rf.BC.l, ==, 0b00000100);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.BC.l = 0b00000100;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(rf.BC.l, ==, 0b00000100);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Set register C, bit 2
    rf.BC.r = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 7);
    munit_assert_int(rf.BC.r, ==, 0b00000100);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.BC.r = 0b00000100;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 9);
    munit_assert_int(rf.BC.r, ==, 0b00000100);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Set register D, bit 2
    rf.DE.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 11);
    munit_assert_int(rf.DE.l, ==, 0b00000100);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.DE.l = 0b00000100;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 13);
    munit_assert_int(rf.DE.l, ==, 0b00000100);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Set register E, bit 2
    rf.DE.r = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 15);
    munit_assert_int(rf.DE.r, ==, 0b00000100);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.DE.r = 0b00000100;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 17);
    munit_assert_int(rf.DE.r, ==, 0b00000100);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Set register H, bit 2
    rf.HL.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 19);
    munit_assert_int(rf.HL.l, ==, 0b00000100);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.HL.l = 0b00000100;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 21);
    munit_assert_int(rf.HL.l, ==, 0b00000100);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Set register L, bit 2
    rf.HL.r = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 23);
    munit_assert_int(rf.HL.r, ==, 0b00000100);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.HL.r = 0b00000100;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 25);
    munit_assert_int(rf.HL.r, ==, 0b00000100);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Set register A, bit 2
    rf.AF.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 27);
    munit_assert_int(rf.AF.l, ==, 0b00000100);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.AF.l = 0b00000100;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 29);
    munit_assert_int(rf.AF.l, ==, 0b00000100);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    return MUNIT_OK;
}

static MunitResult test_set_bit_2_hl_indirect() {
    // Set bit 2 in mem value at mem address
    // in register HL to 1, 0xCBD6
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0xd6,
        0xCB, 0xd6,
    };
    memcpy(memory, instructions, sizeof(instructions));

    rf.HL.lr = 0xBEEF;
    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    memory[0xBEEF] = 0b00000000;
    // execute (takes 4 cycles)
    clock_cpu(); clock_cpu();
    clock_cpu(); clock_cpu();
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(memory[0xBEEF], ==, 0b00000100);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    memory[0xBEEF] = 0b00000100;
    // execute (takes 4 cycles)
    clock_cpu(); clock_cpu();
    clock_cpu(); clock_cpu();
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(memory[0xBEEF], ==, 0b00000100);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    return MUNIT_OK;
}

static MunitResult test_set_bit_3_register() {
    // Set bit 3 in register to 1
    // 0xCBD8, 0xCBD9, 0xCBDA, 0xCBDB, 0xCBDC, 0xCBDD, 0xCBDF
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0xd8,
        0xCB, 0xd8,
        0xCB, 0xd9,
        0xCB, 0xd9,
        0xCB, 0xda,
        0xCB, 0xda,
        0xCB, 0xdb,
        0xCB, 0xdb,
        0xCB, 0xdc,
        0xCB, 0xdc,
        0xCB, 0xdd,
        0xCB, 0xdd,
        0xCB, 0xdf,
        0xCB, 0xdf
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    // Set register B, bit 3
    rf.BC.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(rf.BC.l, ==, 0b00001000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.BC.l = 0b00001000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(rf.BC.l, ==, 0b00001000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Set register C, bit 3
    rf.BC.r = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 7);
    munit_assert_int(rf.BC.r, ==, 0b00001000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.BC.r = 0b00001000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 9);
    munit_assert_int(rf.BC.r, ==, 0b00001000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Set register D, bit 3
    rf.DE.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 11);
    munit_assert_int(rf.DE.l, ==, 0b00001000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.DE.l = 0b00001000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 13);
    munit_assert_int(rf.DE.l, ==, 0b00001000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Set register E, bit 3
    rf.DE.r = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 15);
    munit_assert_int(rf.DE.r, ==, 0b00001000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.DE.r = 0b00001000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 17);
    munit_assert_int(rf.DE.r, ==, 0b00001000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Set register H, bit 3
    rf.HL.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 19);
    munit_assert_int(rf.HL.l, ==, 0b00001000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.HL.l = 0b00001000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 21);
    munit_assert_int(rf.HL.l, ==, 0b00001000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Set register L, bit 3
    rf.HL.r = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 23);
    munit_assert_int(rf.HL.r, ==, 0b00001000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.HL.r = 0b00001000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 25);
    munit_assert_int(rf.HL.r, ==, 0b00001000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Set register A, bit 3
    rf.AF.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 27);
    munit_assert_int(rf.AF.l, ==, 0b00001000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.AF.l = 0b00001000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 29);
    munit_assert_int(rf.AF.l, ==, 0b00001000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    return MUNIT_OK;
}

static MunitResult test_set_bit_3_hl_indirect() {
    // Set bit 3 in mem value at mem address
    // in register HL to 1, 0xCBDE
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0xde,
        0xCB, 0xde,
    };
    memcpy(memory, instructions, sizeof(instructions));

    rf.HL.lr = 0xBEEF;
    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    memory[0xBEEF] = 0b00000000;
    // execute (takes 4 cycles)
    clock_cpu(); clock_cpu();
    clock_cpu(); clock_cpu();
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(memory[0xBEEF], ==, 0b00001000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    memory[0xBEEF] = 0b00001000;
    // execute (takes 4 cycles)
    clock_cpu(); clock_cpu();
    clock_cpu(); clock_cpu();
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(memory[0xBEEF], ==, 0b00001000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    return MUNIT_OK;
}

static MunitResult test_set_bit_4_register() {
    // Set bit 4 in register to 1
    // 0xCBE0, 0xCBE1, 0xCBE2, 0xCBE3, 0xCBE4, 0xCBE5, 0xCBE7
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0xe0,
        0xCB, 0xe0,
        0xCB, 0xe1,
        0xCB, 0xe1,
        0xCB, 0xe2,
        0xCB, 0xe2,
        0xCB, 0xe3,
        0xCB, 0xe3,
        0xCB, 0xe4,
        0xCB, 0xe4,
        0xCB, 0xe5,
        0xCB, 0xe5,
        0xCB, 0xe7,
        0xCB, 0xe7
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    // Set register B, bit 4
    rf.BC.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(rf.BC.l, ==, 0b00010000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.BC.l = 0b00010000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(rf.BC.l, ==, 0b00010000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Set register C, bit 4
    rf.BC.r = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 7);
    munit_assert_int(rf.BC.r, ==, 0b00010000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.BC.r = 0b00010000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 9);
    munit_assert_int(rf.BC.r, ==, 0b00010000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Set register D, bit 4
    rf.DE.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 11);
    munit_assert_int(rf.DE.l, ==, 0b00010000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.DE.l = 0b00010000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 13);
    munit_assert_int(rf.DE.l, ==, 0b00010000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Set register E, bit 4
    rf.DE.r = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 15);
    munit_assert_int(rf.DE.r, ==, 0b00010000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.DE.r = 0b00010000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 17);
    munit_assert_int(rf.DE.r, ==, 0b00010000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Set register H, bit 4
    rf.HL.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 19);
    munit_assert_int(rf.HL.l, ==, 0b00010000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.HL.l = 0b00010000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 21);
    munit_assert_int(rf.HL.l, ==, 0b00010000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Set register L, bit 4
    rf.HL.r = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 23);
    munit_assert_int(rf.HL.r, ==, 0b00010000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.HL.r = 0b00010000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 25);
    munit_assert_int(rf.HL.r, ==, 0b00010000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Set register A, bit 4
    rf.AF.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 27);
    munit_assert_int(rf.AF.l, ==, 0b00010000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.AF.l = 0b00010000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 29);
    munit_assert_int(rf.AF.l, ==, 0b00010000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    return MUNIT_OK;
}

static MunitResult test_set_bit_4_hl_indirect() {
    // Set bit 4 in mem value at mem address
    // in register HL to 1, 0xCBE6
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0xe6,
        0xCB, 0xe6,
    };
    memcpy(memory, instructions, sizeof(instructions));

    rf.HL.lr = 0xBEEF;
    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    memory[0xBEEF] = 0b00000000;
    // execute (takes 4 cycles)
    clock_cpu(); clock_cpu();
    clock_cpu(); clock_cpu();
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(memory[0xBEEF], ==, 0b00010000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    memory[0xBEEF] = 0b00010000;
    // execute (takes 4 cycles)
    clock_cpu(); clock_cpu();
    clock_cpu(); clock_cpu();
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(memory[0xBEEF], ==, 0b00010000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    return MUNIT_OK;
}

static MunitResult test_set_bit_5_register() {
    // Set bit 5 in register to 1
    // 0xCBE8, 0xCBE9, 0xCBEA, 0xCBEB, 0xCBEC, 0xCBED, 0xCBEF
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0xe8,
        0xCB, 0xe8,
        0xCB, 0xe9,
        0xCB, 0xe9,
        0xCB, 0xea,
        0xCB, 0xea,
        0xCB, 0xeb,
        0xCB, 0xeb,
        0xCB, 0xec,
        0xCB, 0xec,
        0xCB, 0xed,
        0xCB, 0xed,
        0xCB, 0xef,
        0xCB, 0xef
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    // Set register B, bit 5
    rf.BC.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(rf.BC.l, ==, 0b00100000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.BC.l = 0b00100000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(rf.BC.l, ==, 0b00100000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Set register C, bit 5
    rf.BC.r = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 7);
    munit_assert_int(rf.BC.r, ==, 0b00100000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.BC.r = 0b00100000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 9);
    munit_assert_int(rf.BC.r, ==, 0b00100000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Set register D, bit 5
    rf.DE.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 11);
    munit_assert_int(rf.DE.l, ==, 0b00100000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.DE.l = 0b00100000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 13);
    munit_assert_int(rf.DE.l, ==, 0b00100000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Set register E, bit 5
    rf.DE.r = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 15);
    munit_assert_int(rf.DE.r, ==, 0b00100000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.DE.r = 0b00100000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 17);
    munit_assert_int(rf.DE.r, ==, 0b00100000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Set register H, bit 5
    rf.HL.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 19);
    munit_assert_int(rf.HL.l, ==, 0b00100000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.HL.l = 0b00100000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 21);
    munit_assert_int(rf.HL.l, ==, 0b00100000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Set register L, bit 5
    rf.HL.r = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 23);
    munit_assert_int(rf.HL.r, ==, 0b00100000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.HL.r = 0b00100000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 25);
    munit_assert_int(rf.HL.r, ==, 0b00100000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Set register A, bit 5
    rf.AF.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 27);
    munit_assert_int(rf.AF.l, ==, 0b00100000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.AF.l = 0b00100000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 29);
    munit_assert_int(rf.AF.l, ==, 0b00100000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    return MUNIT_OK;
}

static MunitResult test_set_bit_5_hl_indirect() {
    // Set bit 5 in mem value at mem address
    // in register HL to 1, 0xCBEE
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0xee,
        0xCB, 0xee,
    };
    memcpy(memory, instructions, sizeof(instructions));

    rf.HL.lr = 0xBEEF;
    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    memory[0xBEEF] = 0b00000000;
    // execute (takes 4 cycles)
    clock_cpu(); clock_cpu();
    clock_cpu(); clock_cpu();
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(memory[0xBEEF], ==, 0b00100000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    memory[0xBEEF] = 0b00100000;
    // execute (takes 4 cycles)
    clock_cpu(); clock_cpu();
    clock_cpu(); clock_cpu();
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(memory[0xBEEF], ==, 0b00100000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    return MUNIT_OK;
}

static MunitResult test_set_bit_6_register() {
    // Set bit 6 in register to 1
    // 0xCBF0, 0xCBF1, 0xCBF2, 0xCBF3, 0xCBF4, 0xCBF5, 0xCBF7
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0xf0,
        0xCB, 0xf0,
        0xCB, 0xf1,
        0xCB, 0xf1,
        0xCB, 0xf2,
        0xCB, 0xf2,
        0xCB, 0xf3,
        0xCB, 0xf3,
        0xCB, 0xf4,
        0xCB, 0xf4,
        0xCB, 0xf5,
        0xCB, 0xf5,
        0xCB, 0xf7,
        0xCB, 0xf7
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    // Set register B, bit 6
    rf.BC.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(rf.BC.l, ==, 0b01000000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.BC.l = 0b01000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(rf.BC.l, ==, 0b01000000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Set register C, bit 6
    rf.BC.r = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 7);
    munit_assert_int(rf.BC.r, ==, 0b01000000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.BC.r = 0b01000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 9);
    munit_assert_int(rf.BC.r, ==, 0b01000000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Set register D, bit 6
    rf.DE.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 11);
    munit_assert_int(rf.DE.l, ==, 0b01000000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.DE.l = 0b01000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 13);
    munit_assert_int(rf.DE.l, ==, 0b01000000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Set register E, bit 6
    rf.DE.r = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 15);
    munit_assert_int(rf.DE.r, ==, 0b01000000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.DE.r = 0b01000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 17);
    munit_assert_int(rf.DE.r, ==, 0b01000000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Set register H, bit 6
    rf.HL.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 19);
    munit_assert_int(rf.HL.l, ==, 0b01000000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.HL.l = 0b01000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 21);
    munit_assert_int(rf.HL.l, ==, 0b01000000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Set register L, bit 6
    rf.HL.r = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 23);
    munit_assert_int(rf.HL.r, ==, 0b01000000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.HL.r = 0b01000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 25);
    munit_assert_int(rf.HL.r, ==, 0b01000000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Set register A, bit 6
    rf.AF.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 27);
    munit_assert_int(rf.AF.l, ==, 0b01000000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.AF.l = 0b01000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 29);
    munit_assert_int(rf.AF.l, ==, 0b01000000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    return MUNIT_OK;
}

static MunitResult test_set_bit_6_hl_indirect() {
    // Set bit 6 in mem value at mem address
    // in register HL to 1, 0xCBF6
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0xf6,
        0xCB, 0xf6,
    };
    memcpy(memory, instructions, sizeof(instructions));

    rf.HL.lr = 0xBEEF;
    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    memory[0xBEEF] = 0b00000000;
    // execute (takes 4 cycles)
    clock_cpu(); clock_cpu();
    clock_cpu(); clock_cpu();
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(memory[0xBEEF], ==, 0b01000000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    memory[0xBEEF] = 0b01000000;
    // execute (takes 4 cycles)
    clock_cpu(); clock_cpu();
    clock_cpu(); clock_cpu();
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(memory[0xBEEF], ==, 0b01000000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    return MUNIT_OK;
}

static MunitResult test_set_bit_7_register() {
    // Set bit 7 in register to 1
    // 0xCBF8, 0xCBF9, 0xCBFA, 0xCBFB, 0xCBFC, 0xCBFD, 0xCBFF
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0xf8,
        0xCB, 0xf8,
        0xCB, 0xf9,
        0xCB, 0xf9,
        0xCB, 0xfa,
        0xCB, 0xfa,
        0xCB, 0xfb,
        0xCB, 0xfb,
        0xCB, 0xfc,
        0xCB, 0xfc,
        0xCB, 0xfd,
        0xCB, 0xfd,
        0xCB, 0xff,
        0xCB, 0xff
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    // Set register B, bit 7
    rf.BC.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(rf.BC.l, ==, 0b10000000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.BC.l = 0b10000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(rf.BC.l, ==, 0b10000000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Set register C, bit 7
    rf.BC.r = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 7);
    munit_assert_int(rf.BC.r, ==, 0b10000000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.BC.r = 0b10000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 9);
    munit_assert_int(rf.BC.r, ==, 0b10000000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Set register D, bit 7
    rf.DE.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 11);
    munit_assert_int(rf.DE.l, ==, 0b10000000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.DE.l = 0b10000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 13);
    munit_assert_int(rf.DE.l, ==, 0b10000000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Set register E, bit 7
    rf.DE.r = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 15);
    munit_assert_int(rf.DE.r, ==, 0b10000000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.DE.r = 0b10000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 17);
    munit_assert_int(rf.DE.r, ==, 0b10000000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Set register H, bit 7
    rf.HL.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 19);
    munit_assert_int(rf.HL.l, ==, 0b10000000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.HL.l = 0b10000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 21);
    munit_assert_int(rf.HL.l, ==, 0b10000000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Set register L, bit 7
    rf.HL.r = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 23);
    munit_assert_int(rf.HL.r, ==, 0b10000000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.HL.r = 0b10000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 25);
    munit_assert_int(rf.HL.r, ==, 0b10000000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    // Set register A, bit 7
    rf.AF.l = 0b00000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 27);
    munit_assert_int(rf.AF.l, ==, 0b10000000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.AF.l = 0b10000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 29);
    munit_assert_int(rf.AF.l, ==, 0b10000000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    return MUNIT_OK;
}

static MunitResult test_set_bit_7_hl_indirect() {
    // Set bit 7 in mem value at mem address
    // in register HL to 1, 0xCBFE
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0xfe,
        0xCB, 0xfe,
    };
    memcpy(memory, instructions, sizeof(instructions));

    rf.HL.lr = 0xBEEF;
    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    memory[0xBEEF] = 0b00000000;
    // execute (takes 4 cycles)
    clock_cpu(); clock_cpu();
    clock_cpu(); clock_cpu();
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(memory[0xBEEF], ==, 0b10000000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    memory[0xBEEF] = 0b10000000;
    // execute (takes 4 cycles)
    clock_cpu(); clock_cpu();
    clock_cpu(); clock_cpu();
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(memory[0xBEEF], ==, 0b10000000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    return MUNIT_OK;
}

MunitTest bitwise_tests[] = {
    {
        "/and_register",
        test_and_register,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/and_flags",
        test_and_flags,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/and_with_hl_indirect",
        test_and_with_hl_indirect,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/and_immediate",
        test_and_immediate,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/or_register",
        test_or_register,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/or_flags",
        test_or_flags,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/or_with_hl_indirect",
        test_or_with_hl_indirect,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/or_immediate",
        test_or_immediate,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/xor_register",
        test_xor_register,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/xor_with_hl_indirect",
        test_xor_with_hl_indirect,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/xor_immediate",
        test_xor_immediate,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/complement_carry_flag",
        test_complement_carry_flag,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/set_carry_flag",
        test_set_carry_flag,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/decimal_adjust_accumulator",
        test_decimal_adjust_accumulator,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/complement_accumulator",
        test_complement_accumulator,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/test_bit_test_0_register",
        test_bit_test_0_register,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/test_bit_test_0_hl_indirect",
        test_bit_test_0_hl_indirect,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/test_bit_test_1_register",
        test_bit_test_1_register,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/test_bit_test_1_hl_indirect",
        test_bit_test_1_hl_indirect,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/test_bit_test_2_register",
        test_bit_test_2_register,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/test_bit_test_2_hl_indirect",
        test_bit_test_2_hl_indirect,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/test_bit_test_3_register",
        test_bit_test_3_register,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/test_bit_test_3_hl_indirect",
        test_bit_test_3_hl_indirect,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/test_bit_test_4_register",
        test_bit_test_4_register,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/test_bit_test_4_hl_indirect",
        test_bit_test_4_hl_indirect,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/test_bit_test_5_register",
        test_bit_test_5_register,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/test_bit_test_5_hl_indirect",
        test_bit_test_5_hl_indirect,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/test_bit_test_6_register",
        test_bit_test_6_register,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/test_bit_test_6_hl_indirect",
        test_bit_test_6_hl_indirect,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/test_bit_test_7_register",
        test_bit_test_7_register,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/test_bit_test_7_hl_indirect",
        test_bit_test_7_hl_indirect,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/test_reset_bit_0_register",
        test_reset_bit_0_register,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/test_reset_bit_0_hl_indirect",
        test_reset_bit_0_hl_indirect,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/test_reset_bit_1_register",
        test_reset_bit_1_register,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/test_reset_bit_1_hl_indirect",
        test_reset_bit_1_hl_indirect,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/test_reset_bit_2_register",
        test_reset_bit_2_register,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/test_reset_bit_2_hl_indirect",
        test_reset_bit_2_hl_indirect,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/test_reset_bit_3_register",
        test_reset_bit_3_register,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/test_reset_bit_3_hl_indirect",
        test_reset_bit_3_hl_indirect,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/test_reset_bit_4_register",
        test_reset_bit_4_register,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/test_reset_bit_4_hl_indirect",
        test_reset_bit_4_hl_indirect,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/test_reset_bit_5_register",
        test_reset_bit_5_register,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/test_reset_bit_5_hl_indirect",
        test_reset_bit_5_hl_indirect,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/test_reset_bit_6_register",
        test_reset_bit_6_register,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/test_reset_bit_6_hl_indirect",
        test_reset_bit_6_hl_indirect,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/test_reset_bit_7_register",
        test_reset_bit_7_register,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/test_set_bit_0_register",
        test_set_bit_0_register,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/test_set_bit_0_hl_indirect",
        test_set_bit_0_hl_indirect,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/test_set_bit_1_register",
        test_set_bit_1_register,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/test_set_bit_1_hl_indirect",
        test_set_bit_1_hl_indirect,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/test_set_bit_2_register",
        test_set_bit_2_register,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/test_set_bit_2_hl_indirect",
        test_set_bit_2_hl_indirect,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/test_set_bit_3_register",
        test_set_bit_3_register,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/test_set_bit_3_hl_indirect",
        test_set_bit_3_hl_indirect,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/test_set_bit_4_register",
        test_set_bit_4_register,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/test_set_bit_4_hl_indirect",
        test_set_bit_4_hl_indirect,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/test_set_bit_5_register",
        test_set_bit_5_register,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/test_set_bit_5_hl_indirect",
        test_set_bit_5_hl_indirect,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/test_set_bit_6_register",
        test_set_bit_6_register,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/test_set_bit_6_hl_indirect",
        test_set_bit_6_hl_indirect,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/test_set_bit_7_register",
        test_set_bit_7_register,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/test_set_bit_7_hl_indirect",
        test_set_bit_7_hl_indirect,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

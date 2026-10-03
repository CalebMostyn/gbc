#include "munit.h"
#include "helpers.h"
#include "cpu.h"

static MunitResult test_jump_immediate() {
    // Set PC to 16-bit address in two immediate bytes, 0xC3
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {0xC3, 0xEF, 0xBE};
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    // execute (takes 4 cycles)
    clock_cpu(); clock_cpu();
    clock_cpu(); clock_cpu();
    // Immediate address + 1 as the next instruction is fetched
    munit_assert_int(rf.PC, ==, 0xBEF0);

    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    return MUNIT_OK;
}

static MunitResult test_jump_hl() {
    // Set PC to value in HL register, 0xE9
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {0xE9};
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    rf.HL.lr = 0xBEEF;
    clock_cpu();
    // HL + 1 as the next instruction is fetched
    munit_assert_int(rf.PC, ==, 0xBEF0);

    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    return MUNIT_OK;
}

static MunitResult test_jump_not_zero() {
    // Set PC to 16-bit address in two immediate bytes
    // if zero flag is false, 0xC2
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xC2, 0x42, 0x42,
        0xC2, 0xEF, 0xBE
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    rf.AF.r = 0b10000000; // set zero flag
    // execute (takes 3 cycles if condition is false)
    clock_cpu(); clock_cpu(); clock_cpu();
    munit_assert_int(rf.PC, ==, 4); // PC += 3 from immediates
    rf.AF.r = 0; // unset zero flag
    // execute (takes 4 cycles if condition is true)
    clock_cpu(); clock_cpu();
    clock_cpu(); clock_cpu();
    // Immediate address + 1 as the next instruction is fetched
    munit_assert_int(rf.PC, ==, 0xBEF0);

    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    return MUNIT_OK;
}

static MunitResult test_jump_zero() {
    // Set PC to 16-bit address in two immediate bytes
    // if zero flag is true, 0xCA
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCA, 0x42, 0x42,
        0xCA, 0xEF, 0xBE
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    // execute (takes 3 cycles if condition is false)
    clock_cpu(); clock_cpu(); clock_cpu();
    munit_assert_int(rf.PC, ==, 4); // PC += 3 from immediates
    rf.AF.r = 0b10000000; // set zero flag
    // execute (takes 4 cycles if condition is true)
    clock_cpu(); clock_cpu();
    clock_cpu(); clock_cpu();
    // Immediate address + 1 as the next instruction is fetched
    munit_assert_int(rf.PC, ==, 0xBEF0);

    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0b10000000);
    return MUNIT_OK;
}

static MunitResult test_jump_not_carry() {
    // Set PC to 16-bit address in two immediate bytes
    // if carry flag is false, 0xD2
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xD2, 0x42, 0x42,
        0xD2, 0xEF, 0xBE
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    rf.AF.r = 0b00010000; // set carry flag
    // execute (takes 3 cycles if condition is false)
    clock_cpu(); clock_cpu(); clock_cpu();
    munit_assert_int(rf.PC, ==, 4); // PC += 3 from immediates
    rf.AF.r = 0; // unset carry flag
    // execute (takes 4 cycles if condition is true)
    clock_cpu(); clock_cpu();
    clock_cpu(); clock_cpu();
    // Immediate address + 1 as the next instruction is fetched
    munit_assert_int(rf.PC, ==, 0xBEF0);

    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    return MUNIT_OK;
}

static MunitResult test_jump_carry() {
    // Set PC to 16-bit address in two immediate bytes
    // if carry flag is true, 0xDA
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xDA, 0x42, 0x42,
        0xDA, 0xEF, 0xBE
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    // execute (takes 3 cycles if condition is false)
    clock_cpu(); clock_cpu(); clock_cpu();
    munit_assert_int(rf.PC, ==, 4); // PC += 3 from immediates
    rf.AF.r = 0b00010000; // set carry flag
    // execute (takes 4 cycles if condition is true)
    clock_cpu(); clock_cpu();
    clock_cpu(); clock_cpu();
    // Immediate address + 1 as the next instruction is fetched
    munit_assert_int(rf.PC, ==, 0xBEF0);

    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0b00010000);
    return MUNIT_OK;
}

static MunitResult test_jump_relative() {
    // Set PC to PC + signed immediate byte, 0x18
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0x18, 0x01, 0x00, // NOP as it will be jumped over
        0x18, 0xFF
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    // execute (takes 3 cycles)
    clock_cpu(); clock_cpu(); clock_cpu();
    // PC += 3 from immediate and relative jump
    munit_assert_int(rf.PC, ==, 4);
    // execute (takes 3 cycles)
    clock_cpu(); clock_cpu(); clock_cpu();
    // PC +2 from immediate, -1 from relative jump,
    munit_assert_int(rf.PC, ==, 5);

    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    return MUNIT_OK;
}

static MunitResult test_jump_not_zero_relative() {
    // Set PC to PC + signed immediate byte
    // if zero flag is false, 0x20
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0x20, 0x01,
        0x20, 0x01,
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    rf.AF.r = 0b10000000; // set zero flag
    // execute (takes 2 cycles if condition is false)
    clock_cpu(); clock_cpu();
    munit_assert_int(rf.PC, ==, 3); // PC += 2 from immediate
    rf.AF.r = 0; // unset zero flag
    // execute (takes 3 cycles if condition is true)
    clock_cpu(); clock_cpu(); clock_cpu();
    // PC += 3 from immediate and relative jump
    munit_assert_int(rf.PC, ==, 6);

    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    return MUNIT_OK;
}

static MunitResult test_jump_zero_relative() {
    // Set PC to PC + signed immediate byte
    // if zero flag is true, 0x28
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0x28, 0x01,
        0x28, 0x01,
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    // execute (takes 2 cycles if condition is false)
    clock_cpu(); clock_cpu();
    munit_assert_int(rf.PC, ==, 3); // PC += 2 from immediate
    rf.AF.r = 0b10000000; // set zero flag
    // execute (takes 3 cycles if condition is true)
    clock_cpu(); clock_cpu(); clock_cpu();
    // PC += 3 from immediate and relative jump
    munit_assert_int(rf.PC, ==, 6);

    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0b10000000);
    return MUNIT_OK;
}

static MunitResult test_jump_not_carry_relative() {
    // Set PC to PC + signed immediate byte
    // if carry flag is false, 0x30
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0x30, 0x01,
        0x30, 0x01,
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    rf.AF.r = 0b00010000; // set carry flag
    // execute (takes 2 cycles if condition is false)
    clock_cpu(); clock_cpu();
    munit_assert_int(rf.PC, ==, 3); // PC += 2 from immediate
    rf.AF.r = 0; // unset carry flag
    // execute (takes 3 cycles if condition is true)
    clock_cpu(); clock_cpu(); clock_cpu();
    // PC += 3 from immediate and relative jump
    munit_assert_int(rf.PC, ==, 6);

    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    return MUNIT_OK;
}

static MunitResult test_jump_carry_relative() {
    // Set PC to PC + signed immediate byte
    // if carry flag is true, 0x38
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0x38, 0x01,
        0x38, 0x01,
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    // execute (takes 2 cycles if condition is false)
    clock_cpu(); clock_cpu();
    munit_assert_int(rf.PC, ==, 3); // PC += 2 from immediate
    rf.AF.r = 0b00010000; // set carry flag
    // execute (takes 3 cycles if condition is true)
    clock_cpu(); clock_cpu(); clock_cpu();
    // PC += 3 from immediate and relative jump
    munit_assert_int(rf.PC, ==, 6);

    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0b00010000);
    return MUNIT_OK;
}

static MunitResult test_call() {
    // Push PC to stack and then set PC
    // to 16-bit address in two immediate bytes, 0xCD
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {0xCD, 0xEF, 0xBE};
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    rf.SP = 0xDEAD;
    // execute (takes 6 cycles)
    clock_cpu(); clock_cpu(); clock_cpu();
    clock_cpu(); clock_cpu(); clock_cpu();
    // Immediate address + 1 as the next instruction is fetched
    munit_assert_int(rf.PC, ==, 0xBEF0);
    munit_assert_int(rf.SP, ==, 0xDEAB); // SP -= 2
    // PC prior to jump pushed to stack (3)
    munit_assert_int(memory[0xDEAC], ==, 0x00); // msbyte
    munit_assert_int(memory[0xDEAB], ==, 0x03); // lsbyte


    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    return MUNIT_OK;
}

static MunitResult test_call_not_zero() {
    // Push PC to stack and then set PC
    // to 16-bit address in two immediate bytes
    // if zero flag is false, 0xC4
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xC4, 0x42, 0x42,
        0xC4, 0xEF, 0xBE
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    rf.SP = 0xDEAD;
    rf.AF.r = 0b10000000; // set zero flag
    // execute (takes 3 cycles if condition is false)
    clock_cpu(); clock_cpu(); clock_cpu();
    munit_assert_int(rf.PC, ==, 4); // PC += 3 from immediates
    munit_assert_int(rf.SP, ==, 0xDEAD); // unmodified
    rf.AF.r = 0; // unset zero flag
    // execute (takes 6 cycles if condition is true)
    clock_cpu(); clock_cpu(); clock_cpu();
    clock_cpu(); clock_cpu(); clock_cpu();
    // Immediate address + 1 as the next instruction is fetched
    munit_assert_int(rf.PC, ==, 0xBEF0);
    munit_assert_int(rf.SP, ==, 0xDEAB); // SP -= 2
    // PC prior to jump pushed to stack (6)
    munit_assert_int(memory[0xDEAC], ==, 0x00); // msbyte
    munit_assert_int(memory[0xDEAB], ==, 0x06); // lsbyte

    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    return MUNIT_OK;
}

static MunitResult test_call_zero() {
    // Push PC to stack and then set PC
    // to 16-bit address in two immediate bytes
    // if zero flag is true, 0xCC
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCC, 0x42, 0x42,
        0xCC, 0xEF, 0xBE
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    rf.SP = 0xDEAD;
    // execute (takes 3 cycles if condition is false)
    clock_cpu(); clock_cpu(); clock_cpu();
    munit_assert_int(rf.PC, ==, 4); // PC += 3 from immediates
    munit_assert_int(rf.SP, ==, 0xDEAD); // unmodified
    rf.AF.r = 0b10000000; // set zero flag
    // execute (takes 6 cycles if condition is true)
    clock_cpu(); clock_cpu(); clock_cpu();
    clock_cpu(); clock_cpu(); clock_cpu();
    // Immediate address + 1 as the next instruction is fetched
    munit_assert_int(rf.PC, ==, 0xBEF0);
    munit_assert_int(rf.SP, ==, 0xDEAB); // SP -= 2
    // PC prior to jump pushed to stack (6)
    munit_assert_int(memory[0xDEAC], ==, 0x00); // msbyte
    munit_assert_int(memory[0xDEAB], ==, 0x06); // lsbyte

    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0b10000000);
    return MUNIT_OK;
}

static MunitResult test_call_not_carry() {
    // Push PC to stack and then set PC
    // to 16-bit address in two immediate bytes
    // if carry flag is false, 0xD4
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xD4, 0x42, 0x42,
        0xD4, 0xEF, 0xBE
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    rf.SP = 0xDEAD;
    rf.AF.r = 0b00010000; // set carry flag
    // execute (takes 3 cycles if condition is false)
    clock_cpu(); clock_cpu(); clock_cpu();
    munit_assert_int(rf.PC, ==, 4); // PC += 3 from immediates
    munit_assert_int(rf.SP, ==, 0xDEAD); // unmodified
    rf.AF.r = 0; // unset carry flag
    // execute (takes 6 cycles if condition is true)
    clock_cpu(); clock_cpu(); clock_cpu();
    clock_cpu(); clock_cpu(); clock_cpu();
    // Immediate address + 1 as the next instruction is fetched
    munit_assert_int(rf.PC, ==, 0xBEF0);
    munit_assert_int(rf.SP, ==, 0xDEAB); // SP -= 2
    // PC prior to jump pushed to stack (6)
    munit_assert_int(memory[0xDEAC], ==, 0x00); // msbyte
    munit_assert_int(memory[0xDEAB], ==, 0x06); // lsbyte

    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    return MUNIT_OK;
}

static MunitResult test_call_carry() {
    // Push PC to stack and then set PC
    // to 16-bit address in two immediate bytes
    // if carry flag is true, 0xDC
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xDC, 0x42, 0x42,
        0xDC, 0xEF, 0xBE
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    rf.SP = 0xDEAD;
    // execute (takes 3 cycles if condition is false)
    clock_cpu(); clock_cpu(); clock_cpu();
    munit_assert_int(rf.PC, ==, 4); // PC += 3 from immediates
    munit_assert_int(rf.SP, ==, 0xDEAD); // unmodified
    rf.AF.r = 0b00010000; // set carry flag
    // execute (takes 6 cycles if condition is true)
    clock_cpu(); clock_cpu(); clock_cpu();
    clock_cpu(); clock_cpu(); clock_cpu();
    // Immediate address + 1 as the next instruction is fetched
    munit_assert_int(rf.PC, ==, 0xBEF0);
    munit_assert_int(rf.SP, ==, 0xDEAB); // SP -= 2
    // PC prior to jump pushed to stack (6)
    munit_assert_int(memory[0xDEAC], ==, 0x00); // msbyte
    munit_assert_int(memory[0xDEAB], ==, 0x06); // lsbyte

    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0b00010000);
    return MUNIT_OK;
}

static MunitResult test_return() {
    // Pop value off stack and set PC to that address, 0xC9
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {0xC9};
    memcpy(memory, instructions, sizeof(instructions));

    memory[0xDEAD] = 0xEF; memory[0xDEAE] = 0xBE;
    rf.SP = 0xDEAD;

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    // execute (takes 4 cycles)
    clock_cpu(); clock_cpu();
    clock_cpu(); clock_cpu();
    // Popped value + 1 as the next instruction is fetched
    munit_assert_int(rf.PC, ==, 0xBEF0);
    munit_assert_int(rf.SP, ==, 0xDEAF); // SP += 2

    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    return MUNIT_OK;
}

static MunitResult test_return_not_zero() {
    // Pop value off stack and set PC to that address
    // if zero flag is false, 0xC0
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {0xC0, 0xC0};
    memcpy(memory, instructions, sizeof(instructions));

    memory[0xDEAD] = 0xEF; memory[0xDEAE] = 0xBE;
    rf.SP = 0xDEAD;

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    rf.AF.r = 0b10000000; // set zero flag
    // execute (takes 2 cycles if condition is false)
    clock_cpu(); clock_cpu();
    munit_assert_int(rf.PC, ==, 2);
    rf.AF.r = 0; // unset zero flag
    // execute (takes 5 cycles if condition is true)
    clock_cpu(); clock_cpu();
    clock_cpu(); clock_cpu(); clock_cpu();
    // Popped value + 1 as the next instruction is fetched
    munit_assert_int(rf.PC, ==, 0xBEF0);
    munit_assert_int(rf.SP, ==, 0xDEAF); // SP += 2

    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    return MUNIT_OK;
}

static MunitResult test_return_zero() {
    // Pop value off stack and set PC to that address
    // if zero flag is true, 0xC8
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {0xC8, 0xC8};
    memcpy(memory, instructions, sizeof(instructions));

    memory[0xDEAD] = 0xEF; memory[0xDEAE] = 0xBE;
    rf.SP = 0xDEAD;

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    // execute (takes 2 cycles if condition is false)
    clock_cpu(); clock_cpu();
    munit_assert_int(rf.PC, ==, 2);
    rf.AF.r = 0b10000000; // set zero flag
    // execute (takes 5 cycles if condition is true)
    clock_cpu(); clock_cpu();
    clock_cpu(); clock_cpu(); clock_cpu();
    // Popped value + 1 as the next instruction is fetched
    munit_assert_int(rf.PC, ==, 0xBEF0);
    munit_assert_int(rf.SP, ==, 0xDEAF); // SP += 2

    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0b10000000);
    return MUNIT_OK;
}

static MunitResult test_return_not_carry() {
    // Pop value off stack and set PC to that address
    // if carry flag is false, 0xD0
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {0xD0, 0xD0};
    memcpy(memory, instructions, sizeof(instructions));

    memory[0xDEAD] = 0xEF; memory[0xDEAE] = 0xBE;
    rf.SP = 0xDEAD;

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    rf.AF.r = 0b00010000; // set zero flag
    // execute (takes 2 cycles if condition is false)
    clock_cpu(); clock_cpu();
    munit_assert_int(rf.PC, ==, 2);
    rf.AF.r = 0; // unset carry flag
    // execute (takes 5 cycles if condition is true)
    clock_cpu(); clock_cpu();
    clock_cpu(); clock_cpu(); clock_cpu();
    // Popped value + 1 as the next instruction is fetched
    munit_assert_int(rf.PC, ==, 0xBEF0);
    munit_assert_int(rf.SP, ==, 0xDEAF); // SP += 2

    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    return MUNIT_OK;
}

static MunitResult test_return_carry() {
    // Pop value off stack and set PC to that address
    // if carry flag is true, 0xD8
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {0xD8, 0xD8};
    memcpy(memory, instructions, sizeof(instructions));

    memory[0xDEAD] = 0xEF; memory[0xDEAE] = 0xBE;
    rf.SP = 0xDEAD;

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    // execute (takes 2 cycles if condition is false)
    clock_cpu(); clock_cpu();
    munit_assert_int(rf.PC, ==, 2);
    rf.AF.r = 0b00010000; // set zero flag
    // execute (takes 5 cycles if condition is true)
    clock_cpu(); clock_cpu();
    clock_cpu(); clock_cpu(); clock_cpu();
    // Popped value + 1 as the next instruction is fetched
    munit_assert_int(rf.PC, ==, 0xBEF0);
    munit_assert_int(rf.SP, ==, 0xDEAF); // SP += 2

    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0b00010000);
    return MUNIT_OK;
}

static MunitResult test_return_enable_interrupt() {
    // Pop value off stack, set PC to that address,
    // and set the interrupt master enable to true, 0xD9
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {0xD9};
    memcpy(memory, instructions, sizeof(instructions));

    memory[0xDEAD] = 0xEF; memory[0xDEAE] = 0xBE;
    rf.SP = 0xDEAD;

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    // execute (takes 4 cycles)
    clock_cpu(); clock_cpu();
    clock_cpu(); clock_cpu();
    // Popped value + 1 as the next instruction is fetched
    munit_assert_int(rf.PC, ==, 0xBEF0);
    munit_assert_int(rf.SP, ==, 0xDEAF); // SP += 2
    munit_assert_true(rf.IME); // interrupts enabled

    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    return MUNIT_OK;
}

static MunitResult test_reset_00() {
    // Push PC to stack and then set PC to 0x00, 0xC7
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {0xC7};
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);

    // RST 0x00
    rf.SP = 0xDEAD;
    // execute (takes 4 cycles)
    clock_cpu(); clock_cpu();
    clock_cpu(); clock_cpu();
    // Reset address + 1 as the next instruction is fetched
    munit_assert_int(rf.PC, ==, 0x01);
    munit_assert_int(rf.SP, ==, 0xDEAB); // SP -= 2
    // PC prior to jump pushed to stack (1)
    munit_assert_int(memory[0xDEAC], ==, 0x00); // msbyte
    munit_assert_int(memory[0xDEAB], ==, 0x01); // lsbyte

    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    return MUNIT_OK;
}

static MunitResult test_reset_08() {
    // Push PC to stack and then set PC to 0x08, 0xCF
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {0xCF};
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);

    // RST 0x08
    rf.SP = 0xDEAD;
    // execute (takes 4 cycles)
    clock_cpu(); clock_cpu();
    clock_cpu(); clock_cpu();
    // Reset address + 1 as the next instruction is fetched
    munit_assert_int(rf.PC, ==, 0x09);
    munit_assert_int(rf.SP, ==, 0xDEAB); // SP -= 2
    // PC prior to jump pushed to stack (1)
    munit_assert_int(memory[0xDEAC], ==, 0x00); // msbyte
    munit_assert_int(memory[0xDEAB], ==, 0x01); // lsbyte

    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    return MUNIT_OK;
}

static MunitResult test_reset_10() {
    // Push PC to stack and then set PC to 0x10, 0xD7
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {0xD7};
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);

    // RST 0x10
    rf.SP = 0xDEAD;
    // execute (takes 4 cycles)
    clock_cpu(); clock_cpu();
    clock_cpu(); clock_cpu();
    // Reset address + 1 as the next instruction is fetched
    munit_assert_int(rf.PC, ==, 0x11);
    munit_assert_int(rf.SP, ==, 0xDEAB); // SP -= 2
    // PC prior to jump pushed to stack (1)
    munit_assert_int(memory[0xDEAC], ==, 0x00); // msbyte
    munit_assert_int(memory[0xDEAB], ==, 0x01); // lsbyte

    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    return MUNIT_OK;
}

static MunitResult test_reset_18() {
    // Push PC to stack and then set PC to 0x18, 0xDF
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {0xDF};
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);

    // RST 0x18
    rf.SP = 0xDEAD;
    // execute (takes 4 cycles)
    clock_cpu(); clock_cpu();
    clock_cpu(); clock_cpu();
    // Reset address + 1 as the next instruction is fetched
    munit_assert_int(rf.PC, ==, 0x19);
    munit_assert_int(rf.SP, ==, 0xDEAB); // SP -= 2
    // PC prior to jump pushed to stack (1)
    munit_assert_int(memory[0xDEAC], ==, 0x00); // msbyte
    munit_assert_int(memory[0xDEAB], ==, 0x01); // lsbyte

    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    return MUNIT_OK;
}

static MunitResult test_reset_20() {
    // Push PC to stack and then set PC to 0x20, 0xE7
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {0xE7};
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);

    // RST 0x20
    rf.SP = 0xDEAD;
    // execute (takes 4 cycles)
    clock_cpu(); clock_cpu();
    clock_cpu(); clock_cpu();
    // Reset address + 1 as the next instruction is fetched
    munit_assert_int(rf.PC, ==, 0x21);
    munit_assert_int(rf.SP, ==, 0xDEAB); // SP -= 2
    // PC prior to jump pushed to stack (1)
    munit_assert_int(memory[0xDEAC], ==, 0x00); // msbyte
    munit_assert_int(memory[0xDEAB], ==, 0x01); // lsbyte

    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    return MUNIT_OK;
}

static MunitResult test_reset_28() {
    // Push PC to stack and then set PC to 0x28, 0xEF
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {0xEF};
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);

    // RST 0x28
    rf.SP = 0xDEAD;
    // execute (takes 4 cycles)
    clock_cpu(); clock_cpu();
    clock_cpu(); clock_cpu();
    // Reset address + 1 as the next instruction is fetched
    munit_assert_int(rf.PC, ==, 0x29);
    munit_assert_int(rf.SP, ==, 0xDEAB); // SP -= 2
    // PC prior to jump pushed to stack (1)
    munit_assert_int(memory[0xDEAC], ==, 0x00); // msbyte
    munit_assert_int(memory[0xDEAB], ==, 0x01); // lsbyte

    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    return MUNIT_OK;
}

static MunitResult test_reset_30() {
    // Push PC to stack and then set PC to 0x30, 0xF7
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {0xF7};
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);

    // RST 0x30
    rf.SP = 0xDEAD;
    // execute (takes 4 cycles)
    clock_cpu(); clock_cpu();
    clock_cpu(); clock_cpu();
    // Reset address + 1 as the next instruction is fetched
    munit_assert_int(rf.PC, ==, 0x31);
    munit_assert_int(rf.SP, ==, 0xDEAB); // SP -= 2
    // PC prior to jump pushed to stack (1)
    munit_assert_int(memory[0xDEAC], ==, 0x00); // msbyte
    munit_assert_int(memory[0xDEAB], ==, 0x01); // lsbyte

    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    return MUNIT_OK;
}

static MunitResult test_reset_38() {
    // Push PC to stack and then set PC to 0x38, 0xFF
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {0xFF};
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);

    // RST 0x38
    rf.SP = 0xDEAD;
    // execute (takes 4 cycles)
    clock_cpu(); clock_cpu();
    clock_cpu(); clock_cpu();
    // Reset address + 1 as the next instruction is fetched
    munit_assert_int(rf.PC, ==, 0x39);
    munit_assert_int(rf.SP, ==, 0xDEAB); // SP -= 2
    // PC prior to jump pushed to stack (1)
    munit_assert_int(memory[0xDEAC], ==, 0x00); // msbyte
    munit_assert_int(memory[0xDEAB], ==, 0x01); // lsbyte

    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    return MUNIT_OK;
}

MunitTest control_flow_tests[] = {
    {
        "/jump_immediate",
        test_jump_immediate,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/jump_hl",
        test_jump_hl,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/jump_not_zero",
        test_jump_not_zero,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/jump_zero",
        test_jump_zero,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/jump_not_carry",
        test_jump_not_carry,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/jump_carry",
        test_jump_carry,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/jump_relative",
        test_jump_relative,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/jump_not_zero_relative",
        test_jump_not_zero_relative,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/jump_zero_relative",
        test_jump_zero_relative,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/jump_not_carry_relative",
        test_jump_not_carry_relative,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/jump_carry_relative",
        test_jump_carry_relative,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/call",
        test_call,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/call_not_zero",
        test_call_not_zero,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/call_zero",
        test_call_zero,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/call_not_carry",
        test_call_not_carry,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/call_carry",
        test_call_carry,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/return",
        test_return,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/return_not_zero",
        test_return_not_zero,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/return_zero",
        test_return_zero,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/return_not_carry",
        test_return_not_carry,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/return_carry",
        test_return_carry,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/return_enable_interrupt",
        test_return_enable_interrupt,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/reset_00",
        test_reset_00,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/reset_08",
        test_reset_08,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/reset_10",
        test_reset_10,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/reset_18",
        test_reset_18,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/reset_20",
        test_reset_20,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/reset_28",
        test_reset_28,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/reset_30",
        test_reset_30,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/reset_38",
        test_reset_38,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

#include "munit.h"
#include "helpers.h"
#include "cpu.h"

static MunitResult test_sub_register() {
    // Subtract register from register A and store in register A
    // 0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x97
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x97};
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    // A - B
    rf.BC.l = 0x01;
    rf.AF.l = 0x42;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 2);
    munit_assert_int(rf.AF.l, ==, 0x41);
    // Subtraction flag set
    munit_assert_int(rf.AF.r, ==, 0b01000000);
    // A - C
    rf.BC.r = 0x01;
    rf.AF.l = 0x43;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(rf.AF.l, ==, 0x42);
    // Subtraction flag set
    munit_assert_int(rf.AF.r, ==, 0b01000000);
    // A - D
    rf.DE.l = 0x01;
    rf.AF.l = 0x44;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 4);
    munit_assert_int(rf.AF.l, ==, 0x43);
    // Subtraction flag set
    munit_assert_int(rf.AF.r, ==, 0b01000000);
    // A - E
    rf.DE.r = 0x01;
    rf.AF.l = 0x45;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(rf.AF.l, ==, 0x44);
    // Subtraction flag set
    munit_assert_int(rf.AF.r, ==, 0b01000000);
    // A - H
    rf.HL.l = 0x01;
    rf.AF.l = 0x46;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 6);
    munit_assert_int(rf.AF.l, ==, 0x45);
    // Subtraction flag set
    munit_assert_int(rf.AF.r, ==, 0b01000000);
    // A - L
    rf.HL.r = 0x01;
    rf.AF.l = 0x47;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 7);
    munit_assert_int(rf.AF.l, ==, 0x46);
    // Subtraction flag set
    munit_assert_int(rf.AF.r, ==, 0b01000000);
    // A - A
    clock_cpu();
    munit_assert_int(rf.PC, ==, 8);
    munit_assert_int(rf.AF.l, ==, 0x00);
    // Subtraction and zero flags set
    munit_assert_int(rf.AF.r, ==, 0b11000000);

    return MUNIT_OK;
}

static MunitResult test_sub_flags() {
    // Sub register A with register and store in register A
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {0x90, 0x90};
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    rf.AF.l = 0xA1;
    rf.BC.l = 0x0F;
    clock_cpu(); // execute
    munit_assert_int(rf.PC, ==, 2);
    munit_assert_int(rf.AF.l, ==, 0x92);
    // Subtraction and half carry due to underflow on nibble
    munit_assert_int(rf.AF.r, ==, 0b01100000);

    rf.BC.l = 0x10;
    rf.AF.l = 0x0F;
    clock_cpu(); // execute
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(rf.AF.l, ==, 0xFF);
    // Subtraction and carry due to underflow on entire subtraction
    munit_assert_int(rf.AF.r, ==, 0b01010000);

    return MUNIT_OK;
}

static MunitResult test_sub_from_hl_indirect() {
    // Subtract value from mem address in HL register from
    // register A and store in register A, 0x96
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {0x96};
    memcpy(memory, instructions, sizeof(instructions));

    // load test value in memory, set HL to test memory addr
    memory[0xBEEF] = 0x01;
    rf.HL.lr = 0xBEEF;
    rf.AF.l = 0x42;
    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 2);
    munit_assert_int(rf.AF.l, ==, 0x41);

    // Subtraction flag set
    munit_assert_int(rf.AF.r, ==, 0b01000000);
    return MUNIT_OK;
}

static MunitResult test_sub_immediate() {
    // Subtract immediate from register A and store in register A, 0xD6
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {0xD6, 0x01};
    memcpy(memory, instructions, sizeof(instructions));

    rf.AF.l = 0x42;
    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3); // PC += 2 from immediate
    munit_assert_int(rf.AF.l, ==, 0x41);

    // Subtraction flag set
    munit_assert_int(rf.AF.r, ==, 0b01000000);
    return MUNIT_OK;
}

static MunitResult test_sub_with_carry() {
    // Subtract register and carry from register A
    // and store in register A
    // 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9F
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9F};
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    // A - B
    rf.BC.l = 0x01;
    rf.AF.l = 0x42;
    rf.AF.r = 0x10; // set carry flag
    clock_cpu();
    munit_assert_int(rf.PC, ==, 2);
    munit_assert_int(rf.AF.l, ==, 0x40);
    // Subtraction flag set
    munit_assert_int(rf.AF.r, ==, 0b01000000);
    // A - C
    rf.BC.r = 0x01;
    rf.AF.l = 0x43;
    rf.AF.r = 0x00; // unset carry flag
    clock_cpu();
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(rf.AF.l, ==, 0x42);
    // Subtraction flag set
    munit_assert_int(rf.AF.r, ==, 0b01000000);
    // A - D
    rf.DE.l = 0x01;
    rf.AF.l = 0x44;
    rf.AF.r = 0x10; // set carry flag
    clock_cpu();
    munit_assert_int(rf.PC, ==, 4);
    munit_assert_int(rf.AF.l, ==, 0x42);
    // Subtraction flag set
    munit_assert_int(rf.AF.r, ==, 0b01000000);
    // A - E
    rf.DE.r = 0x01;
    rf.AF.l = 0x45;
    rf.AF.r = 0x00; // unset carry flag
    clock_cpu();
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(rf.AF.l, ==, 0x44);
    // Subtraction flag set
    munit_assert_int(rf.AF.r, ==, 0b01000000);
    // A - H
    rf.HL.l = 0x01;
    rf.AF.l = 0x46;
    rf.AF.r = 0x10; // set carry flag
    clock_cpu();
    munit_assert_int(rf.PC, ==, 6);
    munit_assert_int(rf.AF.l, ==, 0x44);
    // Subtraction flag set
    munit_assert_int(rf.AF.r, ==, 0b01000000);
    // A - L
    rf.HL.r = 0x01;
    rf.AF.l = 0x47;
    rf.AF.r = 0x00; // unset carry flag
    clock_cpu();
    munit_assert_int(rf.PC, ==, 7);
    munit_assert_int(rf.AF.l, ==, 0x46);
    // Subtraction flag set
    munit_assert_int(rf.AF.r, ==, 0b01000000);
    // A - A
    rf.AF.r = 0x10; // set carry flag
    clock_cpu();
    munit_assert_int(rf.PC, ==, 8);
    // A - A == 0, 0 - 1 = 0xFF
    munit_assert_int(rf.AF.l, ==, 0xFF);
    // Subtraction, half carry, and carry flags set
    munit_assert_int(rf.AF.r, ==, 0b01110000);

    return MUNIT_OK;
}

static MunitResult test_sub_hl_indirect_with_carry() {
    // Subtract value from mem address in HL register
    // and carry flag from register A, store in register A
    // 0x9E
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {0x9E};
    memcpy(memory, instructions, sizeof(instructions));

    // load test value in memory, set HL to test memory addr
    memory[0xBEEF] = 0x01;
    rf.HL.lr = 0xBEEF;
    rf.AF.l = 0x42;
    rf.AF.r = 0x10; // set carry flag
    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 2);
    munit_assert_int(rf.AF.l, ==, 0x40);

    // Subtraction flag set
    munit_assert_int(rf.AF.r, ==, 0b01000000);
    return MUNIT_OK;
}

static MunitResult test_sub_immediate_with_carry() {
    // Subtract immediate and carry flag from register A,
    // store in register A, 0xDE
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {0xDE, 0x01};
    memcpy(memory, instructions, sizeof(instructions));

    // load test value in memory, set HL to test memory addr
    rf.AF.l = 0x42;
    rf.AF.r = 0x10; // set carry flag
    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3); // PC += 2 from immediate
    munit_assert_int(rf.AF.l, ==, 0x40);

    // Subtraction flag set
    munit_assert_int(rf.AF.r, ==, 0b01000000);
    return MUNIT_OK;
}

static MunitResult test_decrement_register() {
    // Decrement a register
    // 0x05, 0x0D, 0x15, 0x1D, 0x25, 0x2D, 0x3D
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {0x5, 0xD, 0x15, 0x1D, 0x25, 0x2D, 0x3D};
    memcpy(memory, instructions, sizeof(instructions));

    rf.BC.lr = 0x4142;
    rf.DE.lr = 0x4344;
    rf.HL.lr = 0x4546;
    rf.AF.l = 0x47;
    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    // B--
    clock_cpu();
    munit_assert_int(rf.PC, ==, 2);
    munit_assert_int(rf.BC.l, ==, 0x40);
    // Subtraction flag set
    munit_assert_int(rf.AF.r, ==, 0b01000000);
    // C--
    clock_cpu();
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(rf.BC.r, ==, 0x41);
    // Subtraction flag set
    munit_assert_int(rf.AF.r, ==, 0b01000000);
    // D--
    clock_cpu();
    munit_assert_int(rf.PC, ==, 4);
    munit_assert_int(rf.DE.l, ==, 0x42);
    // Subtraction flag set
    munit_assert_int(rf.AF.r, ==, 0b01000000);
    // E--
    clock_cpu();
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(rf.DE.r, ==, 0x43);
    // Subtraction flag set
    munit_assert_int(rf.AF.r, ==, 0b01000000);
    // H--
    clock_cpu();
    munit_assert_int(rf.PC, ==, 6);
    munit_assert_int(rf.HL.l, ==, 0x44);
    // Subtraction flag set
    munit_assert_int(rf.AF.r, ==, 0b01000000);
    // L--
    clock_cpu();
    munit_assert_int(rf.PC, ==, 7);
    munit_assert_int(rf.HL.r, ==, 0x45);
    // Subtraction flag set
    munit_assert_int(rf.AF.r, ==, 0b01000000);
    // A--
    clock_cpu();
    munit_assert_int(rf.PC, ==, 8);
    munit_assert_int(rf.AF.l, ==, 0x46);
    // Subtraction flag set
    munit_assert_int(rf.AF.r, ==, 0b01000000);

    return MUNIT_OK;
}

static MunitResult test_decrement_hl_indirect() {
    // Decrement value at mem address in HL register, 0x35
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {0x35};
    memcpy(memory, instructions, sizeof(instructions));

    // load test value in memory, set HL to test memory addr
    memory[0xBEEF] = 0x42;
    rf.HL.lr = 0xBEEF;
    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    clock_cpu(); clock_cpu(); clock_cpu();// execute (takes 3 cycles)
    munit_assert_int(rf.PC, ==, 2);
    munit_assert_int(memory[0xBEEF], ==, 0x41);

    // Subtraction flag set
    munit_assert_int(rf.AF.r, ==, 0b01000000);
    return MUNIT_OK;
}

MunitTest subtract_tests[] = {
    {
        "/sub_register",
        test_sub_register,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/sub_flags",
        test_sub_flags,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/sub_from_hl_indirect",
        test_sub_from_hl_indirect,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/sub_immediate",
        test_sub_immediate,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/sub_with_carry",
        test_sub_with_carry,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/sub_hl_indirect_with_carry",
        test_sub_hl_indirect_with_carry,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/sub_immediate_with_carry",
        test_sub_immediate_with_carry,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/decrement_register",
        test_decrement_register,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/decrement_hl_indirect",
        test_decrement_hl_indirect,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

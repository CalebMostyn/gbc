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

    // load test value in memory, set HL to test memory addr
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
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

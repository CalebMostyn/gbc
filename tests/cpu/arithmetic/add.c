#include "munit.h"
#include "helpers.h"
#include "cpu.h"

static MunitResult test_add_register() {
    // Add register A with register and store in register A
    // 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x87
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x87};
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    // B + A
    rf.BC.l = 0x42;
    rf.AF.l = 0x01;
    clock_cpu(); // execute
    munit_assert_int(rf.PC, ==, 2);
    // result stored in register A
    munit_assert_int(rf.AF.l, ==, 0x43);
    // C + A
    rf.BC.r = 0x43;
    rf.AF.l = 0x01;
    clock_cpu(); // execute
    munit_assert_int(rf.PC, ==, 3);
    // result stored in register A
    munit_assert_int(rf.AF.l, ==, 0x44);
    // D + A
    rf.DE.l = 0x44;
    rf.AF.l = 0x01;
    clock_cpu(); // execute
    munit_assert_int(rf.PC, ==, 4);
    // result stored in register A
    munit_assert_int(rf.AF.l, ==, 0x45);
    // E + A
    rf.DE.r = 0x45;
    rf.AF.l = 0x01;
    clock_cpu(); // execute
    munit_assert_int(rf.PC, ==, 5);
    // result stored in register A
    munit_assert_int(rf.AF.l, ==, 0x46);
    // H + A
    rf.HL.l = 0x46;
    rf.AF.l = 0x01;
    clock_cpu(); // execute
    munit_assert_int(rf.PC, ==, 6);
    // result stored in register A
    munit_assert_int(rf.AF.l, ==, 0x47);
    // L + A
    rf.HL.r = 0x47;
    rf.AF.l = 0x01;
    clock_cpu(); // execute
    munit_assert_int(rf.PC, ==, 7);
    // result stored in register A
    munit_assert_int(rf.AF.l, ==, 0x48);
    // A + A
    rf.AF.l = 0x01;
    clock_cpu(); // execute
    munit_assert_int(rf.PC, ==, 8);
    // result stored in register A
    munit_assert_int(rf.AF.l, ==, 0x02);

    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    return MUNIT_OK;
}

static MunitResult test_add_flags() {
    // Add register A with register and store in register A
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {0x80, 0x80, 0x80};
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    clock_cpu(); // execute
    munit_assert_int(rf.PC, ==, 2);
    // 0 + 0
    munit_assert_int(rf.AF.l, ==, 0x00);
    // zero flag enabled, result was 0
    munit_assert_int(rf.AF.r, ==, 0b10000000);

    rf.BC.l = 0x0F;
    rf.AF.l = 0x0F;
    clock_cpu(); // execute
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(rf.AF.l, ==, 0x1E);
    // half carry enabled, nibble addition exceeds 0x0F
    munit_assert_int(rf.AF.r, ==, 0b00100000);

    rf.BC.l = 0xFF;
    rf.AF.l = 0x10;
    clock_cpu(); // execute
    munit_assert_int(rf.PC, ==, 4);
    // 8-bit overflow
    munit_assert_int(rf.AF.l, ==, 0x0F);
    // just carry enabled, overflowed 0xFF but not 0x0F
    munit_assert_int(rf.AF.r, ==, 0b00010000);

    return MUNIT_OK;
}

static MunitResult test_add_from_hl_indirect() {
    // Add register A with value from mem address in HL register
    // and store in register A, 0x86
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {0x86};
    memcpy(memory, instructions, sizeof(instructions));

    // load test value in memory, set HL to test memory addr
    memory[0xBEEF] = 0x42;
    rf.HL.lr = 0xBEEF;
    rf.AF.l = 0x01;
    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 2);
    munit_assert_int(rf.AF.l, ==, 0x43);

    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    return MUNIT_OK;
}

static MunitResult test_add_immediate() {
    // Add register A with immediate and store in register A, 0xC6
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {0xC6, 0x42};
    memcpy(memory, instructions, sizeof(instructions));

    rf.AF.l = 0x01;
    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3); // PC += 2 from immediate
    munit_assert_int(rf.AF.l, ==, 0x43);

    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    return MUNIT_OK;
}

static MunitResult test_add_with_carry() {
    // Add register A with register and carry flag,
    // and store in register A
    // 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8F
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8F};
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    // B + A
    rf.BC.l = 0x42;
    rf.AF.l = 0x01;
    rf.AF.r = 0x10; // set carry flag
    clock_cpu();
    munit_assert_int(rf.PC, ==, 2);
    munit_assert_int(rf.AF.l, ==, 0x44);
    // C + A
    rf.BC.r = 0x43;
    rf.AF.l = 0x01;
    rf.AF.r = 0x00; // unset carry flag
    clock_cpu();
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(rf.AF.l, ==, 0x44);
    // D + A
    rf.DE.l = 0x44;
    rf.AF.l = 0x01;
    rf.AF.r = 0x10; // set carry flag
    clock_cpu();
    munit_assert_int(rf.PC, ==, 4);
    munit_assert_int(rf.AF.l, ==, 0x46);
    // E + A
    rf.DE.r = 0x45;
    rf.AF.l = 0x01;
    rf.AF.r = 0x00; // unset carry flag
    clock_cpu();
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(rf.AF.l, ==, 0x46);
    // H + A
    rf.HL.l = 0x46;
    rf.AF.l = 0x01;
    rf.AF.r = 0x10; // set carry flag
    clock_cpu();
    munit_assert_int(rf.PC, ==, 6);
    munit_assert_int(rf.AF.l, ==, 0x48);
    // L + A
    rf.HL.r = 0x47;
    rf.AF.l = 0x01;
    rf.AF.r = 0x00; // unset carry flag
    clock_cpu();
    munit_assert_int(rf.PC, ==, 7);
    munit_assert_int(rf.AF.l, ==, 0x48);
    // A + A
    rf.AF.l = 0x01;
    rf.AF.r = 0x10; // set carry flag
    clock_cpu();
    munit_assert_int(rf.PC, ==, 8);
    munit_assert_int(rf.AF.l, ==, 0x03);

    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    return MUNIT_OK;
}

static MunitResult test_add_from_hl_indirect_with_carry() {
    // Add register A with value from mem address in HL register
    // and carry flag, and store in register A, 0x8E
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {0x8E};
    memcpy(memory, instructions, sizeof(instructions));

    // load test value in memory, set HL to test memory addr
    memory[0xBEEF] = 0x42;
    rf.HL.lr = 0xBEEF;
    rf.AF.l = 0x01;
    rf.AF.r = 0x10; // set carry flag
    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 2);
    munit_assert_int(rf.AF.l, ==, 0x44);

    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    return MUNIT_OK;
}

static MunitResult test_add_immediate_with_carry() {
    // Add register A with immediate and store in register A, 0xC6
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {0xCE, 0x42};
    memcpy(memory, instructions, sizeof(instructions));

    rf.AF.l = 0x01;
    rf.AF.r = 0x10; // set carry flag
    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3); // PC += 2 from immediate
    munit_assert_int(rf.AF.l, ==, 0x44);

    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    return MUNIT_OK;
}

static MunitResult test_increment_register() {
    // Increment a register
    // 0x04, 0x0C, 0x14, 0x1C, 0x24, 0x2C, 0x3C
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {0x4, 0xC, 0x14, 0x1C, 0x24, 0x2C, 0x3C};
    memcpy(memory, instructions, sizeof(instructions));

    rf.BC.lr = 0x4142;
    rf.DE.lr = 0x4344;
    rf.HL.lr = 0x4546;
    rf.AF.l = 0x47;
    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    // B++
    clock_cpu();
    munit_assert_int(rf.PC, ==, 2);
    munit_assert_int(rf.BC.l, ==, 0x42);
    // C++
    clock_cpu();
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(rf.BC.r, ==, 0x43);
    // D++
    clock_cpu();
    munit_assert_int(rf.PC, ==, 4);
    munit_assert_int(rf.DE.l, ==, 0x44);
    // E++
    clock_cpu();
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(rf.DE.r, ==, 0x45);
    // H++
    clock_cpu();
    munit_assert_int(rf.PC, ==, 6);
    munit_assert_int(rf.HL.l, ==, 0x46);
    // L++
    clock_cpu();
    munit_assert_int(rf.PC, ==, 7);
    munit_assert_int(rf.HL.r, ==, 0x47);
    // A++
    clock_cpu();
    munit_assert_int(rf.PC, ==, 8);
    munit_assert_int(rf.AF.l, ==, 0x48);

    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    return MUNIT_OK;
}

static MunitResult test_increment_hl_indirect() {
    // Increment value at mem address in HL register, 0x34
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {0x34};
    memcpy(memory, instructions, sizeof(instructions));

    // load test value in memory, set HL to test memory addr
    memory[0xBEEF] = 0x42;
    rf.HL.lr = 0xBEEF;
    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    clock_cpu(); clock_cpu(); clock_cpu();// execute (takes 3 cycles)
    munit_assert_int(rf.PC, ==, 2);
    munit_assert_int(memory[0xBEEF], ==, 0x43);

    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    return MUNIT_OK;
}

MunitTest add_tests[] = {
    {
        "/add_register",
        test_add_register,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/add_flags",
        test_add_flags,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/add_from_hl_indirect",
        test_add_from_hl_indirect,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/add_immediate",
        test_add_immediate,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/add_with_carry",
        test_add_with_carry,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/add_from_hl_indirect_with_carry",
        test_add_from_hl_indirect_with_carry,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/add_immediate_with_carry",
        test_add_immediate_with_carry,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/increment_register",
        test_increment_register,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/increment_hl_indirect",
        test_increment_hl_indirect,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

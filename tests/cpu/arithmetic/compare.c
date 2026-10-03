#include "munit.h"
#include "helpers.h"
#include "cpu.h"

static MunitResult test_compare_register() {
    // Compare register to register A
    // 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBF
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBF};
    memcpy(memory, instructions, sizeof(instructions));

    rf.AF.l = 0x42;

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    // A == B ?
    rf.BC.l = 0x42;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 2);
    // Zero and subtraction flag
    munit_assert_int(rf.AF.r, ==, 0b11000000);
    // A == C ?
    rf.BC.r = 0x41;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 3);
    // Subtraction flag
    munit_assert_int(rf.AF.r, ==, 0b01000000);
    // A == D ?
    rf.DE.l = 0x42;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 4);
    // Zero and subtraction flag
    munit_assert_int(rf.AF.r, ==, 0b11000000);
    // A == E ?
    rf.DE.r = 0x41;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 5);
    // Subtraction flag
    munit_assert_int(rf.AF.r, ==, 0b01000000);
    // A == H ?
    rf.HL.l = 0x42;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 6);
    // Zero and subtraction flag
    munit_assert_int(rf.AF.r, ==, 0b11000000);
    // A == L ?
    rf.BC.r = 0x41;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 7);
    // Subtraction flag
    munit_assert_int(rf.AF.r, ==, 0b01000000);
    // A == A ?
    clock_cpu();
    munit_assert_int(rf.PC, ==, 8);
    // Zero and subtraction flag
    munit_assert_int(rf.AF.r, ==, 0b11000000);

    return MUNIT_OK;
}
static MunitResult test_compare_flags() {
    // Compare register to register A
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {0xB8, 0xB8};
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    rf.BC.l = 0x32;
    rf.AF.l = 0x41;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 2);
    // Subtraction and half carry flag
    munit_assert_int(rf.AF.r, ==, 0b01100000);
    rf.BC.l = 0x52;
    rf.AF.l = 0x42;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 3);
    // Subtraction and carry flag
    munit_assert_int(rf.AF.r, ==, 0b01010000);

    return MUNIT_OK;
}

static MunitResult test_compare_from_hl_indirect() {
    // Compare register A to value from mem address in HL register
    // 0xBE
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {0xBE};
    memcpy(memory, instructions, sizeof(instructions));

    // load test value in memory, set HL to test memory addr
    memory[0xBEEF] = 0x42;
    rf.HL.lr = 0xBEEF;
    rf.AF.l = 0x42;
    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 2);
    munit_assert_int(rf.AF.r, ==, 0b11000000);

    return MUNIT_OK;
}

static MunitResult test_compare_immediate() {
    // Compare immediate to register A, 0xFE
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {0xFE, 0x42, 0xFE, 0x41};
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    rf.AF.l = 0x42;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3); // PC += 2 from immediate
    // Zero and subtraction flags
    munit_assert_int(rf.AF.r, ==, 0b11000000);
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 5); // PC += 2 from immediate
    // Subtraction flag
    munit_assert_int(rf.AF.r, ==, 0b01000000);

    return MUNIT_OK;
}

MunitTest compare_tests[] = {
    {
        "/compare_register",
        test_compare_register,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/compare_flags",
        test_compare_flags,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/compare_from_hl_indirect",
        test_compare_from_hl_indirect,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/compare_immediate",
        test_compare_immediate,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

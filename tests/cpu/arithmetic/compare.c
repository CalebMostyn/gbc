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

MunitTest compare_tests[] = {
    {
        "/compare_register",
        test_compare_register,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

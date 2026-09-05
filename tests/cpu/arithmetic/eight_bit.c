#include "munit.h"
#include "helpers.h"
#include "cpu.h"

static MunitResult test_add() {
    // Add register A with register and store in register A
    // 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {0x80};
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    rf.BC.l = 0x42;
    rf.AF.l = 0x01;
    clock_cpu(); // execute
    munit_assert_int(rf.PC, ==, 2);
    // result stored in register A
    munit_assert_int(rf.AF.l, ==, 0x43);

    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    return MUNIT_OK;
}

MunitTest arithmetic_eight_bit_tests[] = {
    {
        "/add",
        test_add,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

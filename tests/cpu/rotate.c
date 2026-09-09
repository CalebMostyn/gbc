#include "munit.h"
#include "helpers.h"
#include "cpu.h"

static MunitResult test_rotate_left_circular_accumulator() {
    // Rotate register A left circularly, 0x07
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {0x07, 0x07};
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    rf.AF.l = 0b00000001;
    clock_cpu(); // execute
    munit_assert_int(rf.PC, ==, 2);
    munit_assert_int(rf.AF.l, ==, 0b00000010);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.AF.l = 0b10000000;
    clock_cpu(); // execute
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(rf.AF.l, ==, 0b00000001);
    // Carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00010000);

    return MUNIT_OK;
}

static MunitResult test_rotate_right_circular_accumulator() {
    // Rotate register A right circularly, 0x0F
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {0x0F, 0x0F};
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    rf.AF.l = 0b00000001;
    clock_cpu(); // execute
    munit_assert_int(rf.PC, ==, 2);
    munit_assert_int(rf.AF.l, ==, 0b10000000);
    // Carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00010000);
    rf.AF.l = 0b10000000;
    clock_cpu(); // execute
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(rf.AF.l, ==, 0b01000000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    return MUNIT_OK;
}

static MunitResult test_rotate_left_accumulator() {
    // Rotate register A left, inserting carry flag, 0x17
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {0x17, 0x17};
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    rf.AF.l = 0b00000001;
    clock_cpu(); // execute
    munit_assert_int(rf.PC, ==, 2);
    // Zero rotated in, carry flag was false
    munit_assert_int(rf.AF.l, ==, 0b00000010);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.AF.r = 0b00010000; // set carry flag
    clock_cpu(); // execute
    munit_assert_int(rf.PC, ==, 3);
    // One rotated in, carry flag was true
    munit_assert_int(rf.AF.l, ==, 0b00000101);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    return MUNIT_OK;
}

static MunitResult test_rotate_right_accumulator() {
    // Rotate register A right, inserting carry flag, 0x1F
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {0x1F, 0x1F};
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    rf.AF.l = 0b00000100;
    clock_cpu(); // execute
    munit_assert_int(rf.PC, ==, 2);
    // Zero rotated in, carry flag was false
    munit_assert_int(rf.AF.l, ==, 0b00000010);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.AF.r = 0b00010000; // set carry flag
    clock_cpu(); // execute
    munit_assert_int(rf.PC, ==, 3);
    // One rotated in, carry flag was true
    munit_assert_int(rf.AF.l, ==, 0b10000001);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    return MUNIT_OK;
}

MunitTest rotate_tests[] = {
    {
        "/rotate_left_circular_accumulator",
        test_rotate_left_circular_accumulator,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/rotate_right_circular_accumulator",
        test_rotate_right_circular_accumulator,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/rotate_left_accumulator",
        test_rotate_left_accumulator,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/rotate_right_accumulator",
        test_rotate_right_accumulator,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

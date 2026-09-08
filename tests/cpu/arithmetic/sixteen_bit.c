#include "munit.h"
#include "helpers.h"
#include "cpu.h"

static MunitResult test_increment_register_pair() {
    // Increments a register pair
    // 0x03, 0x13, 0x23, 0x33
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {0x03, 0x13, 0x23, 0x33};
    memcpy(memory, instructions, sizeof(instructions));

    rf.BC.lr = 0xBEEF;
    rf.DE.lr = 0xBEEE;
    rf.HL.lr = 0xBEED;
    rf.SP = 0xBEEC;
    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    // BC++
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 2);
    munit_assert_int(rf.BC.lr, ==, 0xBEF0);
    // DE++
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(rf.DE.lr, ==, 0xBEEF);
    // HL++
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 4);
    munit_assert_int(rf.HL.lr, ==, 0xBEEE);
    // SP++
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(rf.SP, ==, 0xBEED);

    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    return MUNIT_OK;
}

static MunitResult test_decrement_register_pair() {
    // Decrements a register pair
    // 0x0B, 0x1B, 0x2B, 0x3B
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {0x0B, 0x1B, 0x2B, 0x3B};
    memcpy(memory, instructions, sizeof(instructions));

    rf.BC.lr = 0xBEEF;
    rf.DE.lr = 0xBEEE;
    rf.HL.lr = 0xBEED;
    rf.SP = 0xBEEC;
    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    // BC--
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 2);
    munit_assert_int(rf.BC.lr, ==, 0xBEEE);
    // DE--
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(rf.DE.lr, ==, 0xBEED);
    // HL--
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 4);
    munit_assert_int(rf.HL.lr, ==, 0xBEEC);
    // SP--
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(rf.SP, ==, 0xBEEB);

    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    return MUNIT_OK;
}

static MunitResult test_add_register_pair_with_hl() {
    // Adds a register pair with register HL
    // 0x09, 0x19, 0x29, 0x39
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {0x09, 0x19, 0x29, 0x39};
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    // BC + HL
    rf.BC.lr = 0xBE00;
    rf.HL.lr = 0x00EF;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 2);
    munit_assert_int(rf.BC.lr, ==, 0xBEEF);
    // DE + HL
    rf.DE.lr = 0xDE00;
    rf.HL.lr = 0x00AD;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(rf.DE.lr, ==, 0xDEAD);
    // HL + HL
    rf.HL.lr = 0x0101;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 4);
    munit_assert_int(rf.HL.lr, ==, 0x0202);
    // SP + HL
    rf.SP = 0xBE00;
    rf.HL.lr = 0x00EF;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(rf.SP, ==, 0xBEEF);

    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    return MUNIT_OK;
}

static MunitResult test_add_sp_with_immediate() {
    // Adds SP and immediate value, 0xE8
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {0xE8, 0x42};
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    rf.SP = 0x4200;
    // execute (takes 4 cycles)
    clock_cpu(); clock_cpu();
    clock_cpu(); clock_cpu();
    munit_assert_int(rf.PC, ==, 3); // PC += 2 from immediate
    munit_assert_int(rf.SP, ==, 0x4242);
    // TODO: Test adding negative immediate?

    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    return MUNIT_OK;
}

MunitTest arithmetic_sixteen_bit_tests[] = {
    {
        "/increment_register_pair",
        test_increment_register_pair,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/decrement_register_pair",
        test_decrement_register_pair,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/add_register_pair_with_hl",
        test_add_register_pair_with_hl,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/add_sp_with_immediate",
        test_add_sp_with_immediate,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

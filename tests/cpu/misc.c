#include "munit.h"
#include "helpers.h"
#include "cpu.h"

static MunitResult test_pc_increment() {
    munit_assert_int(rf.PC, ==, 0);
    // each clock pulse should increment PC,
    // as memory is initialized to all 0s (NOP)
    // which only take 1 clock cycle
    for (int i = 1; i <= 1000; i++) {
        clock_cpu();
        munit_assert_int(rf.PC, ==, i);
    }
    // Assert the only register updated was PC
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    blank_rf.PC = 1000;
    assert_register_file_equal(blank_rf, rf);
    return MUNIT_OK;
}

static MunitResult test_cpu_halt() {
    // set first instruction to CPU halt, 0x76
    memory[0] = 0x76;
    munit_assert_int(rf.PC, ==, 0);
    // first clock pulse, fetches instruction at mem 0
    clock_cpu();
    munit_assert_int(rf.PC, ==, 1);
    // second clock pulse, executes CPU halt
    clock_cpu();
    munit_assert_int(rf.PC, ==, 2);
    munit_assert_true(cpu_halted);

    rf.IME = 1; // interrupts being enabled should not impact halt as mem is 0'd out
    register_file old_rf = rf;
    // PC should no longer increment, instructions should not be executed
    for (int i = 1; i <= 1000; i++) {
        clock_cpu();
        munit_assert_int(rf.PC, ==, 2);
        assert_register_file_equal(old_rf, rf);
    }

    // Force an interrupt, which should resume CPU on next clock pulse
    memory[IE_ADDR] = 0x01;
    memory[IF_ADDR] = 0x01;
    clock_cpu();
    munit_assert_false(cpu_halted);
    // PC to ISR location
    munit_assert_int(rf.PC, ==, 64);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    // TODO: Validate halt bug where cpu is resumed if there is a pending
    // interrupt when interrupts are disabled
    return MUNIT_OK;
}

static MunitResult test_nops() {
    // Undefined instructions/explicit NOPs
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0x00, 0xD3, 0xE3, 0xE4,
        0xF4, 0xDB, 0xEB, 0xEC,
        0xDD, 0xED, 0xFC, 0xFD
    };
    memcpy(memory, instructions, sizeof(instructions));


    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    // 0x00, NOP
    clock_cpu();
    munit_assert_int(rf.PC, ==, 2);
    // 0xD3
    clock_cpu();
    munit_assert_int(rf.PC, ==, 3);
    // 0xE3
    clock_cpu();
    munit_assert_int(rf.PC, ==, 4);
    // 0xE4
    clock_cpu();
    munit_assert_int(rf.PC, ==, 5);
    // 0xF4
    clock_cpu();
    munit_assert_int(rf.PC, ==, 6);
    // 0xDB
    clock_cpu();
    munit_assert_int(rf.PC, ==, 7);
    // 0xEB
    clock_cpu();
    munit_assert_int(rf.PC, ==, 8);
    // 0xEC
    clock_cpu();
    munit_assert_int(rf.PC, ==, 9);
    // 0xDD
    clock_cpu();
    munit_assert_int(rf.PC, ==, 10);
    // 0xED
    clock_cpu();
    munit_assert_int(rf.PC, ==, 11);
    // 0xFC
    clock_cpu();
    munit_assert_int(rf.PC, ==, 12);
    // 0xFD
    clock_cpu();
    munit_assert_int(rf.PC, ==, 13);

    // Assert the only register updated was PC
    blank_rf.PC = 13;
    assert_register_file_equal(blank_rf, rf);
    return MUNIT_OK;
}

MunitTest misc_instructions_tests[] = {
    {
        "/pc_increment",
        test_pc_increment,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/cpu_halt",
        test_cpu_halt,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/nops",
        test_nops,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

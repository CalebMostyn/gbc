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

static MunitResult test_halt() {
    // Halts the CPU until next interrupt, 0x76
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {0x76};
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    clock_cpu(); // execute
    // Next instruction not fetched, CPU halted
    munit_assert_int(rf.PC, ==, 1);
    munit_assert_true(cpu_halted);

    rf.IME = 1; // interrupts being enabled should not impact halt as mem is 0'd out
    register_file old_rf = rf;
    // PC should no longer increment, instructions should not be executed
    for (int i = 1; i <= 1000; i++) {
        clock_cpu();
        munit_assert_int(rf.PC, ==, 1);
        assert_register_file_equal(old_rf, rf);
    }

    // Force an interrupt, which should resume CPU on next clock pulse
    memory[IE_ADDR] = 0x01;
    memory[IF_ADDR] = 0x01;
    clock_cpu();
    munit_assert_false(cpu_halted);
    // PC to ISR location
    munit_assert_int(rf.PC, ==, 0x40);
    // TODO: Validate halt bug where cpu is resumed if there is a pending
    // interrupt when interrupts are disabled

    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    return MUNIT_OK;
}

static MunitResult test_stop() {
    // Stops the CPU (really the whole console) until next Joypad interrupt, 0x10
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {0x10};
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    clock_cpu(); // execute
    // Next instruction not fetched, CPU halted
    munit_assert_int(rf.PC, ==, 1);
    munit_assert_true(cpu_stopped);

    register_file old_rf = rf;
    // PC should no longer increment, instructions should not be executed
    for (int i = 1; i <= 1000; i++) {
        clock_cpu();
        munit_assert_int(rf.PC, ==, 1);
        assert_register_file_equal(old_rf, rf);
    }

    // V-Blank interrupt will not resume CPU
    memory[IE_ADDR] = 0x01;
    memory[IF_ADDR] = 0x01;
    clock_cpu();
    munit_assert_true(cpu_stopped);
    // PC unchanged
    munit_assert_int(rf.PC, ==, 1);

    // Joypad interrupt WILL resume CPU
    memory[IF_ADDR] = 0x10; // IE and IME do not need to be set
    clock_cpu();
    munit_assert_false(cpu_stopped);
    // PC to ISR location
    munit_assert_int(rf.PC, ==, 0x60);

    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    return MUNIT_OK;
}

static MunitResult test_disable_interrupts() {
    // Sets interrupt master enable to false, 0xF3
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {0xF3};
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    clock_cpu(); // execute
    munit_assert_int(rf.PC, ==, 2);
    // interrupts disabled
    munit_assert_false(rf.IME);

    // interrupt should be ignored
    memory[IE_ADDR] = 0x01;
    memory[IF_ADDR] = 0x01;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 3); // NOT ISR address

    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    return MUNIT_OK;
}

static MunitResult test_enable_interrupts() {
    // Sets interrupt master enable to true, 0xFB
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {0xFB};
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    clock_cpu(); // execute
    munit_assert_int(rf.PC, ==, 2);
    // interrupts not enabled yet
    munit_assert_false(rf.IME);

    // interrupt should be ignored
    memory[IE_ADDR] = 0x01;
    memory[IF_ADDR] = 0x01;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 3); // NOT ISR address
    // interrupts now enabled
    munit_assert_true(rf.IME);
    // next clock actually handles interrupt
    clock_cpu();
    munit_assert_int(rf.PC, ==, 0x40); // ISR address

    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
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
        "/halt",
        test_halt,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/stop",
        test_stop,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/disable_interrupts",
        test_disable_interrupts,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/enable_interrupts",
        test_enable_interrupts,
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

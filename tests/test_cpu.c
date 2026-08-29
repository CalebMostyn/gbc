#include "munit.h"
#include "cpu.h"

static void assert_register_file_equal(register_file a, register_file b) {
    munit_assert_int(a.IME, ==, b.IME);
    munit_assert_int(a.AF.lr, ==, b.AF.lr);
    munit_assert_int(a.BC.lr, ==, b.BC.lr);
    munit_assert_int(a.DE.lr, ==, b.DE.lr);
    munit_assert_int(a.HL.lr, ==, b.HL.lr);
    munit_assert_int(a.PC, ==, b.PC);
    munit_assert_int(a.SP, ==, b.SP);
}

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
    // TODO: Validate halt bug where cpu is resumed if there is a pending
    // interrupt when interrupts are disabled
    return MUNIT_OK;
}

static MunitResult test_load_from_hl_indirect() {
    // Loads register from mem address in HL register
    // 0x46, 0x4E, 0x56, 0x5E, 0x66, 0x6E, 0x7E
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {0x46, 0x4E, 0x56, 0x5E, 0x66, 0x6E, 0x7E};
    memcpy(memory, instructions, sizeof(instructions));

    // load test value in memory, set HL to test memory addr
    memory[0xBEEF] = 0x42;
    rf.HL.lr = 0xBEEF;
    // load b from HL address
    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 2);
    munit_assert_int(rf.BC.l, ==, 0x42);
    // load c from HL address
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(rf.BC.r, ==, 0x42);
    // load d from HL address
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 4);
    munit_assert_int(rf.DE.l, ==, 0x42);
    // load e from HL address
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(rf.DE.r, ==, 0x42);
    // load h from HL address
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 6);
    munit_assert_int(rf.HL.l, ==, 0x42);
    rf.HL.lr = 0xBEEF; // reset HL after it being overwritten
    // load l from HL address
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 7);
    munit_assert_int(rf.HL.r, ==, 0x42);
    rf.HL.lr = 0xBEEF; // reset HL after it being overwritten
    // load a from HL address
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 8);
    munit_assert_int(rf.AF.l, ==, 0x42);
    return MUNIT_OK;
}

MunitTest cpu_tests[] = {
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
        "/load_from_hl_indirect",
        test_load_from_hl_indirect,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

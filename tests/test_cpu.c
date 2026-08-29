#include "munit.h"
#include "cpu.h"

// INSTRUCTIONS TESTED:
// 0x0, 0x76
// 0x46, 0x4E, 0x56, 0x5E, 0x66, 0x6E, 0x7E
// 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x77
// 0x36
// 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x47,
// 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4F,
// 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x57,
// 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5F,
// 0x60, 0x61, 0x62, 0x63, 0x64, 0x65,  0x67,
// 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D,  0x6F,
// 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7F

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
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
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

    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    return MUNIT_OK;
}

static MunitResult test_load_to_hl_indirect_register() {
    // Loads mem address in HL register from register
    // 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x77
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x77};
    memcpy(memory, instructions, sizeof(instructions));

    // set memory location to load to
    rf.HL.lr = 0xDEAD;
    munit_assert_int(memory[0xDEAD], ==, 0);

    // load HL addr from b
    rf.BC.l = 0x42;
    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 2);
    munit_assert_int(memory[0xDEAD], ==, 0x42);
    // load HL addr from c
    rf.BC.r = 0x43;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(memory[0xDEAD], ==, 0x43);
    // load HL addr from d
    rf.DE.l = 0x44;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 4);
    munit_assert_int(memory[0xDEAD], ==, 0x44);
    // load HL addr from e
    rf.DE.r = 0x45;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(memory[0xDEAD], ==, 0x45);
    // load HL addr from h
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 6);
    munit_assert_int(memory[0xDEAD], ==, 0xDE);
    // load HL addr from l
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 7);
    munit_assert_int(memory[0xDEAD], ==, 0xAD);
    // load HL addr from a
    rf.AF.l = 0x46;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 8);
    munit_assert_int(memory[0xDEAD], ==, 0x46);

    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    return MUNIT_OK;
}

static MunitResult test_load_to_hl_indirect_immediate() {
    // Loads mem address in HL register from immediate, 0x36
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    // 0x36 instruction, 0x42 immediate value
    uint8_t instructions[] = {0x36, 0x42};
    memcpy(memory, instructions, sizeof(instructions));

    // set memory location to load to
    rf.HL.lr = 0xDEAD;
    munit_assert_int(memory[0xDEAD], ==, 0);

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    clock_cpu(); clock_cpu(); clock_cpu(); // execute (takes 3 cycles)
    munit_assert_int(rf.PC, ==, 3); // PC +=2 because of immediate
    munit_assert_int(memory[0xDEAD], ==, 0x42); // set to immediate

    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    return MUNIT_OK;
}

static MunitResult test_load_register_from_register() {
    // Loads register from other register
    // Load to B
    // 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x47,
    // Load to C
    // 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4F,
    // Load to D
    // 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x57,
    // Load to E
    // 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5F,
    // Load to H
    // 0x60, 0x61, 0x62, 0x63, 0x64, 0x65,  0x67,
    // Load to L
    // 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D,  0x6F,
    // Load to A
    // 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7F

    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        // Load to B
        0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x47,
        // Load to C
        0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4F,
        // Load to D
        0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x57,
        // Load to E
        0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5F,
        // Load to H
        0x60, 0x61, 0x62, 0x63, 0x64, 0x65,  0x67,
        // Load to L
        0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D,  0x6F,
        // Load to A
        0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7F
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);

    // B <- B
    rf.BC.l = 0x42;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 2);
    munit_assert_int(rf.BC.l, ==, 0x42);
    // B <- C
    rf.BC.r = 0x43;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(rf.BC.l, ==, 0x43);
    // B <- D
    rf.DE.l = 0x44;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 4);
    munit_assert_int(rf.BC.l, ==, 0x44);
    // B <- E
    rf.DE.r = 0x45;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(rf.BC.l, ==, 0x45);
    // B <- H
    rf.HL.l = 0x46;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 6);
    munit_assert_int(rf.BC.l, ==, 0x46);
    // B <- L
    rf.HL.r = 0x47;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 7);
    munit_assert_int(rf.BC.l, ==, 0x47);
    // B <- A
    rf.AF.l = 0x48;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 8);
    munit_assert_int(rf.BC.l, ==, 0x48);

    // C <- B
    rf.BC.l = 0x49;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 9);
    munit_assert_int(rf.BC.r, ==, 0x49);
    // C <- C
    rf.BC.r = 0x50;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 10);
    munit_assert_int(rf.BC.r, ==, 0x50);
    // C <- D
    rf.DE.l = 0x51;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 11);
    munit_assert_int(rf.BC.r, ==, 0x51);
    // C <- E
    rf.DE.r = 0x52;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 12);
    munit_assert_int(rf.BC.r, ==, 0x52);
    // C <- H
    rf.HL.l = 0x53;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 13);
    munit_assert_int(rf.BC.r, ==, 0x53);
    // C <- L
    rf.HL.r = 0x54;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 14);
    munit_assert_int(rf.BC.r, ==, 0x54);
    // C <- A
    rf.AF.l = 0x55;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 15);
    munit_assert_int(rf.BC.r, ==, 0x55);
 
    // D <- B
    rf.BC.l = 0x56;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 16);
    munit_assert_int(rf.DE.l, ==, 0x56);
    // D <- C
    rf.BC.r = 0x57;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 17);
    munit_assert_int(rf.DE.l, ==, 0x57);
    // D <- D
    rf.DE.l = 0x58;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 18);
    munit_assert_int(rf.DE.l, ==, 0x58);
    // D <- E
    rf.DE.r = 0x59;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 19);
    munit_assert_int(rf.DE.l, ==, 0x59);
    // D <- H
    rf.HL.l = 0x60;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 20);
    munit_assert_int(rf.DE.l, ==, 0x60);
    // D <- L
    rf.HL.r = 0x61;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 21);
    munit_assert_int(rf.DE.l, ==, 0x61);
    // D <- A
    rf.AF.l = 0x62;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 22);
    munit_assert_int(rf.DE.l, ==, 0x62);

    // E <- B
    rf.BC.l = 0x63;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 23);
    munit_assert_int(rf.DE.r, ==, 0x63);
    // E <- C
    rf.BC.r = 0x64;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 24);
    munit_assert_int(rf.DE.r, ==, 0x64);
    // E <- D
    rf.DE.l = 0x65;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 25);
    munit_assert_int(rf.DE.r, ==, 0x65);
    // E <- E
    rf.DE.r = 0x66;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 26);
    munit_assert_int(rf.DE.r, ==, 0x66);
    // E <- H
    rf.HL.l = 0x67;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 27);
    munit_assert_int(rf.DE.r, ==, 0x67);
    // E <- L
    rf.HL.r = 0x68;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 28);
    munit_assert_int(rf.DE.r, ==, 0x68);
    // E <- A
    rf.AF.l = 0x69;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 29);
    munit_assert_int(rf.DE.r, ==, 0x69);

    // H <- B
    rf.BC.l = 0x70;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 30);
    munit_assert_int(rf.HL.l, ==, 0x70);
    // H <- C
    rf.BC.r = 0x71;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 31);
    munit_assert_int(rf.HL.l, ==, 0x71);
    // H <- D
    rf.DE.l = 0x72;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 32);
    munit_assert_int(rf.HL.l, ==, 0x72);
    // H <- E
    rf.DE.r = 0x73;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 33);
    munit_assert_int(rf.HL.l, ==, 0x73);
    // H <- H
    rf.HL.l = 0x74;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 34);
    munit_assert_int(rf.HL.l, ==, 0x74);
    // H <- L
    rf.HL.r = 0x75;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 35);
    munit_assert_int(rf.HL.l, ==, 0x75);
    // H <- A
    rf.AF.l = 0x76;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 36);
    munit_assert_int(rf.HL.l, ==, 0x76);

    // L <- B
    rf.BC.l = 0x77;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 37);
    munit_assert_int(rf.HL.r, ==, 0x77);
    // L <- C
    rf.BC.r = 0x78;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 38);
    munit_assert_int(rf.HL.r, ==, 0x78);
    // L <- D
    rf.DE.l = 0x79;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 39);
    munit_assert_int(rf.HL.r, ==, 0x79);
    // L <- E
    rf.DE.r = 0x80;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 40);
    munit_assert_int(rf.HL.r, ==, 0x80);
    // L <- H
    rf.HL.l = 0x81;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 41);
    munit_assert_int(rf.HL.r, ==, 0x81);
    // L <- L
    rf.HL.r = 0x82;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 42);
    munit_assert_int(rf.HL.r, ==, 0x82);
    // L <- A
    rf.AF.l = 0x83;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 43);
    munit_assert_int(rf.HL.r, ==, 0x83);

    // A <- B
    rf.BC.l = 0x84;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 44);
    munit_assert_int(rf.AF.l, ==, 0x84);
    // A <- C
    rf.BC.r = 0x85;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 45);
    munit_assert_int(rf.AF.l, ==, 0x85);
    // A <- D
    rf.DE.l = 0x86;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 46);
    munit_assert_int(rf.AF.l, ==, 0x86);
    // A <- E
    rf.DE.r = 0x87;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 47);
    munit_assert_int(rf.AF.l, ==, 0x87);
    // A <- H
    rf.HL.l = 0x88;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 48);
    munit_assert_int(rf.AF.l, ==, 0x88);
    // A <- L
    rf.HL.r = 0x89;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 49);
    munit_assert_int(rf.AF.l, ==, 0x89);
    // A <- A
    rf.AF.l = 0x90;
    clock_cpu();
    munit_assert_int(rf.PC, ==, 50);
    munit_assert_int(rf.AF.l, ==, 0x90);

    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
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
    {
        "/load_to_hl_indirect_register",
        test_load_to_hl_indirect_register,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/load_to_hl_indirect_immediate",
        test_load_to_hl_indirect_immediate,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/load_register_from_register",
        test_load_register_from_register,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

#include "memory_bus.h"
#include "munit.h"
#include "helpers.h"
#include "cpu.h"

static MunitResult test_load_16_bit_register_from_immediates() {
    // Loads 16-bit register from two immediate bytes
    // 0x01, 0x11, 0x21, 0x31
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0x01, 0xAD, 0xDE, 
        0x11, 0xAD, 0xDE, 
        0x21, 0xAD, 0xDE, 
        0x31, 0xAD, 0xDE, 
    };
    write_instructions_to_memory(0, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    clock_cpu(); clock_cpu(); clock_cpu(); // execute (takes 3 cycles)
    munit_assert_int(rf.PC, ==, 4); // PC += 3 from immediates
    munit_assert_int(rf.BC.lr, ==, 0xDEAD); // set to immediate
    clock_cpu(); clock_cpu(); clock_cpu(); // execute (takes 3 cycles)
    munit_assert_int(rf.PC, ==, 7); // PC += 3 from immediates
    munit_assert_int(rf.DE.lr, ==, 0xDEAD); // set to immediate
    clock_cpu(); clock_cpu(); clock_cpu(); // execute (takes 3 cycles)
    munit_assert_int(rf.PC, ==, 10); // PC += 3 from immediates
    munit_assert_int(rf.HL.lr, ==, 0xDEAD); // set to immediate
    clock_cpu(); clock_cpu(); clock_cpu(); // execute (takes 3 cycles)
    munit_assert_int(rf.PC, ==, 13); // PC += 3 from immediates
    munit_assert_int(rf.SP, ==, 0xDEAD); // set to immediate

    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    return MUNIT_OK;
}

static MunitResult test_load_immediate_indirect_from_sp() {
    // Loads mem address at immediate bytes from SP
    // 0x08
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {0x08, 0xEF, 0xBE};
    write_instructions_to_memory(0, instructions, sizeof(instructions));

    rf.SP = 0x4224;
    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    // execute (takes 5 cycles)
    clock_cpu(); clock_cpu();
    clock_cpu(); clock_cpu(); clock_cpu();
    munit_assert_int(rf.PC, ==, 4); // PC += 3 from immediates
    munit_assert_int(dma_read(0xBEEF), ==, 0x24); // set to SP lsbyte
    munit_assert_int(dma_read(0xBEF0), ==, 0x42); // set to immediate

    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    return MUNIT_OK;
}

static MunitResult test_load_sp_from_hl() {
    // Loads SP from register HL
    // 0xF9
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {0xF9};
    write_instructions_to_memory(0, instructions, sizeof(instructions));

    rf.HL.lr = 0x4224;
    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 2);
    munit_assert_int(rf.SP, ==, 0x4224); // set to immediate

    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    return MUNIT_OK;
}

static MunitResult test_push_to_stack() {
    // Push to stack from register pair
    // 0xC5, 0xD5, 0xE5, 0xF5
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {0xC5, 0xD5, 0xE5, 0xF5};
    write_instructions_to_memory(0, instructions, sizeof(instructions));

    rf.SP = 0xBEF0; // SP is decremented before each use

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    rf.BC.lr = 0xDEAD;
    clock_cpu(); clock_cpu(); clock_cpu(); clock_cpu(); // execute (takes 4 cycles)
    munit_assert_int(rf.PC, ==, 2);
    munit_assert_int(rf.SP, ==, 0xBEEE);
    munit_assert_int(dma_read(0xBEEF), ==, 0xDE);
    munit_assert_int(dma_read(0xBEEE), ==, 0xAD);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    rf.DE.lr = 0xDEAD;
    clock_cpu(); clock_cpu(); clock_cpu(); clock_cpu(); // execute (takes 4 cycles)
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(rf.SP, ==, 0xBEEC);
    munit_assert_int(dma_read(0xBEED), ==, 0xDE);
    munit_assert_int(dma_read(0xBEEC), ==, 0xAD);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    rf.HL.lr = 0xDEAD;
    clock_cpu(); clock_cpu(); clock_cpu(); clock_cpu(); // execute (takes 4 cycles)
    munit_assert_int(rf.PC, ==, 4);
    munit_assert_int(rf.SP, ==, 0xBEEA);
    munit_assert_int(dma_read(0xBEEB), ==, 0xDE);
    munit_assert_int(dma_read(0xBEEA), ==, 0xAD);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    rf.AF.lr = 0xDEAD;
    clock_cpu(); clock_cpu(); clock_cpu(); clock_cpu(); // execute (takes 4 cycles)
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(rf.SP, ==, 0xBEE8);
    munit_assert_int(dma_read(0xBEE9), ==, 0xDE);
    munit_assert_int(dma_read(0xBEE8), ==, 0xAD);

    return MUNIT_OK;
}

static MunitResult test_pop_from_stack() {
    // Pop from stack to register pair
    // 0xC1, 0xD1, 0xE1, 0xF1
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {0xC1, 0xD1, 0xE1, 0xF1};
    write_instructions_to_memory(0, instructions, sizeof(instructions));

    dma_write(0xBEE8, 0xAD); dma_write(0xBEE9, 0xDE);
    dma_write(0xBEEA, 0xAD); dma_write(0xBEEB, 0xDE);
    dma_write(0xBEEC, 0xAD); dma_write(0xBEED, 0xDE);
    dma_write(0xBEEE, 0xAD); dma_write(0xBEEF, 0xDE);
    rf.SP = 0xBEE8;

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    clock_cpu(); clock_cpu(); clock_cpu();  // execute (takes 3 cycles)
    munit_assert_int(rf.PC, ==, 2);
    munit_assert_int(rf.BC.lr, ==, 0xDEAD);
    munit_assert_int(rf.SP, ==, 0xBEEA);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    clock_cpu(); clock_cpu(); clock_cpu();  // execute (takes 3 cycles)
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(rf.DE.lr, ==, 0xDEAD);
    munit_assert_int(rf.SP, ==, 0xBEEC);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    clock_cpu(); clock_cpu(); clock_cpu();  // execute (takes 3 cycles)
    munit_assert_int(rf.PC, ==, 4);
    munit_assert_int(rf.HL.lr, ==, 0xDEAD);
    munit_assert_int(rf.SP, ==, 0xBEEE);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    clock_cpu(); clock_cpu(); clock_cpu();  // execute (takes 3 cycles)
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(rf.AF.lr, ==, 0xDEA0); // Flag lower nibble never written to
    munit_assert_int(rf.SP, ==, 0xBEF0);

    return MUNIT_OK;
}

static MunitResult test_push_then_pop() {
    // Push from BC, pop to DE
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {0xC5, 0xD1};
    write_instructions_to_memory(0, instructions, sizeof(instructions));

    rf.SP = 0xDEAD;
    rf.BC.lr = 0xBEEF;

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    // PUSH
    clock_cpu(); clock_cpu(); clock_cpu(); clock_cpu();  // execute (takes 4 cycles)
    munit_assert_int(rf.PC, ==, 2);
    munit_assert_int(rf.SP, ==, 0xDEAB);
    // POP
    clock_cpu(); clock_cpu(); clock_cpu();  // execute (takes 4 cycles)
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(rf.SP, ==, 0xDEAD);
    // Should be same value we pushed on the stack
    munit_assert_int(rf.DE.lr, ==, 0xBEEF);

    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    return MUNIT_OK;
}

static MunitResult test_load_hl_from_sp_plus_immediate() {
    // Load HL from SP + immediate byte, 0xF8
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {0xF8, 0x0B, 0xF8, 0xFF};
    write_instructions_to_memory(0, instructions, sizeof(instructions));

    rf.SP = 0xB000;
    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    clock_cpu(); clock_cpu(); clock_cpu();  // execute (takes 3 cycles)
    munit_assert_int(rf.PC, ==, 3); // PC +=2 from immediate
    munit_assert_int(rf.HL.lr, ==, 0xB00B);

    rf.SP = 0xBEF0;
    clock_cpu(); clock_cpu(); clock_cpu();  // execute (takes 3 cycles)
    munit_assert_int(rf.PC, ==, 5); // PC +=2 from immediate
    // Immediate is treated as signed, so 0xFF should be -1,
    // thus 0xBEF0 - 1 == 0xBEEF
    munit_assert_int(rf.HL.lr, ==, 0xBEEF);

    // Only carry flag should be true,
    // zero and sub are always false,
    // and half carry is not incurred
    munit_assert_int(rf.AF.r, ==, 0b00010000);
    return MUNIT_OK;
}

MunitTest load_sixteen_bit_tests[] = {
    {
        "/load_16_bit_register_from_immediates",
        test_load_16_bit_register_from_immediates,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/load_immediate_indirect_from_sp",
        test_load_immediate_indirect_from_sp,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/load_sp_from_hl",
        test_load_sp_from_hl,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/push_to_stack",
        test_push_to_stack,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/pop_from_stack",
        test_pop_from_stack,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/push_then_pop",
        test_push_then_pop,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/load_hl_from_sp_plus_immediate",
        test_load_hl_from_sp_plus_immediate,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

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

static MunitResult test_rotate_left_circular_b() {
    // Rotate register B left circularly, 0xCB00
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0x00,
        0xCB, 0x00
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    rf.BC.l = 0b00000001;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(rf.BC.l, ==, 0b00000010);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.BC.l = 0b10000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(rf.BC.l, ==, 0b00000001);
    // Carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00010000);

    return MUNIT_OK;
}

static MunitResult test_rotate_left_circular_c() {
    // Rotate register C left circularly, 0xCB01
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0x01,
        0xCB, 0x01
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    rf.BC.r = 0b00000001;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(rf.BC.r, ==, 0b00000010);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.BC.r = 0b10000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(rf.BC.r, ==, 0b00000001);
    // Carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00010000);

    return MUNIT_OK;
}

static MunitResult test_rotate_left_circular_d() {
    // Rotate register D left circularly, 0xCB02
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0x02,
        0xCB, 0x02
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    rf.DE.l = 0b00000001;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(rf.DE.l, ==, 0b00000010);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.DE.l = 0b10000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(rf.DE.l, ==, 0b00000001);
    // Carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00010000);

    return MUNIT_OK;
}

static MunitResult test_rotate_left_circular_e() {
    // Rotate register E left circularly, 0xCB03
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0x03,
        0xCB, 0x03
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    rf.DE.r = 0b00000001;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(rf.DE.r, ==, 0b00000010);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.DE.r = 0b10000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(rf.DE.r, ==, 0b00000001);
    // Carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00010000);

    return MUNIT_OK;
}

static MunitResult test_rotate_left_circular_h() {
    // Rotate register H left circularly, 0xCB04
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0x04,
        0xCB, 0x04
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    rf.HL.l = 0b00000001;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(rf.HL.l, ==, 0b00000010);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.HL.l = 0b10000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(rf.HL.l, ==, 0b00000001);
    // Carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00010000);

    return MUNIT_OK;
}

static MunitResult test_rotate_left_circular_l() {
    // Rotate register L left circularly, 0xCB05
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0x05,
        0xCB, 0x05
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    rf.HL.r = 0b00000001;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(rf.HL.r, ==, 0b00000010);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.HL.r = 0b10000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(rf.HL.r, ==, 0b00000001);
    // Carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00010000);

    return MUNIT_OK;
}

static MunitResult test_rotate_left_circular_hl_indirect() {
    // Rotate mem value at mem address in register HL
    // left circularly, 0xCB06
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0x06,
        0xCB, 0x06
    };
    memcpy(memory, instructions, sizeof(instructions));

    rf.HL.lr = 0xBEEF;
    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    memory[0xBEEF] = 0b00000001;
    // execute (takes 4 cycles)
    clock_cpu(); clock_cpu();
    clock_cpu(); clock_cpu();
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(memory[0xBEEF], ==, 0b00000010);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    memory[0xBEEF] = 0b10000000;
    // execute (takes 4 cycles)
    clock_cpu(); clock_cpu();
    clock_cpu(); clock_cpu();
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(memory[0xBEEF], ==, 0b00000001);
    // Carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00010000);

    return MUNIT_OK;
}

static MunitResult test_rotate_left_circular_a() {
    // Rotate register A left circularly, 0xCB07
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0x07,
        0xCB, 0x07
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    rf.AF.l = 0b00000001;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(rf.AF.l, ==, 0b00000010);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.AF.l = 0b10000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(rf.AF.l, ==, 0b00000001);
    // Carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00010000);

    return MUNIT_OK;
}

static MunitResult test_rotate_right_circular_b() {
    // Rotate register B right circularly, 0xCB08
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0x08,
        0xCB, 0x08
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    rf.BC.l = 0b00000001;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(rf.BC.l, ==, 0b10000000);
    // Carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00010000);
    rf.BC.l = 0b10000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(rf.BC.l, ==, 0b01000000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    return MUNIT_OK;
}

static MunitResult test_rotate_right_circular_c() {
    // Rotate register C right circularly, 0xCB09
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0x09,
        0xCB, 0x09
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    rf.BC.r = 0b00000001;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(rf.BC.r, ==, 0b10000000);
    // Carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00010000);
    rf.BC.r = 0b10000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(rf.BC.r, ==, 0b01000000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    return MUNIT_OK;
}

static MunitResult test_rotate_right_circular_d() {
    // Rotate register D right circularly, 0xCB0A
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0x0A,
        0xCB, 0x0A
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    rf.DE.l = 0b00000001;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(rf.DE.l, ==, 0b10000000);
    // Carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00010000);
    rf.DE.l = 0b10000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(rf.DE.l, ==, 0b01000000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    return MUNIT_OK;
}

static MunitResult test_rotate_right_circular_e() {
    // Rotate register E right circularly, 0xCB0B
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0x0B,
        0xCB, 0x0B
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    rf.DE.r = 0b00000001;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(rf.DE.r, ==, 0b10000000);
    // Carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00010000);
    rf.DE.r = 0b10000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(rf.DE.r, ==, 0b01000000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    return MUNIT_OK;
}

static MunitResult test_rotate_right_circular_h() {
    // Rotate register H right circularly, 0xCB0C
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0x0C,
        0xCB, 0x0C
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    rf.HL.l = 0b00000001;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(rf.HL.l, ==, 0b10000000);
    // Carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00010000);
    rf.HL.l = 0b10000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(rf.HL.l, ==, 0b01000000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    return MUNIT_OK;
}

static MunitResult test_rotate_right_circular_l() {
    // Rotate register L right circularly, 0xCB0D
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0x0D,
        0xCB, 0x0D
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    rf.HL.r = 0b00000001;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(rf.HL.r, ==, 0b10000000);
    // Carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00010000);
    rf.HL.r = 0b10000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(rf.HL.r, ==, 0b01000000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    return MUNIT_OK;
}

static MunitResult test_rotate_right_circular_hl_indirect() {
    // Rotate mem value at mem address in register HL
    // right circularly, 0xCB0E
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0x0E,
        0xCB, 0x0E
    };
    memcpy(memory, instructions, sizeof(instructions));

    rf.HL.lr = 0xBEEF;
    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    memory[0xBEEF] = 0b00000001;
    // execute (takes 4 cycles)
    clock_cpu(); clock_cpu();
    clock_cpu(); clock_cpu();
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(memory[0xBEEF], ==, 0b10000000);
    // Carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00010000);
    memory[0xBEEF] = 0b10000000;
    // execute (takes 4 cycles)
    clock_cpu(); clock_cpu();
    clock_cpu(); clock_cpu();
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(memory[0xBEEF], ==, 0b01000000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    return MUNIT_OK;
}

static MunitResult test_rotate_right_circular_a() {
    // Rotate register A right circularly, 0xCB0F
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0x0F,
        0xCB, 0x0F
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    rf.AF.l = 0b00000001;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(rf.AF.l, ==, 0b10000000);
    // Carry flag set
    munit_assert_int(rf.AF.r, ==, 0b00010000);
    rf.AF.l = 0b10000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(rf.AF.l, ==, 0b01000000);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    return MUNIT_OK;
}

static MunitResult test_rotate_left_b() {
    // Rotate register B left, inserting carry flag, 0xCB10
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0x10,
        0xCB, 0x10
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    rf.BC.l = 0b00000001;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3);
    // Zero rotated in, carry flag was false
    munit_assert_int(rf.BC.l, ==, 0b00000010);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.AF.r = 0b00010000; // set carry flag
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(rf.BC.l, ==, 0b00000101);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    return MUNIT_OK;
}

static MunitResult test_rotate_left_c() {
    // Rotate register C left, inserting carry flag, 0xCB11
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0x11,
        0xCB, 0x11
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    rf.BC.r = 0b00000001;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3);
    // Zero rotated in, carry flag was false
    munit_assert_int(rf.BC.r, ==, 0b00000010);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.AF.r = 0b00010000; // set carry flag
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(rf.BC.r, ==, 0b00000101);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    return MUNIT_OK;
}

static MunitResult test_rotate_left_d() {
    // Rotate register D left, inserting carry flag, 0xCB12
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0x12,
        0xCB, 0x12
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    rf.DE.l = 0b00000001;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3);
    // Zero rotated in, carry flag was false
    munit_assert_int(rf.DE.l, ==, 0b00000010);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.AF.r = 0b00010000; // set carry flag
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(rf.DE.l, ==, 0b00000101);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    return MUNIT_OK;
}

static MunitResult test_rotate_left_e() {
    // Rotate register E left, inserting carry flag, 0xCB13
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0x13,
        0xCB, 0x13
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    rf.DE.r = 0b00000001;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3);
    // Zero rotated in, carry flag was false
    munit_assert_int(rf.DE.r, ==, 0b00000010);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.AF.r = 0b00010000; // set carry flag
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(rf.DE.r, ==, 0b00000101);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    return MUNIT_OK;
}

static MunitResult test_rotate_left_h() {
    // Rotate register H left, inserting carry flag, 0xCB14
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0x14,
        0xCB, 0x14
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    rf.HL.l = 0b00000001;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3);
    // Zero rotated in, carry flag was false
    munit_assert_int(rf.HL.l, ==, 0b00000010);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.AF.r = 0b00010000; // set carry flag
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(rf.HL.l, ==, 0b00000101);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    return MUNIT_OK;
}

static MunitResult test_rotate_left_l() {
    // Rotate register L left, inserting carry flag, 0xCB15
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0x15,
        0xCB, 0x15
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    rf.HL.r = 0b00000001;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3);
    // Zero rotated in, carry flag was false
    munit_assert_int(rf.HL.r, ==, 0b00000010);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.AF.r = 0b00010000; // set carry flag
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(rf.HL.r, ==, 0b00000101);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    return MUNIT_OK;
}

static MunitResult test_rotate_left_hl_indirect() {
    // Rotate mem value at mem address in register HL
    // left, inserting carry flag, 0xCB16
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0x16,
        0xCB, 0x16
    };
    memcpy(memory, instructions, sizeof(instructions));

    rf.HL.lr = 0xBEEF;
    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    memory[0xBEEF] = 0b00000001;
    // execute (takes 4 cycles)
    clock_cpu(); clock_cpu();
    clock_cpu(); clock_cpu();
    munit_assert_int(rf.PC, ==, 3);
    // Zero rotated in, carry flag was false
    munit_assert_int(memory[0xBEEF], ==, 0b00000010);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.AF.r = 0b00010000; // set carry flag
    // execute (takes 4 cycles)
    clock_cpu(); clock_cpu();
    clock_cpu(); clock_cpu();
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(memory[0xBEEF], ==, 0b00000101);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    return MUNIT_OK;
}

static MunitResult test_rotate_left_a() {
    // Rotate register A left, inserting carry flag, 0xCB17
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0x17,
        0xCB, 0x17
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    rf.AF.l = 0b00000001;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3);
    // Zero rotated in, carry flag was false
    munit_assert_int(rf.AF.l, ==, 0b00000010);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.AF.r = 0b00010000; // set carry flag
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(rf.AF.l, ==, 0b00000101);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    return MUNIT_OK;
}

static MunitResult test_rotate_right_b() {
    // Rotate register B right, inserting carry flag, 0xCB18
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0x18,
        0xCB, 0x18
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    rf.BC.l = 0b00000100;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3);
    // Zero rotated in, carry flag was false
    munit_assert_int(rf.BC.l, ==, 0b00000010);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.AF.r = 0b00010000; // set carry flag
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 5);
    // One rotated in, carry flag was true
    munit_assert_int(rf.BC.l, ==, 0b10000001);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    return MUNIT_OK;
}

static MunitResult test_rotate_right_c() {
    // Rotate register C right, inserting carry flag, 0xCB19
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0x19,
        0xCB, 0x19
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    rf.BC.r = 0b00000100;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3);
    // Zero rotated in, carry flag was false
    munit_assert_int(rf.BC.r, ==, 0b00000010);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.AF.r = 0b00010000; // set carry flag
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 5);
    // One rotated in, carry flag was true
    munit_assert_int(rf.BC.r, ==, 0b10000001);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    return MUNIT_OK;
}

static MunitResult test_rotate_right_d() {
    // Rotate register D right, inserting carry flag, 0xCB1A
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0x1a,
        0xCB, 0x1a
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    rf.DE.l = 0b00000100;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3);
    // Zero rotated in, carry flag was false
    munit_assert_int(rf.DE.l, ==, 0b00000010);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.AF.r = 0b00010000; // set carry flag
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 5);
    // One rotated in, carry flag was true
    munit_assert_int(rf.DE.l, ==, 0b10000001);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    return MUNIT_OK;
}

static MunitResult test_rotate_right_e() {
    // Rotate register E right, inserting carry flag, 0xCB1B
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0x1b,
        0xCB, 0x1b
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    rf.DE.r = 0b00000100;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3);
    // Zero rotated in, carry flag was false
    munit_assert_int(rf.DE.r, ==, 0b00000010);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.AF.r = 0b00010000; // set carry flag
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 5);
    // One rotated in, carry flag was true
    munit_assert_int(rf.DE.r, ==, 0b10000001);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    return MUNIT_OK;
}

static MunitResult test_rotate_right_h() {
    // Rotate register H right, inserting carry flag, 0xCB1C
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0x1c,
        0xCB, 0x1c
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    rf.HL.l = 0b00000100;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3);
    // Zero rotated in, carry flag was false
    munit_assert_int(rf.HL.l, ==, 0b00000010);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.AF.r = 0b00010000; // set carry flag
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 5);
    // One rotated in, carry flag was true
    munit_assert_int(rf.HL.l, ==, 0b10000001);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    return MUNIT_OK;
}

static MunitResult test_rotate_right_l() {
    // Rotate register L right, inserting carry flag, 0xCB1D
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0x1d,
        0xCB, 0x1d
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    rf.HL.r = 0b00000100;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3);
    // Zero rotated in, carry flag was false
    munit_assert_int(rf.HL.r, ==, 0b00000010);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.AF.r = 0b00010000; // set carry flag
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 5);
    // One rotated in, carry flag was true
    munit_assert_int(rf.HL.r, ==, 0b10000001);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    return MUNIT_OK;
}

static MunitResult test_rotate_right_hl_indirect() {
    // Rotate mem value at mem address in register HL
    // right, inserting carry flag, 0xCB1E
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0x1e,
        0xCB, 0x1e
    };
    memcpy(memory, instructions, sizeof(instructions));

    rf.HL.lr = 0xBEEF;
    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    memory[0xBEEF] = 0b00000100;
    // execute (takes 4 cycles)
    clock_cpu(); clock_cpu();
    clock_cpu(); clock_cpu();
    munit_assert_int(rf.PC, ==, 3);
    // Zero rotated in, carry flag was false
    munit_assert_int(memory[0xBEEF], ==, 0b00000010);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.AF.r = 0b00010000; // set carry flag
    // execute (takes 4 cycles)
    clock_cpu(); clock_cpu();
    clock_cpu(); clock_cpu();
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(memory[0xBEEF], ==, 0b10000001);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    return MUNIT_OK;
}

static MunitResult test_rotate_right_a() {
    // Rotate register A right, inserting carry flag, 0xCB1F
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0x1f,
        0xCB, 0x1f
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    rf.AF.l = 0b00000100;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3);
    // Zero rotated in, carry flag was false
    munit_assert_int(rf.AF.l, ==, 0b00000010);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.AF.r = 0b00010000; // set carry flag
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 5);
    // One rotated in, carry flag was true
    munit_assert_int(rf.AF.l, ==, 0b10000001);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);

    return MUNIT_OK;
}

static MunitResult test_shift_left_b() {
    // Shift register B left, 0xCB20
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0x20,
        0xCB, 0x20
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    rf.BC.l = 0b00000001;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(rf.BC.l, ==, 0b00000010);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.BC.l = 0b10000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(rf.BC.l, ==, 0b00000000);
    // Zero and carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10010000);

    return MUNIT_OK;
}

static MunitResult test_shift_left_c() {
    // Shift register C left, 0xCB21
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0x21,
        0xCB, 0x21
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    rf.BC.r = 0b00000001;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(rf.BC.r, ==, 0b00000010);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.BC.r = 0b10000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(rf.BC.r, ==, 0b00000000);
    // Zero and carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10010000);

    return MUNIT_OK;
}

static MunitResult test_shift_left_d() {
    // Shift register D left, 0xCB22
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0x22,
        0xCB, 0x22
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    rf.DE.l = 0b00000001;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(rf.DE.l, ==, 0b00000010);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.DE.l = 0b10000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(rf.DE.l, ==, 0b00000000);
    // Zero and carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10010000);

    return MUNIT_OK;
}

static MunitResult test_shift_left_e() {
    // Shift register E left, 0xCB23
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0x23,
        0xCB, 0x23
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    rf.DE.r = 0b00000001;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(rf.DE.r, ==, 0b00000010);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.DE.r = 0b10000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(rf.DE.r, ==, 0b00000000);
    // Zero and carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10010000);

    return MUNIT_OK;
}

static MunitResult test_shift_left_h() {
    // Shift register H left, 0xCB24
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0x24,
        0xCB, 0x24
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    rf.HL.l = 0b00000001;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(rf.HL.l, ==, 0b00000010);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.HL.l = 0b10000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(rf.HL.l, ==, 0b00000000);
    // Zero and carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10010000);

    return MUNIT_OK;
}

static MunitResult test_shift_left_l() {
    // Shift register L left, 0xCB25
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0x25,
        0xCB, 0x25
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    rf.HL.r = 0b00000001;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(rf.HL.r, ==, 0b00000010);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.HL.r = 0b10000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(rf.HL.r, ==, 0b00000000);
    // Zero and carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10010000);

    return MUNIT_OK;
}

static MunitResult test_shift_left_hl_indirect() {
    // Shift mem value at mem address
    // in register HL left, 0xCB26
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0x26,
        0xCB, 0x26
    };
    memcpy(memory, instructions, sizeof(instructions));

    rf.HL.lr = 0xBEEF;
    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    memory[0xBEEF] = 0b00000001;
    // execute (takes 4 cycles)
    clock_cpu(); clock_cpu();
    clock_cpu(); clock_cpu();
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(memory[0xBEEF], ==, 0b00000010);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    memory[0xBEEF] = 0b10000000;
    // execute (takes 4 cycles)
    clock_cpu(); clock_cpu();
    clock_cpu(); clock_cpu();
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(memory[0xBEEF], ==, 0b00000000);
    // Zero and carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10010000);

    return MUNIT_OK;
}

static MunitResult test_shift_left_a() {
    // Shift register A left, 0xCB27
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0x27,
        0xCB, 0x27
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    rf.AF.l = 0b00000001;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(rf.AF.l, ==, 0b00000010);
    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    rf.AF.l = 0b10000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(rf.AF.l, ==, 0b00000000);
    // Zero and carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10010000);

    return MUNIT_OK;
}

static MunitResult test_shift_right_b() {
    // Shift register B right, 0xCB28
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0x28,
        0xCB, 0x28
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    rf.BC.l = 0b00000001;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(rf.BC.l, ==, 0b00000000);
    // Zero and carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10010000);
    rf.BC.l = 0b10000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(rf.BC.l, ==, 0b11000000);
    // All flags unset
    munit_assert_int(rf.AF.r, ==, 0);

    return MUNIT_OK;
}

static MunitResult test_shift_right_c() {
    // Shift register C right, 0xCB29
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0x29,
        0xCB, 0x29
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    rf.BC.r = 0b00000001;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(rf.BC.r, ==, 0b00000000);
    // Zero and carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10010000);
    rf.BC.r = 0b10000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(rf.BC.r, ==, 0b11000000);
    // All flags unset
    munit_assert_int(rf.AF.r, ==, 0);

    return MUNIT_OK;
}

static MunitResult test_shift_right_d() {
    // Shift register D right, 0xCB2A
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0x2a,
        0xCB, 0x2a
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    rf.DE.l = 0b00000001;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(rf.DE.l, ==, 0b00000000);
    // Zero and carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10010000);
    rf.DE.l = 0b10000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(rf.DE.l, ==, 0b11000000);
    // All flags unset
    munit_assert_int(rf.AF.r, ==, 0);

    return MUNIT_OK;
}

static MunitResult test_shift_right_e() {
    // Shift register E right, 0xCB2B
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0x2b,
        0xCB, 0x2b
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    rf.DE.r = 0b00000001;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(rf.DE.r, ==, 0b00000000);
    // Zero and carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10010000);
    rf.DE.r = 0b10000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(rf.DE.r, ==, 0b11000000);
    // All flags unset
    munit_assert_int(rf.AF.r, ==, 0);

    return MUNIT_OK;
}

static MunitResult test_shift_right_h() {
    // Shift register H right, 0xCB2C
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0x2c,
        0xCB, 0x2c
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    rf.HL.l = 0b00000001;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(rf.HL.l, ==, 0b00000000);
    // Zero and carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10010000);
    rf.HL.l = 0b10000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(rf.HL.l, ==, 0b11000000);
    // All flags unset
    munit_assert_int(rf.AF.r, ==, 0);

    return MUNIT_OK;
}

static MunitResult test_shift_right_l() {
    // Shift register L right, 0xCB2D
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0x2d,
        0xCB, 0x2d
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    rf.HL.r = 0b00000001;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(rf.HL.r, ==, 0b00000000);
    // Zero and carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10010000);
    rf.HL.r = 0b10000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(rf.HL.r, ==, 0b11000000);
    // All flags unset
    munit_assert_int(rf.AF.r, ==, 0);

    return MUNIT_OK;
}

static MunitResult test_shift_right_hl_indirect() {
    // Shift mem value at mem address
    // in register HL right, 0xCB2E
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0x2e,
        0xCB, 0x2e
    };
    memcpy(memory, instructions, sizeof(instructions));

    rf.HL.lr = 0xBEEF;
    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    memory[0xBEEF] = 0b00000001;
    // execute (takes 4 cycles)
    clock_cpu(); clock_cpu();
    clock_cpu(); clock_cpu();
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(memory[0xBEEF], ==, 0);
    // Zero and carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10010000);
    memory[0xBEEF] = 0b10000000;
    // execute (takes 4 cycles)
    clock_cpu(); clock_cpu();
    clock_cpu(); clock_cpu();
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(memory[0xBEEF], ==, 0b11000000);
    // All flags unset
    munit_assert_int(rf.AF.r, ==, 0);

    return MUNIT_OK;
}


static MunitResult test_shift_right_a() {
    // Shift register A right, 0xCB2F
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0x2f,
        0xCB, 0x2f
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    rf.AF.l = 0b00000001;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(rf.AF.l, ==, 0b00000000);
    // Zero and carry flags set
    munit_assert_int(rf.AF.r, ==, 0b10010000);
    rf.AF.l = 0b10000000;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 5);
    munit_assert_int(rf.AF.l, ==, 0b11000000);
    // All flags unset
    munit_assert_int(rf.AF.r, ==, 0);

    return MUNIT_OK;
}

static MunitResult test_swap_b() {
    // Swap hi/lo bytes in register B, 0xCB30
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0x30,
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    rf.BC.l = 0x24;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(rf.BC.l, ==, 0x42);

    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    return MUNIT_OK;
}

static MunitResult test_swap_c() {
    // Swap hi/lo bytes in register C, 0xCB31
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0x31,
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    rf.BC.r = 0x24;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(rf.BC.r, ==, 0x42);

    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    return MUNIT_OK;
}

static MunitResult test_swap_d() {
    // Swap hi/lo bytes in register D, 0xCB32
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0x32,
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    rf.DE.l = 0x24;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(rf.DE.l, ==, 0x42);

    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    return MUNIT_OK;
}

static MunitResult test_swap_e() {
    // Swap hi/lo bytes in register E, 0xCB33
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0x33,
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    rf.DE.r = 0x24;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(rf.DE.r, ==, 0x42);

    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    return MUNIT_OK;
}

static MunitResult test_swap_h() {
    // Swap hi/lo bytes in register H, 0xCB34
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0x34,
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    rf.HL.l = 0x24;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(rf.HL.l, ==, 0x42);

    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    return MUNIT_OK;
}

static MunitResult test_swap_l() {
    // Swap hi/lo bytes in register L, 0xCB35
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0x35,
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    rf.HL.r = 0x24;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(rf.HL.r, ==, 0x42);

    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    return MUNIT_OK;
}

static MunitResult test_swap_hl_indirect() {
    // Swap hi/lo bytes in mem value at mem address
    // in register HL, 0xCB36
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0x36,
    };
    memcpy(memory, instructions, sizeof(instructions));

    rf.HL.lr = 0xBEEF;
    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    memory[0xBEEF] = 0x24;
    // execute (takes 4 cycles)
    clock_cpu(); clock_cpu();
    clock_cpu(); clock_cpu();
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(memory[0xBEEF], ==, 0x42);

    // CPU flags untouched
    munit_assert_int(rf.AF.r, ==, 0);
    return MUNIT_OK;
}


static MunitResult test_swap_a() {
    // Swap hi/lo bytes in register A, 0xCB37
    register_file blank_rf;
    memset(&blank_rf, 0, sizeof(blank_rf));
    assert_register_file_equal(blank_rf, rf);

    uint8_t instructions[] = {
        0xCB, 0x37,
    };
    memcpy(memory, instructions, sizeof(instructions));

    munit_assert_int(rf.PC, ==, 0);
    clock_cpu(); // initial load
    munit_assert_int(rf.PC, ==, 1);
    rf.AF.l = 0x24;
    clock_cpu(); clock_cpu(); // execute (takes 2 cycles)
    munit_assert_int(rf.PC, ==, 3);
    munit_assert_int(rf.AF.l, ==, 0x42);

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
    {
        "/rotate_left_circular_b",
        test_rotate_left_circular_b,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/rotate_left_circular_c",
        test_rotate_left_circular_c,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/rotate_left_circular_d",
        test_rotate_left_circular_d,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/rotate_left_circular_e",
        test_rotate_left_circular_e,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/rotate_left_circular_h",
        test_rotate_left_circular_h,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/rotate_left_circular_l",
        test_rotate_left_circular_l,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/rotate_left_circular_hl_indirect",
        test_rotate_left_circular_hl_indirect,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/rotate_left_circular_a",
        test_rotate_left_circular_a,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/rotate_right_circular_b",
        test_rotate_right_circular_b,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/rotate_right_circular_c",
        test_rotate_right_circular_c,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/rotate_right_circular_d",
        test_rotate_right_circular_d,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/rotate_right_circular_e",
        test_rotate_right_circular_e,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/rotate_right_circular_h",
        test_rotate_right_circular_h,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/rotate_right_circular_l",
        test_rotate_right_circular_l,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/rotate_right_circular_hl_indirect",
        test_rotate_right_circular_hl_indirect,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/rotate_right_circular_a",
        test_rotate_right_circular_a,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/rotate_left_b",
        test_rotate_left_b,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/rotate_left_c",
        test_rotate_left_c,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/rotate_left_d",
        test_rotate_left_d,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/rotate_left_e",
        test_rotate_left_e,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/rotate_left_h",
        test_rotate_left_h,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/rotate_left_l",
        test_rotate_left_l,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/rotate_left_hl_indirect",
        test_rotate_left_hl_indirect,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/rotate_left_a",
        test_rotate_left_a,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/rotate_right_b",
        test_rotate_right_b,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/rotate_right_c",
        test_rotate_right_c,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/rotate_right_d",
        test_rotate_right_d,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/rotate_right_e",
        test_rotate_right_e,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/rotate_right_h",
        test_rotate_right_h,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/rotate_right_l",
        test_rotate_right_l,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/rotate_right_hl_indirect",
        test_rotate_right_hl_indirect,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/rotate_right_a",
        test_rotate_right_a,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/shift_left_b",
        test_shift_left_b,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/shift_left_c",
        test_shift_left_c,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/shift_left_d",
        test_shift_left_d,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/shift_left_e",
        test_shift_left_e,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/shift_left_h",
        test_shift_left_h,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/shift_left_l",
        test_shift_left_l,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/shift_left_hl_indirect",
        test_shift_left_hl_indirect,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/shift_left_a",
        test_shift_left_a,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/shift_right_b",
        test_shift_right_b,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/shift_right_c",
        test_shift_right_c,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/shift_right_d",
        test_shift_right_d,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/shift_right_e",
        test_shift_right_e,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/shift_right_h",
        test_shift_right_h,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/shift_right_l",
        test_shift_right_l,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/shift_right_hl_indirect",
        test_shift_right_hl_indirect,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/shift_right_a",
        test_shift_right_a,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/swap_b",
        test_swap_b,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/swap_c",
        test_swap_c,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/swap_d",
        test_swap_d,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/swap_e",
        test_swap_e,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/swap_h",
        test_swap_h,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/swap_l",
        test_swap_l,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/swap_hl_indirect",
        test_swap_hl_indirect,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/swap_a",
        test_swap_a,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

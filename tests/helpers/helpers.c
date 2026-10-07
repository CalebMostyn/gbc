#include "helpers.h"
#include "memory_bus.h"
#include "munit.h"

void assert_register_file_equal(register_file a, register_file b) {
    munit_assert_int(a.IME, ==, b.IME);
    munit_assert_int(a.AF.lr, ==, b.AF.lr);
    munit_assert_int(a.BC.lr, ==, b.BC.lr);
    munit_assert_int(a.DE.lr, ==, b.DE.lr);
    munit_assert_int(a.HL.lr, ==, b.HL.lr);
    munit_assert_int(a.PC, ==, b.PC);
    munit_assert_int(a.SP, ==, b.SP);
}

void write_instructions_to_memory(uint16_t start_addr, const uint8_t* inst, size_t size) {
    for (size_t ii = 0; ii < size; ii++) {
        dma_write(start_addr + ii, inst[ii]);
    }
}

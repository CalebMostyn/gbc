#include "munit.h"
#include "cpu.h"

static MunitResult test_pc_increment() {
    munit_assert_int(rf.PC, ==, 0);
    clock_cpu();
    munit_assert_int(rf.PC, ==, 1);
    return MUNIT_OK;
}

static MunitResult test_cpu_halt() {
    memory[0] = 0x76;
    munit_assert_int(rf.PC, ==, 0);
    clock_cpu();
    munit_assert_int(rf.PC, ==, 1);

    clock_cpu();
    munit_assert_int(rf.PC, ==, 2);
    munit_assert_true(cpu_halted);

    clock_cpu();
    munit_assert_int(rf.PC, ==, 2);
    clock_cpu();
    munit_assert_int(rf.PC, ==, 2);
    clock_cpu();
    munit_assert_int(rf.PC, ==, 2);
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
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

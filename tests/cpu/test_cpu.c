#include "munit.h"

extern MunitTest load_eight_bit_tests[];
extern MunitTest load_sixteen_bit_tests[];
extern MunitTest add_tests[];
extern MunitTest subtract_tests[];
extern MunitTest compare_tests[];
extern MunitTest bitwise_tests[];
extern MunitTest arithmetic_sixteen_bit_tests[];
extern MunitTest rotate_tests[];
extern MunitTest control_flow_tests[];
extern MunitTest misc_instructions_tests[];
extern MunitTest test_rom_tests[];

static MunitSuite cpu_arithmetic_suite[] = {
    {
        "/add",
        add_tests,
        NULL,
        0,
        MUNIT_SUITE_OPTION_NONE
    },
    {
        "/sub",
        subtract_tests,
        NULL,
        0,
        MUNIT_SUITE_OPTION_NONE
    },
    {
        "/cp",
        compare_tests,
        NULL,
        0,
        MUNIT_SUITE_OPTION_NONE
    },
    {
        "/sixteen_bit",
        arithmetic_sixteen_bit_tests,
        NULL,
        0,
        MUNIT_SUITE_OPTION_NONE
    },
    {
        "/bitwise",
        bitwise_tests,
        NULL,
        0,
        MUNIT_SUITE_OPTION_NONE
    },
    {
        NULL,
        NULL,
        NULL,
        0,
        MUNIT_SUITE_OPTION_NONE
    }
};

static MunitSuite cpu_load_suite[] = {
    {
        "/eight_bit",
        load_eight_bit_tests,
        NULL,
        0,
        MUNIT_SUITE_OPTION_NONE
    },
    {
        "/sixteen_bit",
        load_sixteen_bit_tests,
        NULL,
        0,
        MUNIT_SUITE_OPTION_NONE
    },
    {
        NULL,
        NULL,
        NULL,
        0,
        MUNIT_SUITE_OPTION_NONE
    }
};

MunitSuite cpu_suite[] = {
    {
        "/load",
        NULL,
        cpu_load_suite,
        0,
        MUNIT_SUITE_OPTION_NONE
    },
    {
        "/misc",
        misc_instructions_tests,
        NULL,
        0,
        MUNIT_SUITE_OPTION_NONE
    },
    {
        "/control_flow",
        control_flow_tests,
        NULL,
        0,
        MUNIT_SUITE_OPTION_NONE
    },
    {
        "/rotate",
        rotate_tests,
        NULL,
        0,
        MUNIT_SUITE_OPTION_NONE
    },
    {
        "/arithmetic",
        NULL,
        cpu_arithmetic_suite,
        0,
        MUNIT_SUITE_OPTION_NONE
    },
    {
        "/test_rom",
        test_rom_tests,
        NULL,
        0,
        MUNIT_SUITE_OPTION_NONE
    },
    {
        NULL,
        NULL,
        NULL,
        0,
        MUNIT_SUITE_OPTION_NONE
    }
};


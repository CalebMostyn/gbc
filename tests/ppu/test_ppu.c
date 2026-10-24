#include "munit.h"

extern MunitTest ppu_test_rom_tests[];

MunitSuite ppu_suite[] = {
    {
        "/test_rom",
        ppu_test_rom_tests,
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

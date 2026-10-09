#include "memory_bus.h"
#include "munit.h"
#include "helpers.h"
#include "cpu.h"
#include <stdio.h>

uint64_t MAX_M_CYCLES = 100000000;
char BLARGG_ROM_PATH[] = "roms/gb-test-roms/cpu_instrs/individual/";
char file_path[50] = "";

static MunitResult run_blargg_test(char* file_name) {
    strcpy(file_path, "");
    strcat(file_path, BLARGG_ROM_PATH);
    strcat(file_path, file_name);
    FILE* rom = fopen(file_path, "rb");
    if (!rom) {
        return MUNIT_FAIL;
    }

    load_cartridge_rom(rom, 0x0000);
    fclose(rom);

    init_register_file_to_post_bootloader_state(&rf);

    for (uint64_t cycles = 0; cycles < MAX_M_CYCLES; cycles++) {
        clock_cpu();

        if (strstr(serial_output, "Passed")) {
            return MUNIT_OK;
        }

        if (strstr(serial_output, "Failed")) {
            fprintf(stderr, "Failed, Blargg output: %s\n", serial_output);
            return MUNIT_FAIL;
        }
    }

    // timeout
    fprintf(stderr, "Timed out, Blargg output: %s\n", serial_output);
    return MUNIT_FAIL;
}

static MunitResult test_blargg_cpu_instrs_01() {
    return run_blargg_test("01-special.gb");
}

static MunitResult test_blargg_cpu_instrs_02() {
    return run_blargg_test("02-interrupts.gb");
}

static MunitResult test_blargg_cpu_instrs_03() {
    return run_blargg_test("03-op sp,hl.gb");
}

static MunitResult test_blargg_cpu_instrs_04() {
    return run_blargg_test("04-op r,imm.gb");
}

static MunitResult test_blargg_cpu_instrs_05() {
    return run_blargg_test("05-op rp.gb");
}

static MunitResult test_blargg_cpu_instrs_06() {
    return run_blargg_test("06-ld r,r.gb");
}

static MunitResult test_blargg_cpu_instrs_07() {
    return run_blargg_test("07-jr,jp,call,ret,rst.gb");
}

static MunitResult test_blargg_cpu_instrs_08() {
    return run_blargg_test("08-misc instrs.gb");
}

static MunitResult test_blargg_cpu_instrs_09() {
    return run_blargg_test("09-op r,r.gb");
}

static MunitResult test_blargg_cpu_instrs_10() {
    return run_blargg_test("10-bit ops.gb");
}

static MunitResult test_blargg_cpu_instrs_11() {
    return run_blargg_test("11-op a,(hl).gb");
}

MunitTest test_rom_tests[] = {
    {
        "/test_blargg_cpu_instrs_01",
        test_blargg_cpu_instrs_01,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/test_blargg_cpu_instrs_02",
        test_blargg_cpu_instrs_02,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/test_blargg_cpu_instrs_03",
        test_blargg_cpu_instrs_03,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/test_blargg_cpu_instrs_04",
        test_blargg_cpu_instrs_04,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/test_blargg_cpu_instrs_05",
        test_blargg_cpu_instrs_05,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/test_blargg_cpu_instrs_06",
        test_blargg_cpu_instrs_06,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/test_blargg_cpu_instrs_07",
        test_blargg_cpu_instrs_07,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/test_blargg_cpu_instrs_08",
        test_blargg_cpu_instrs_08,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/test_blargg_cpu_instrs_09",
        test_blargg_cpu_instrs_09,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/test_blargg_cpu_instrs_10",
        test_blargg_cpu_instrs_10,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/test_blargg_cpu_instrs_11",
        test_blargg_cpu_instrs_11,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

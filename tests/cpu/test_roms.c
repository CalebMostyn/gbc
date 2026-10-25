#include "emu_core.h"
#include "memory_bus.h"
#include "munit.h"
#include "helpers.h"
#include <stdio.h>

static MunitResult run_blargg_test(char* dir, char* file_name, int max_m_cycles) {
    FILE* rom = fopen(build_file_path(dir, file_name, ".gb"), "rb");
    if (!rom) {
        return MUNIT_FAIL;
    }

    load_cartridge_rom(rom, 0x0000);
    fclose(rom);

    init_register_file_to_post_bootloader_state(&rf);

    for (uint64_t cycles = 0; cycles < max_m_cycles; cycles++) {
        emulate_clock_cycle();

        if (strstr(serial_output, "Passed")) {
            return MUNIT_OK;
        }

        if (strstr(serial_output, "Failed")) {
            printf("Failed, Blargg output: %s\n", serial_output);
            return MUNIT_FAIL;
        }
    }

    // timeout
    printf("Timed out, Blargg output: %s\n", serial_output);
    return MUNIT_FAIL;
}

static MunitResult test_blargg_cpu_instrs_01() {
    return run_blargg_test("tests/gb-test-roms/cpu_instrs/individual/", "01-special", 2500000);
}

static MunitResult test_blargg_cpu_instrs_02() {
    return run_blargg_test("tests/gb-test-roms/cpu_instrs/individual/", "02-interrupts", 500000);
}

static MunitResult test_blargg_cpu_instrs_03() {
    return run_blargg_test("tests/gb-test-roms/cpu_instrs/individual/", "03-op sp,hl", 2500000);
}

static MunitResult test_blargg_cpu_instrs_04() {
    return run_blargg_test("tests/gb-test-roms/cpu_instrs/individual/", "04-op r,imm", 3000000);
}

static MunitResult test_blargg_cpu_instrs_05() {
    return run_blargg_test("tests/gb-test-roms/cpu_instrs/individual/", "05-op rp", 4000000);
}

static MunitResult test_blargg_cpu_instrs_06() {
    return run_blargg_test("tests/gb-test-roms/cpu_instrs/individual/", "06-ld r,r", 700000);
}

static MunitResult test_blargg_cpu_instrs_07() {
    return run_blargg_test("tests/gb-test-roms/cpu_instrs/individual/", "07-jr,jp,call,ret,rst", 800000);
}

static MunitResult test_blargg_cpu_instrs_08() {
    return run_blargg_test("tests/gb-test-roms/cpu_instrs/individual/", "08-misc instrs", 600000);
}

static MunitResult test_blargg_cpu_instrs_09() {
    return run_blargg_test("tests/gb-test-roms/cpu_instrs/individual/", "09-op r,r", 9750000);
}

static MunitResult test_blargg_cpu_instrs_10() {
    return run_blargg_test("tests/gb-test-roms/cpu_instrs/individual/", "10-bit ops", 14750000);
}

static MunitResult test_blargg_cpu_instrs_11() {
    return run_blargg_test("tests/gb-test-roms/cpu_instrs/individual/", "11-op a,(hl)", 18500000);
}

static MunitResult test_blargg_instr_timing() {
    return run_blargg_test("tests/gb-test-roms/instr_timing/", "instr_timing", 700000);
}

MunitTest cpu_test_rom_tests[] = {
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
    {
        "/test_blargg_instr_timing",
        test_blargg_instr_timing,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

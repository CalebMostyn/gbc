#include "emu_core.h"
#include "lodepng/lodepng.h"
#include "memory_bus.h"
#include "munit.h"
#include "helpers.h"
#include "ppu.h"
#include "test_lcd.h"
#include <stdio.h>


static MunitResult run_ppu_test_rom(char* dir, char* file_name, int max_m_cycles) {
    FILE* rom = fopen(build_file_path(dir, file_name, ".gb"), "rb");
    munit_assert_ptr_not_null(rom);
    if (!rom) {
        return MUNIT_FAIL;
    }

    load_cartridge_rom(rom, 0x0000);
    fclose(rom);

    init_register_file_to_post_bootloader_state(&rf);

    for (uint64_t cycles = 0; cycles < max_m_cycles; cycles++) {
        emulate_clock_cycle();
    }
    uint8_t* im_buf = render_lcd_grayscale_rgba8();
#if WRITE_TEST_IMAGES
    write_lcd_png(build_file_path("build/tests/", file_name, ".png"), im_buf);
#endif

    uint8_t* expected_buf = 0;
    unsigned int expected_width, expected_height;
    unsigned int err = lodepng_decode32_file(&expected_buf, &expected_width, &expected_height, build_file_path(dir, file_name, ".png"));

    // validate expected image
    munit_assert_int(err, ==, 0);
    munit_assert_int(expected_width, ==, LCD_RES_X);
    munit_assert_int(expected_height, ==, LCD_RES_Y);

    bool equal = lcd_equal(im_buf, expected_buf);
#if WRITE_TEST_IMAGES
    uint8_t* diff_img_buf = diff_lcd(im_buf, expected_buf);
    write_lcd_png(build_file_path("build/tests/", file_name, "-diff.png"), diff_img_buf);
    free(diff_img_buf);
#endif
    munit_assert_true(equal);

    free(im_buf);
    return MUNIT_OK;
}

static MunitResult test_dmg_acid2() {
    // rendered in about 10 frames
    return run_ppu_test_rom("tests/test-roms/", "dmg-acid2", 200000);
}

MunitTest ppu_test_rom_tests[] = {
    {
        "/test_dmg_acid2",
        test_dmg_acid2,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

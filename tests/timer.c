#include "timer.h"
#include "memory_bus.h"
#include "munit.h"
#include <math.h>
#include <stdint.h>

static MunitResult test_div() {
    uint8_t expected_div_value = 0;
    for (uint64_t ii = 0; ii < pow(2,16);) {
        clock_timer();
        if (++ii % 64 == 0) {
            // increases every 256 T-cycles AKA 64 M-cycles
            expected_div_value++;
        }
        munit_assert_int(timer_reg_div, ==, expected_div_value);
    }
    return MUNIT_OK;
}

static MunitResult test_tima_clock_select_0() {
    timer_reg_tac = 0x04; // enable tima, clock select == 0
    timer_reg_tma = 42;
    uint8_t expected_tima_value = 0;
    for (uint64_t ii = 0; ii < pow(2,16);) {
        clock_timer();
        if (++ii % 256 == 0) {
            // increases every 1028 T-cycles AKA 256 M-cycles
            expected_tima_value++;
            if (expected_tima_value == 0) {
                munit_assert_true(tima_pending_reload);
                munit_assert_int(dma_read(0xFF0F), ==, 0);
                clock_timer(); ii++;
                munit_assert_int(dma_read(0xFF0F), ==, 0x04);
                dma_write(0xFF0F, 0); // emulate clearing interrupt
                munit_assert_true(tima_reloading);
                expected_tima_value = 42;
            }
        }
        munit_assert_int(timer_reg_tima, ==, expected_tima_value);
    }
    return MUNIT_OK;
}

static MunitResult test_tima_clock_select_1() {
    timer_reg_tac = 0x05; // enable tima, clock select == 1
    timer_reg_tma = 42;
    uint8_t expected_tima_value = 0;
    for (uint64_t ii = 0; ii < pow(2,16);) {
        clock_timer();
        if (++ii % 4 == 0) {
            // increases every 16 T-cycles AKA 4 M-cycles
            expected_tima_value++;
            if (expected_tima_value == 0) {
                munit_assert_true(tima_pending_reload);
                munit_assert_int(dma_read(0xFF0F), ==, 0);
                clock_timer(); ii++;
                munit_assert_int(dma_read(0xFF0F), ==, 0x04);
                dma_write(0xFF0F, 0); // emulate clearing interrupt
                munit_assert_true(tima_reloading);
                expected_tima_value = 42;
            }
        }
        munit_assert_int(timer_reg_tima, ==, expected_tima_value);
    }
    return MUNIT_OK;
}

static MunitResult test_tima_clock_select_2() {
    timer_reg_tac = 0x06; // enable tima, clock select == 2
    timer_reg_tma = 42;
    uint8_t expected_tima_value = 0;
    for (uint64_t ii = 0; ii < pow(2,16);) {
        clock_timer();
        if (++ii % 16 == 0) {
            // increases every 64 T-cycles AKA 16 M-cycles
            expected_tima_value++;
            if (expected_tima_value == 0) {
                munit_assert_true(tima_pending_reload);
                munit_assert_int(dma_read(0xFF0F), ==, 0);
                clock_timer(); ii++;
                munit_assert_int(dma_read(0xFF0F), ==, 0x04);
                dma_write(0xFF0F, 0); // emulate clearing interrupt
                munit_assert_true(tima_reloading);
                expected_tima_value = 42;
            }
        }
        munit_assert_int(timer_reg_tima, ==, expected_tima_value);
    }
    return MUNIT_OK;
}

static MunitResult test_tima_clock_select_3() {
    timer_reg_tac = 0x07; // enable tima, clock select == 3
    timer_reg_tma = 42;
    uint8_t expected_tima_value = 0;
    for (uint64_t ii = 0; ii < pow(2,16);) {
        clock_timer();
        if (++ii % 64 == 0) {
            // increases every 256 T-cycles AKA 64 M-cycles
            expected_tima_value++;
            if (expected_tima_value == 0) {
                munit_assert_true(tima_pending_reload);
                munit_assert_int(dma_read(0xFF0F), ==, 0);
                clock_timer(); ii++;
                munit_assert_int(dma_read(0xFF0F), ==, 0x04);
                dma_write(0xFF0F, 0); // emulate clearing interrupt
                munit_assert_true(tima_reloading);
                expected_tima_value = 42;
            }
        }
        munit_assert_int(timer_reg_tima, ==, expected_tima_value);
    }
    return MUNIT_OK;
}

static MunitResult test_tima_disabled() {
    for (uint64_t ii = 0; ii < pow(2,16); ii++) {
        clock_timer();
        munit_assert_int(timer_reg_tima, ==, 0);
    }
    return MUNIT_OK;
}

static MunitResult test_tma_write_during_tima_reload() {
    timer_reg_tac = 0x05; // enable tima, clock select == 0
    timer_reg_tma = 24;
    timer_reg_tima = 0xFF; // about to overflow
    for (uint64_t ii = 0; ii < 4; ii++) {
        clock_timer();
    }
    munit_assert_true(tima_pending_reload);
    munit_assert_int(dma_read(0xFF0F), ==, 0);
    clock_timer();
    munit_assert_int(dma_read(0xFF0F), ==, 0x04);
    munit_assert_true(tima_reloading);

    munit_assert_int(timer_reg_tima, ==, 24);
    memory_bus_write(TIMER_REG_TIMA, 77); // Ignored
    munit_assert_int(timer_reg_tima, ==, 24);
    memory_bus_write(TIMER_REG_TMA, 42); // Overwrites TIMA
    munit_assert_int(timer_reg_tima, ==, 42);

    return MUNIT_OK;
}


MunitTest timer_tests[] = {
    {
        "/div",
        test_div,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/test_tima_clock_select_0",
        test_tima_clock_select_0,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/test_tima_clock_select_1",
        test_tima_clock_select_1,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/test_tima_clock_select_2",
        test_tima_clock_select_2,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/test_tima_clock_select_3",
        test_tima_clock_select_3,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/test_tima_disabled",
        test_tima_disabled,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/test_tma_write_during_tima_reload",
        test_tma_write_during_tima_reload,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

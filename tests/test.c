#include "munit.h"

extern MunitTest cpu_tests[];

static MunitSuite suites[] = {
    {
        "/cpu",
        cpu_tests,
        NULL,
        1,
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

static MunitSuite suite = {
    "",
    NULL,
    suites,
    0,
    MUNIT_SUITE_OPTION_NONE
};

int
main(int argc, char *argv[])
{
    return munit_suite_main(&suite, NULL, argc, argv);
}

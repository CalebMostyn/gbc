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

void init_register_file_to_post_bootloader_state(register_file* rf) {
    (*rf).AF.lr = 0x01B0;
    (*rf).BC.lr = 0x0013;
    (*rf).DE.lr = 0x00D8;
    (*rf).HL.lr = 0x014D;
    (*rf).SP = 0xFFFE;
    (*rf).PC = 0x0100;
}

char* build_file_path(char* prefix, char* path, char* ext) {
    char* result = NULL;
    size_t result_len = 0;
    if (prefix) {
        result_len += strlen(prefix);
    } else {
        prefix = "";
    }
    if (path) {
        result_len += strlen(path);
    } else {
        path = "";
    }
    if (ext) {
        result_len += strlen(ext);
    } else {
        ext = "";
    }
    if (result_len > 0) {
        result_len++; // null terminator
        result = (char*)malloc(result_len);
        snprintf(result, result_len, "%s%s%s", prefix, path, ext);
    }
    return result;
}

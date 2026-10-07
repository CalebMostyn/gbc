#include "cpu.h"
#include <stddef.h>

void assert_register_file_equal(register_file a, register_file b);

void write_instructions_to_memory(uint16_t start_addr, const uint8_t* inst, size_t size);

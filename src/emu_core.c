#include "emu_core.h"
#include "cpu.h"
#include "ppu.h"
#include "apu.h"

// Update and draw game frame
void emulate_clock_cycle() {
    // Processor
    clock_cpu();
    // Graphics
    clock_ppu();
    // Audio
    clock_apu();
}


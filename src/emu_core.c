#include "emu_core.h"
#include "cpu.h"
#include "ppu.h"
#include "apu.h"
#include "timer.h"

// Update and draw game frame
void emulate_clock_cycle() {
    // Timers
    clock_timer();
    // Processor
    clock_cpu();
    // Graphics
    clock_ppu();
    // Audio
    clock_apu();
}


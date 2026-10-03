#ifndef EMU_CORE_H
#define EMU_CORE_H

#include "cpu.h"
#include "ppu.h"
#include "apu.h"

/* ---- Constants ---- */
// TODO: I think a 'clock' cycle actually takes
// 4 ticks of the on-chip clock, so this is 4x speed?
#define CPU_CLOCK_HZ 4194304     // 4.19 MHz

void emulate_clock_cycle(); // emulate a clock tick across all components

#endif // EMU_CORE_H

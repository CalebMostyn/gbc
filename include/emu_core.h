#ifndef EMU_CORE_H
#define EMU_CORE_H

/* ---- Constants ---- */
#define T_CYCLE_HZ 4194304     // 4.19 MHz
#define M_CYCLE_HZ (T_CYCLE_HZ / 4) // 1.04 MHz

void emulate_clock_cycle(); // emulate a clock tick across all components

#endif // EMU_CORE_H

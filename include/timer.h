#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include <stdbool.h>
#include "emu_core.h"

#define TIMER_REG_DIV 0xFF04
#define TIMER_REG_TIMA 0xFF05
#define TIMER_REG_TMA 0xFF06
#define TIMER_REG_TAC 0xFF07

#define DIV_TICK_HZ 16384
#define DIV_M_CYCLES_PER_TICK (M_CYCLE_HZ / DIV_TICK_HZ)

#define TIMA_TICK_HZ_0 4096
#define TIMA_0_M_CYCLES_PER_TICK (M_CYCLE_HZ / TIMA_TICK_HZ_0)
#define TIMA_TICK_HZ_1 262144
#define TIMA_1_M_CYCLES_PER_TICK (M_CYCLE_HZ / TIMA_TICK_HZ_1)
#define TIMA_TICK_HZ_2 65536
#define TIMA_2_M_CYCLES_PER_TICK (M_CYCLE_HZ / TIMA_TICK_HZ_2)
#define TIMA_TICK_HZ_3 16384
#define TIMA_3_M_CYCLES_PER_TICK (M_CYCLE_HZ / TIMA_TICK_HZ_3)

extern bool tima_pending_reload;
extern bool tima_reloading;

extern uint8_t timer_reg_div;
extern uint8_t timer_reg_tima;
extern uint8_t timer_reg_tma;
extern uint8_t timer_reg_tac;

void clock_timer(); // emulate a clock tick on the timers 

#endif // TIMER_H

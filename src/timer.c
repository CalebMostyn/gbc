#include "timer.h"
#include "memory_bus.h"

uint8_t timer_reg_div = 0;
uint8_t timer_reg_tima = 0;
uint8_t timer_reg_tma = 0;
uint8_t timer_reg_tac = 0;

bool tima_pending_reload = false;
bool tima_reloading = false;

static unsigned int div_num_ticks = 0;
static unsigned int tima_num_ticks = 0;
void clock_timer() {
    // div always ticks
    if (++div_num_ticks >= (DIV_M_CYCLES_PER_TICK)) {
        timer_reg_div++;
        div_num_ticks = 0;
    }
    tima_reloading = false;
    if (tima_pending_reload) {
        tima_pending_reload = false;
        timer_reg_tima = timer_reg_tma;
        tima_num_ticks = 0;
        tima_reloading = true;
        // write bit 2 to IF
        memory_bus_write(0xFF0F, memory_bus_read(0xFF0F) | 0x04);
    }
    if (timer_reg_tac & 0x04) {
        uint8_t clock_select = timer_reg_tac & 0x03;
        uint8_t prev_tima = timer_reg_tima;
        switch (clock_select) {
            case 0:
                if (++tima_num_ticks >= TIMA_0_M_CYCLES_PER_TICK) {
                    timer_reg_tima++;
                    tima_num_ticks = 0;
                }
                break;
            case 1:
                if (++tima_num_ticks >= TIMA_1_M_CYCLES_PER_TICK) {
                    timer_reg_tima++;
                    tima_num_ticks = 0;
                }
                break;
            case 2:
                if (++tima_num_ticks >= TIMA_2_M_CYCLES_PER_TICK) {
                    timer_reg_tima++;
                    tima_num_ticks = 0;
                }
                break;
            case 3:
                if (++tima_num_ticks >= TIMA_3_M_CYCLES_PER_TICK) {
                    timer_reg_tima++;
                    tima_num_ticks = 0;
                }
                break;
        }
        if (prev_tima != 0 && timer_reg_tima == 0) {
            // we just overflowed, mark reload pending
            tima_pending_reload = true;
        }
    }
}

#include "cpu.h"
#include <stdint.h>
#include "raylib.h"

register_file rf;

int state = 0;
void clock_cpu() {
    // TODO: CPU functionality
    switch (state) {
        case 0:
            rf.AF.l = (uint8_t)42;
            break;
        case 1:
            rf.AF.r = (uint8_t)42;
            break;
        case 10:
            TraceLog(LOG_INFO, "Reg A: %d, Reg F: %d, AF: %d", rf.AF.l, rf.AF.r, rf.AF.lr);
            break;
    }
    state++;
}

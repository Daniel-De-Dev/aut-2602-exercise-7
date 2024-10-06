#ifndef TIMER_H
#define TIMER_H

#include <avr/io.h>

void set_main_clk_16mhz();

void init_TCA0(uint16_t top_value);

#endif  /* TIMER_H */
#include <avr/io.h>
#include "timer.h"

void set_main_clk_16mhz() {
    // Edit the protected register using a helper function that performs this safely
    ccp_write_io(
        (void *) & CLKCTRL.OSCHFCTRLA, 
        CLKCTRL_FRQSEL_16M_gc | (0 << CLKCTRL_AUTOTUNE_bp) | (0 << CLKCTRL_RUNSTDBY_bp) 
    );
}

void init_TCA0(uint16_t top_value) {
    // Set the top value to the provided amount
    TCA0.SINGLE.PER = top_value;

    // Enable the peripheral and set prescaler to DIV64
    TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV64_gc | (0b1 << TCA_SINGLE_ENABLE_bp);

    // Enable overflow interrupt
    TCA0.SINGLE.INTCTRL = TCA_SINGLE_OVF_bm;
}

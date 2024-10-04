/*
 * File:   main.c
 * Author: Daniel
 *
 * Created on September 20, 2024, 1:43 PM
 */

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "clock.h"
#include "usart.h"

// Declare a volatile flag to control temperature printing
volatile uint8_t print_temp = 1;

/**
 * @brief Sends a character via USART3 (used for stdout redirection).
 * 
 * This function is used by the `printf` function to send characters
 * through USART3, allowing the program to redirect its output to a serial interface.
 * 
 * @param c The character to send.
 * @param _ The output stream (ignored in this case).
 * @return Always returns 0.
 */
int usart3_send_char(char c, FILE* _) {
    usart_transmit_char(&USART3, c);
    return 0;
}

// Redirect stdout to USART3
FILE new_std_out = FDEV_SETUP_STREAM(usart3_send_char, NULL, _FDEV_SETUP_WRITE);

/**
 * @brief ISR for USART3 receive complete interrupt.
 * 
 * This interrupt is triggered when a character is received via USART3.
 * Depending on the character, different actions are taken:
 * - 's': Print clock settings.
 * - 't': Toggle temperature display on/off.
 */
ISR(USART3_RXC_vect) {
    char c = usart_receive_char(&USART3);
    
    if (c == 's') {
        printf("F_CPU: %lu Hz,\t", F_CPU);
        printf("CLK_PER: %lu Hz,\t", get_clk_per());
        printf("\n");
    } else if (c == 't') {
        print_temp ^= 1;    // Toggle print_temp flag
    }
}

/**
 * @brief Main function to initialize USART, ADC, and print temperature data.
 * 
 * This function initializes the USART for communication and the ADC to measure the
 * internal temperature sensor. It also enables interrupts for USART3 and controls the
 * temperature output based on received commands.
 */
void main(void) {
    // Initialize USART3 for communication
    usart_init(&USART3, &PORTB);
    
    // Enable global interrupts
    sei();

    // Main loop: Continuously print temperature when enabled
    while(1) {
        
    }
}
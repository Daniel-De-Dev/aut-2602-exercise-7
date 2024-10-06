/*
 * File:   main.c
 * Author: Daniel
 *
 * Created on October 6, 2024, 1:43 PM
 */

#include <stdio.h>
#include <avr/io.h>
#include <stdint.h> 
#include <avr/interrupt.h>
#include "clock.h"
#include "usart.h"
#include "inttypes.h"
#include "timer.h"

// Constants for limits
#define MIN_FREQUENCY 1000   // 1 kHz
#define MAX_FREQUENCY 5000   // 5 kHz
#define BUZZER_PIN 2         // Buzzer connected to PORTF Pin 2
#define LED_PIN 3            // LED connected to PORTB Pin 3

volatile uint16_t overflow_counter = 0;  // Counter for overflows
uint32_t frequency = MIN_FREQUENCY;      // Start at 1000 Hz
int8_t direction = 1;                    // Increment direction (1 for up, -1 for down)


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
 * - 's': Print clock settings and current state.
 * - 'o': Toggle buzzer on/off.
 */
ISR(USART3_RXC_vect) {
    char c = usart_receive_char(&USART3);
    if (c == 's') {
        // Print clock settings and frequency information
        printf("F_CPU: %lu Hz,\t", F_CPU);
        printf("CLK_PER: %lu Hz,\t", get_clk_per());
        printf("Overflow Counter: %u\t", overflow_counter);
        printf("TCA0 Count: %u\n", TCA0.SINGLE.CNT);
        printf("Current Frequency: %u Hz\n", frequency);
    } else if (c == 'o') {
        // Toggle buzzer on/off
        PORTF.DIRTGL = (1 << BUZZER_PIN);
    }
}

/**
 * @brief Set TCA timer frequency.
 *
 * @param frequency_hz Desired frequency in Hz.
 */
void set_TCA_hz(uint32_t frequency_hz) {
    uint32_t top_value = (F_CPU / (64 * frequency_hz)) - 1;
    TCA0.SINGLE.PER = top_value;
}

/**
 * @brief ISR for TCA0 overflow interrupt.
 *
 * This toggles the buzzer on each overflow.
 */
ISR(TCA0_OVF_vect) {
    overflow_counter++;  // Increment overflow counter

    // Toggle the buzzer on overflow (if active)
    PORTF.OUTTGL = (1 << BUZZER_PIN);

    // Clear the overflow interrupt flag
    TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;
}

/**
 * @brief Toggles LED with frequency ranging from 1 kHz to 5 kHz.
 */
void toggle_led_1KHZ_5KHZ(void) {
    PORTB.DIRSET = (1 << LED_PIN);  // Set LED pin as output

    while (1) {
        // Update the frequency every 30 overflows
        if (overflow_counter >= 30) {
            PORTB.OUTTGL = (1 << LED_PIN);  // Toggle LED

            // Set new blink frequency
            set_TCA_hz(frequency);

            // Adjust frequency within limits (1 kHz - 5 kHz)
            if (frequency >= MAX_FREQUENCY) {
                direction = -1;
            } else if (frequency <= MIN_FREQUENCY) {
                direction = 1;
            }

            frequency += direction * 10;  // Increment/decrement frequency
            overflow_counter = 0;  // Reset overflow counter
        }
    }
}

/**
 * @brief Main function that initializes peripherals and enters the main loop.
 */
int main(void) {
    // Initialize USART3 for communication
    usart_init(&USART3, &PORTB);

    // Set the clock to 16 MHz
    set_main_clk_16mhz();

    // Initialize the TCA timer to 250Hz (16Mhz/(64*250Hz)) = 1000
    init_TCA0(1000);

    // Set buzzer pin direction as output
    PORTF.DIRSET = (1 << BUZZER_PIN);  // Pin 2 on PORTF for buzzer

    // Enable global interrupts
    sei();

    // Main loop: Adjust frequency between 0 Hz and 29 kHz (the range i could hear)
    while (1) {
        // Update the frequency every 30 overflows for smoother transitions
        if (overflow_counter > 30) {
            // Set new frequency
            set_TCA_hz(frequency);

            // Change direction at frequency bounds
            if (frequency >= 5000) {
                direction = -1;
            } else if (frequency <= 1000) {
                direction = 1;
            }

            // Update the frequency with a step size
            frequency += direction * 10;

            overflow_counter = 0;  // Reset overflow counter
        }
    }
}
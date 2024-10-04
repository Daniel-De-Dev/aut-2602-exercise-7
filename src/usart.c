/*
 * File:   usart.c
 * Author: Daniel
 *
 * Created on September 12, 2024, 11:56 AM
 */

#include <stdio.h>
#include <avr/io.h>
#include "usart.h"

// Buffer to hold the received message
char message[MESSAGE_LENGTH];

#define DEFAULT_BAUD_RATE 9600

/**
 * @brief Initializes USART by configuring the appropriate ports.
 */
void usart_init(USART_t* USART, PORT_t* PORT) {
    USART->BAUD = (uint16_t)CALCULATED_BAUD_VALUE(DEFAULT_BAUD_RATE);
    USART->CTRLC = (1 << 1) | (1 << 0);                         // 8-bit data size, async mode
    PORT->DIRSET = (1 << 0);                                    // Set TX pin as output
    USART->CTRLB = (1 << USART_RXEN_bp) | (1 << USART_TXEN_bp);
    USART->CTRLA = (1 << USART_RXCIE_bp);
    stdout = &new_std_out;                                      // Redirect stdout to USART
}

/**
 * @brief Transmits a single character via USART.
 */
void usart_transmit_char(USART_t* USART, char c) {
    while (!(USART->STATUS & (1 << 5))) {}
    USART->TXDATAL = c;
}

/**
 * @brief Receives a single character via USART.
 * @return The received character, or 0 if no data is available.
 */
char usart_receive_char(USART_t* USART) {
    if (USART->STATUS & (1 << USART_RXCIF_bp)) {
        return USART->RXDATAL;
    }
    return 0;   // Return 0 if no data is received
}

/**
 * @brief Transmits a null-terminated string via USART.
 */
void usart_transmit_string(USART_t* USART, const char* str) {
    // Transmit each character in the string until the null terminator
    for (uint8_t i = 0; str[i] != '\0'; i++) {
        usart_transmit_char(USART, str[i]);
    }
}

/**
 * @brief Receives a string via USART until '\r', '\n', or message buffer is full.
 */
void usart_receive_string(USART_t* USART) {
    char c;
    int index = 0;

    while (index < MESSAGE_LENGTH - 1) {
        c = usart_receive_char(USART);
        if (c == '\r' || c == '\n') {
            break;
        }
        if (c) {
            message[index++] = c;
        }
    }
    message[index] = '\0';  // Null-terminate the string
}
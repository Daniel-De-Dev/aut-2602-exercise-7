/* 
 * File:   usart.h
 * Author: Daniel
 *
 * Created on September 12, 2024, 11:57 AM
 */

#ifndef USART_H
#define	USART_H

#include <avr/io.h>
#include <stdio.h>

/**
 * @brief Sample rate for Asynchronous Normal mode.
 */
#define SAMPLE_RATE 16

/**
 * @brief Macro to calculate the baud rate value for the USART.
 * 
 * This calculates the baud value based on the given baud rate and the CPU frequency.
 * From page 380 section 27.3.2.2.1 in AVR128DBX Datasheet
 * 
 * @param BAUD_RATE The desired baud rate (bits per second).
 * @return The calculated baud value for the USART.
 */
#define CALCULATED_BAUD_VALUE(BAUD_RATE) ((64UL * F_CPU) / (SAMPLE_RATE * (uint32_t)BAUD_RATE))

/**
 * @brief Initializes the provided USART port.
 * 
 * This function configures the USART with the necessary settings to enable communication.
 * 
 * @param USART Pointer to the USART port.
 * @param PORT Pointer to the corresponding PORT that has the corresponding TX pin.
 */
void usart_init(USART_t* USART, PORT_t* PORT);

/**
 * @brief Transmits a single character via USART.
 * 
 * This function sends one character through the USART transmitter.
 * 
 * @param USART Pointer to the USART port.
 * @param c The character to transmit.
 */
void usart_transmit_char(USART_t* USART, char c);

/**
 * @brief Receives a single character via USART.
 * 
 * This function checks and retrieves a single character received by the USART receiver.
 * Otherwise returns `0`
 * 
 * @param USART Pointer to the USART port.
 * @return The received character or `0` if no data had been received.
 */
char usart_receive_char(USART_t* USART);

/**
 * @brief Transmits a null-terminated string via USART.
 * 
 * This function sends an entire string, character by character, through the USART.
 * 
 * @param USART Pointer to the USART port.
 * @param str The null-terminated string to transmit.
 */
void usart_transmit_string(USART_t* USART, const char* str);

/**
 * @brief Receives a string and saves it into the message buffer.
 * 
 * This function waits for and receives a series of characters, which are then stored
 * in the global `message` buffer. It will continue reading until a `\n` or `\r` is received.
 * Or until it reaches the `MESSAGE_LENGTH`
 * 
 * @param USART Pointer to the USART port.
 */
void usart_receive_string(USART_t* USART);

/**
 * @brief A custom file stream object to redirect output to the USART.
 * 
 * This is useful for redirecting standard output (like printf) to the USART.
 */
extern FILE new_std_out;

/**
 * @brief Length of the message buffer for received strings.
 */
#define MESSAGE_LENGTH 20

/**
 * @brief Buffer to hold the received message.
 * 
 * The message buffer is shared across modules and is of size `MESSAGE_LENGTH`.
 */
extern char message[MESSAGE_LENGTH];

#endif	/* USART_H */
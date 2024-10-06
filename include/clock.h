/*
 *  File:  clock.h
 *  Author: Daniel
 * 
 *  Created on September 21, 2024, 4:39 PM
 */

#ifndef CLOCK_H
#define CLOCK_H

#include <avr/io.h>

/**
 * @brief Defines the clock frequency (F_CPU) in Hz.
 * 
 * This is the system clock frequency used for timing and delay calculations.
 * Ensure that this matches the clock settings specified in makefile and anywhere else.
 */
#define F_CPU 16000000UL     /**< System clock frequency is set to 16 MHz */

/**
 * @brief Retrieves the peripheral clock frequency.
 * 
 * This function returns the clock frequency that peripherals use in the system.
 * The value is which is derived from the main system clock (F_CPU).
 * 
 * @return The peripheral clock frequency in Hz.
 */
uint32_t get_clk_per(void);

/**
 * @brief Retrieves the clock frequency used by the specified ADC.
 * 
 * This function returns the clock frequency that the given ADC module uses.
 * 
 * @param ADC Pointer to the ADC port.
 * @return The ADC clock frequency in Hz.
 */
uint32_t get_clk_adc(ADC_t* ADC);

/**
 * @brief Sets the clock frequency for the specified ADC module.
 * 
 * This function attempts to configure the clock frequency for the given ADC to match the target frequency.
 * It rounds up to the nearest value to the specified target.
 * 
 * @param ADC Pointer to the ADC port.
 * @param target The desired clock frequency in Hz.
 * @return A status code (0 for success, non-zero for failure).
 */
uint8_t set_clk_adc(ADC_t* ADC, uint32_t target);

#endif  /* CLOCK_H */
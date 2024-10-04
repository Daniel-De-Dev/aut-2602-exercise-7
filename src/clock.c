/*
 *  File:  clock.c
 *  Author: Daniel
 * 
 *  Created on September 21, 2024, 4:39 PM
 */

#include <avr/io.h>
#include "clock.h"

/**
 * @brief Calculates the clock frequency based on a given prescaler value.
 * 
 * This helper function returns the clock frequency by dividing the peripheral
 * clock by the prescaler value.
 * 
 * @param clk_per Peripheral clock frequency.
 * @param prescaler_value The prescaler divisor.
 * @return The calculated clock frequency.
 */
uint32_t calculate_prescaler(uint32_t clk_per, uint32_t prescaler_value) {
    return (clk_per + prescaler_value - 1) / prescaler_value;  // Handles rounding
}
    
/**
 * @brief Retrieves the peripheral clock frequency (CLK_PER).
 * 
 * This function checks the system's clock configuration and determines whether a
 * prescaler is being applied. If a prescaler is enabled, the clock frequency is
 * divided by the appropriate prescaler value.
 * 
 * @return The peripheral clock frequency in Hz.
 */
uint32_t get_clk_per(void) {
    uint8_t mclkctrlb = CLKCTRL.MCLKCTRLB;  // Retrieve current clock control register value

    // Check if the prescaler is enabled
    if (mclkctrlb & (1 << CLKCTRL_PEN_bp)) {
        // Determine the clock division factor by masking and checking PDIV bits
        switch (mclkctrlb & CLKCTRL_PDIV_gm) {
            case CLKCTRL_PDIV_2X_gc:
                return calculate_prescaler(F_CPU, 2);
            case CLKCTRL_PDIV_4X_gc:
                return calculate_prescaler(F_CPU, 4);
            case CLKCTRL_PDIV_8X_gc:
                return calculate_prescaler(F_CPU, 8);
            case CLKCTRL_PDIV_16X_gc:
                return calculate_prescaler(F_CPU, 16);
            case CLKCTRL_PDIV_32X_gc:
                return calculate_prescaler(F_CPU, 32);
            case CLKCTRL_PDIV_64X_gc:
                return calculate_prescaler(F_CPU, 64);
            case CLKCTRL_PDIV_6X_gc:
                return calculate_prescaler(F_CPU, 6);
            case CLKCTRL_PDIV_10X_gc:
                return calculate_prescaler(F_CPU, 10);
            case CLKCTRL_PDIV_12X_gc:
                return calculate_prescaler(F_CPU, 12);
            case CLKCTRL_PDIV_24X_gc:
                return calculate_prescaler(F_CPU, 24);
            case CLKCTRL_PDIV_48X_gc:
                return calculate_prescaler(F_CPU, 48);
            default:
                return F_CPU;  // Default case if no prescaler matches
        }
    }

    // If the prescaler is disabled, CLK_PER equals F_CPU
    return F_CPU;
}

/**
 * @brief Returns the appropriate prescaler divisor for the specified ADC port.
 * 
 * This function reads the prescaler settings of the ADC and returns the divisor
 * value used for calculating the ADC clock frequency.
 * 
 * @param ADC Pointer to the ADC port.
 * @return The prescaler divisor, or 0 if an invalid prescaler is set.
 */
uint32_t get_prescaler_value(ADC_t* ADC) {
    uint8_t prescaler_bits = ADC->CTRLC & ADC_PRESC_gm;
    switch (prescaler_bits) {
        case ADC_PRESC_DIV2_gc: return 2;
        case ADC_PRESC_DIV4_gc: return 4;
        case ADC_PRESC_DIV8_gc: return 8;
        case ADC_PRESC_DIV12_gc: return 12;
        case ADC_PRESC_DIV16_gc: return 16;
        case ADC_PRESC_DIV20_gc: return 20;
        case ADC_PRESC_DIV24_gc: return 24;
        case ADC_PRESC_DIV28_gc: return 28;
        case ADC_PRESC_DIV32_gc: return 32;
        case ADC_PRESC_DIV48_gc: return 48;
        case ADC_PRESC_DIV64_gc: return 64;
        case ADC_PRESC_DIV96_gc: return 96;
        case ADC_PRESC_DIV128_gc: return 128;
        case ADC_PRESC_DIV256_gc: return 256;
        default: return 0;  // Invalid prescaler setting
    }
}

/**
 * @brief Retrieves the clock frequency for the specified ADC port.
 * 
 * This function reads the ADC's prescaler settings and calculates the resulting
 * ADC clock frequency.
 * 
 * @param ADC Pointer to the ADC port.
 * @return The ADC clock frequency in Hz, or 0 if an invalid prescaler is found.
 */
uint32_t get_clk_adc(ADC_t* ADC) {
    uint32_t clk_per = get_clk_per();
    uint32_t prescaler_value = get_prescaler_value(ADC);
    
    if (prescaler_value > 0) {
        return calculate_prescaler(clk_per, prescaler_value);
    } else {
        return 0;  // Error: Invalid prescaler
    }
}

/**
 * @brief Sets the clock frequency for the specified ADC port.
 * 
 * This function attempts to configure the clock frequency of the ADC by adjusting
 * the prescaler setting to match the target frequency.
 * 
 * @param ADC Pointer to the ADC port.
 * @param target The desired clock frequency in Hz.
 * @return 0 if successful, 1 if no suitable prescaler is found.
 */
uint8_t set_clk_adc(ADC_t* ADC, uint32_t target) {
    // Get the peripheral clock (CLK_PER) frequency
    uint32_t clk_per = get_clk_per();

    // Calculate the ratio of CLK_PER to the desired target CLK_ADC
    uint32_t ratio = (clk_per + target - 1) / target;  // Rounds up

    // Find the closest prescaler setting based on the ratio
    if (ratio <= 2) {
        ADC->CTRLC = ADC_PRESC_DIV2_gc;
        return 0;
    } else if (ratio <= 4) {
        ADC->CTRLC = ADC_PRESC_DIV4_gc;
        return 0;
    } else if (ratio <= 8) {
        ADC->CTRLC = ADC_PRESC_DIV8_gc;
        return 0;
    } else if (ratio <= 12) {
        ADC->CTRLC = ADC_PRESC_DIV12_gc;
        return 0;
    } else if (ratio <= 16) {
        ADC->CTRLC = ADC_PRESC_DIV16_gc;
        return 0;
    } else if (ratio <= 20) {
        ADC->CTRLC = ADC_PRESC_DIV20_gc;
        return 0;
    } else if (ratio <= 24) {
        ADC->CTRLC = ADC_PRESC_DIV24_gc;
        return 0;
    } else if (ratio <= 28) {
        ADC->CTRLC = ADC_PRESC_DIV28_gc;
        return 0;
    } else if (ratio <= 32) {
        ADC->CTRLC = ADC_PRESC_DIV32_gc;
        return 0;
    } else if (ratio <= 48) {
        ADC->CTRLC = ADC_PRESC_DIV48_gc;
        return 0;
    } else if (ratio <= 64) {
        ADC->CTRLC = ADC_PRESC_DIV64_gc;
        return 0;
    } else if (ratio <= 96) {
        ADC->CTRLC = ADC_PRESC_DIV96_gc;
        return 0;
    } else if (ratio <= 128) {
        ADC->CTRLC = ADC_PRESC_DIV128_gc;
        return 0;
    } else if (ratio <= 256) {
        ADC->CTRLC = ADC_PRESC_DIV256_gc;
        return 0;
    }

    // If no suitable prescaler was found, return an error
    return 1;
}
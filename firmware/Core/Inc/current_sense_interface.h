/**
  ******************************************************************************
  * @file           : current_sense_interface.h
  * @brief          : Header for current_sense_interface.cpp file.
  *                   current_sense_interface files contain the requred functions to setup and use the DFSDM peripheral to read phase currents
  ******************************************************************************
**/

#pragma once

#include "stm32f413xx.h"
#include <assert.h>
#include <math.h>
#include "device_descriptor.h"
#include "logging.h"

//#define DFSDM_FOSR (1000*ADCCLK/PWMCLK/2 - 2) // TODO: figure out why this is producing sample times too long
#define DFSDM_FOSR 130 // just under 1 update period at 25khz PWM frequency
#define SHUNT_RESISTANCE 0.005	// (Ohms) Current shunt resistance

// Class for managing DFSDM hardware
class current_sense_interface{
    private:

        logging* logs;

        uint32_t incomplete_conversion_count = 0;

        const int32_t conversion_divisor_milliamps = int32_t(-((float)(pow(DFSDM_FOSR+1, 3) * (1.0 - ADC_MIN_VALUE_PERCENT*2.0) * SHUNT_RESISTANCE) / (ADC_VOLTAGE * 1000)));

    public:

        int32_t phase_U_milliamps = 0;
        int32_t phase_V_milliamps = 0;
        int32_t phase_W_milliamps = 0;

        current_sense_interface(logging* logs);
        
        void init(void);

        uint32_t get_currents();

        void start_sample();

        void enable_short_circuit_detection();

        void disable_short_circuit_detection();

        uint32_t short_circuit_detected();

        void clear_short_circuit_detected();
};
/**
  ******************************************************************************
  * @file           : phase_pwm.h
  * @brief          : Header for phase_pwm.cpp file.
  *                   phase_pwm files contain the requred functions to setup and use the TIM1 peripheral for outputting phase PWM
  ******************************************************************************
**/

#pragma once

#include "stm32f413xx.h"
#include <assert.h>
#include "device_descriptor.h"
#include "logging.h"

#define PWM_ticks 1000*SYSCLK/PWMCLK/2

// Class for managing phase outpuit PWM hardware
class phase_pwm{
    private:

      logging* logs;

      bool release = false;
      const float max_duty_cycle = 0.95;
      const uint16_t max_pwm_ticks = PWM_ticks * max_duty_cycle;
      const uint16_t min_pwm_ticks = PWM_ticks * (1-max_duty_cycle);


    public:
        phase_pwm(logging* logs);

        const float max_percent = max_duty_cycle;
        const float min_percent = 1.0-max_duty_cycle;

        void release_mode(void);  // allows actual PWM output
        
        void init(void);

        void enable(void);

        void disable(void);

        bool break_flag_triggered(void);

        void clear_break_flag(void);

        void set_raw(uint32_t U, uint32_t V, uint32_t W);

        void set_voltage(float U, float V, float W, float dc_bus_voltage);

        void set_percentange(float U, float V, float W);

        bool is_enabled(void);
};
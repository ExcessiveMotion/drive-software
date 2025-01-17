/**
  ******************************************************************************
  * @file           : fans.h
  * @brief          : Header for fans.cpp file.
  *                   contains the required functions to setup and use San Ace 40 9GA0424P3H001 Fans 
  ******************************************************************************
**/

#pragma once

#include "stm32f413xx.h"
#include <assert.h>
#include "device_descriptor.h"
#include "logging.h"

#define PWM_ticks 1000*SYSCLK/PWMCLK/2

class fans{
    private:
      logging* logs;

      const uint32_t tach_sample_period_ms = 500;
      const uint32_t tach_sample_count = SYSTICK_FREQUENCY * tach_sample_period_ms / 1000;
      uint32_t update_cycle_count = 0;

      uint32_t set_speed_rpm = 0;
      uint16_t tachometer_1_rpm = 0;
      uint16_t tachometer_2_rpm = 0;

      void configure_GPIOB6_for_PWM(void);
      void configure_TIM4_for_PWM(void);

      void configure_GPIOC6_for_tachometer(void);
      void configure_TIM3_for_tachometer(void);

      void configure_GPIOC7_for_tachometer(void);
      void configure_TIM8_for_tachometer(void);

    public:

        const uint32_t max_rpm = MAX_FAN_SPEED_RPM;
        
        fans(logging* logs);

        void init();

        uint32_t set_speed(uint32_t speed_rpm);

        uint32_t get_fan_1_speed_rpm(void);

        uint32_t get_fan_2_speed_rpm(void);

        void SysTick_Handler();
};




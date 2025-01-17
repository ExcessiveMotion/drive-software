#pragma once

//^#define pi 3.141592653589793
#include "device.h"


// float Kp = .01;		// Proportional gain for current controller (V/A)
// float Ki = .02;		// Integral gain for current controller (V/A/s)
// float I_term_limit = 12;

// #define LOW_TEST_CURRENT 0.0
// #define HIGH_TEST_CURRENT 3.0

// float requested_Iq = LOW_TEST_CURRENT;
// float requested_Id = 0.0;
// float electrical_rads_per_second = 10.0;

// float theta = 0.0;

// // DO NOT MODIFY THESE OUTSIDE OF ISR FUNCTION
// uint32_t CAPTURE_COMP_U = PWM_ticks * 0.5;
// uint32_t CAPTURE_COMP_V = PWM_ticks * 0.5;
// uint32_t CAPTURE_COMP_W = PWM_ticks * 0.5;
// float I_Term = 0;	// Integral term for current controller (V)

// uint64_t sysTick_counter = 0;


// void delay_us(uint32_t time_us);

// void CPU_init(void);

// extern "C" {
// void USART6_Error_Interrupt_Handler(void);
// void Parity_Error_Callback(void);
// void Framing_Error_Callback(void);
// void Noise_Detected_Error_Callback(void);
// void Overrun_Error_Callback(void);
// }

// void TIM2_init(void);

// int Clarke_and_Park_Transform(float theta, float A, float B, float C, float *D, float *Q);
// int Inverse_Carke_and_Park_Transform(float theta, float D, float Q, float *A, float *B, float *C);



// uint32_t adc_power_on_delay_cycles = 0;	// PWM cycle counter to wait after enabling pwm to allow ADCs to power on

// user_io userIO(1000/SYSTICK_FREQUENCY);
// fans Fans;
// adc_interface adc(0);
// current_sense_interface currentSense(0);
// phase_pwm phasePWM(0);
// sto STO(0);
// communication comm(0);

// volatile bool phase_pwm_updated_flag = false;

// enum States {
// 	STARTUP,
// 	FAULT,
// 	IDLE,
// 	RUN
// };

// enum Enter_idle_steps {
// 	CHECK_OK_TO_START,
// 	ENABLE_STO,
// 	IDLE_
// };

// enum Enter_run_steps {
// 	ENABLE_PWM,
// 	ENABLE_SHORT_CIRCUIT_DETECTION,
// 	RUN_
// };

// enum Enter_fault_steps {
// 	DISABLE_PWM,
// 	DISABLE_STO,
// 	FAULT_
// };

// int Clarke_and_Park_Transform(float theta, float A, float B, float C, float *D, float *Q);
// int Inverse_Carke_and_Park_Transform(float theta, float D, float Q, float *A, float *B, float *C);
// float Current_Controller(float EI);
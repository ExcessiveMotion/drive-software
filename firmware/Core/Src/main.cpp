#include "main.h"

device* Device = nullptr;
#include "interrupt_catch.h"

int main(void){

	device dev = device();
	Device = &dev;

	Device->init();
	
	Device->run();

	return 1;	// should never reach this
}

// int main(void){

// 	userIO.init();
// 	//adc.init();
// 	//currentSense.init();
// 	//phasePWM.init();
// 	//STO.init();
// 	//comm.init();

// 	// test LEDs
// 	userIO.set_led_state(0b1111, userIO.on);
// 	delay_us(1000*1000);
// 	userIO.set_led_state(0b1110, userIO.off);


// 	current_state = FAULT;

// 	requested_state = IDLE;

// 	uint64_t last_idle_attempt_time = sysTick_counter;

// 	uint64_t last_run_attempt_time = sysTick_counter;

// 	uint64_t last_current_test_time = sysTick_counter;

// 	Fans.set_speed(6200);


// 	volatile uint32_t speed1;
// 	volatile uint32_t speed2;

// 	while(1){
// 		comm.debug();

// 		for(int i = 0; i < 12400000; i++){
// 			Fans.set_speed(i/1000);	
// 		}
// 	}

	// while(0){

	// 	if(current_state == FAULT && sysTick_counter>last_idle_attempt_time+(SYSTICK_FREQUENCY*1)){
	// 		requested_state = IDLE;
	// 		last_idle_attempt_time = sysTick_counter;
	// 		last_run_attempt_time = sysTick_counter;
	// 	}

	// 	if(current_state == IDLE && sysTick_counter>last_run_attempt_time+(SYSTICK_FREQUENCY*1)){
	// 		requested_state = RUN;
	// 		last_run_attempt_time = sysTick_counter;
	// 	}

		


	// 	//volatile uint32_t temp = comm.get_commutaion_angle();
	// 	//theta = ((float)temp)/0xFFFF * (2*pi);
	// 	//theta += pi/2;	// offset to match test fanuc encoder phase angle

	// 	// if(temp > 0x8000){
	// 	// 	userIO.set_led_state(0b1000, userIO.on);
	// 	// }
	// 	// else{
	// 	// 	userIO.set_led_state(0b1000, userIO.off);
	// 	// }

	// 	switch(current_state){
	// 		case STARTUP:
	// 			// we should never actually reach this since startup should already be done
	// 			break;

	// 		case FAULT:
	// 			// system experienced a critical fault and needed to shutdown
	// 			if(requested_state == IDLE){
	// 				switch(enter_idle_step){
	// 					case CHECK_OK_TO_START:
	// 						//	TODO: clear errors
							
	// 						//	TODO: check everything to make sure we can exit fault state safely

	// 						// verify STO is ok
							
	// 						if(STO.check_fault()){
	// 							enter_idle_step = CHECK_OK_TO_START;
	// 							requested_state = FAULT;
	// 							// TODO: specify fault
	// 							break;
	// 						}

	// 						// attempt to enable drive STO on rising edge of update flag
	// 						if(phase_pwm_updated_flag){
	// 							STO.enable();
	// 							enter_idle_step = ENABLE_STO;
	// 							phase_pwm_updated_flag = 0;
	// 						}
	// 						break;

	// 					case ENABLE_STO:

	// 						// check to see if drive was successfully enabled on next update cycle
	// 						if(phase_pwm_updated_flag){
	// 							phasePWM.clear_break_flag();
	// 							if(STO.output_allowed() && !phasePWM.break_flag_triggered()){
	// 								enter_idle_step = IDLE_;
	// 								phase_pwm_updated_flag = 0;
	// 							}
	// 							else{
	// 								enter_idle_step = CHECK_OK_TO_START;
	// 								requested_state = FAULT;
	// 								// TODO: specify fault
	// 								break;
	// 							}
	// 						}
	// 						break;

	// 					case IDLE_:
	// 						userIO.set_led_state(0b0001, userIO.blink_slow);
	// 						current_state = IDLE;
	// 						enter_idle_step = CHECK_OK_TO_START;
	// 						break;

	// 				}
	// 			}
	// 			break;

	// 		case IDLE:
	// 			// system is ok and ready to start

	// 			if(!STO.output_allowed()){
	// 				requested_state = FAULT;
	// 			}

	// 			if(requested_state == RUN){
	// 				switch(enter_run_step){
	// 					case ENABLE_PWM:	// switch on PWM outputs
	// 						phasePWM.enable();
	// 						enter_run_step = ENABLE_SHORT_CIRCUIT_DETECTION;
	// 						adc_power_on_delay_cycles = ADC_ENABLE_DELAY_CYCLES;
	// 					break;

	// 					case ENABLE_SHORT_CIRCUIT_DETECTION:	// clear any detected short flags and enable short detection
	// 						if(phase_pwm_updated_flag){
	// 							adc_power_on_delay_cycles--;
	// 							phase_pwm_updated_flag = 0;

	// 							if(adc_power_on_delay_cycles == 0){
	// 								currentSense.clear_short_circuit_detected();
	// 								currentSense.enable_short_circuit_detection();
	// 								currentSense.start_sample();
	// 								adc.start_sample();
	// 								enter_run_step = RUN_;
	// 						}
	// 						}
							
	// 					break;

	// 					case RUN_:
	// 						userIO.set_led_state(0b0001, userIO.blink_fast);
	// 						enter_run_step = ENABLE_PWM;
	// 						current_state = RUN;
	// 					break;
					
	// 				};
	// 			}

	// 			if(requested_state == FAULT){
	// 				switch(enter_fault_step){
	// 					case DISABLE_PWM:
	// 						phasePWM.disable();
	// 						currentSense.disable_short_circuit_detection();
	// 						enter_fault_step = DISABLE_STO;
	// 					break;

	// 					case DISABLE_STO:
	// 						STO.disable();
	// 						enter_fault_step = FAULT_;
	// 					break;

	// 					case FAULT_:
	// 						userIO.set_led_state(0b0001, userIO.on);
	// 						enter_fault_step = DISABLE_PWM;
	// 						current_state = FAULT;
	// 				};
	// 			}
				
	// 			break;

	// 		case RUN:
	// 			// PWM enabled and control loops running
	// 			if(!STO.output_allowed()){
	// 				requested_state = FAULT;
	// 			}

	// 			// if(sysTick_counter > last_current_test_time + SYSTICK_FREQUENCY*1){
	// 			// 	last_current_test_time = sysTick_counter;

	// 			// 	if(requested_Iq == HIGH_TEST_CURRENT){
	// 			// 		requested_Iq = LOW_TEST_CURRENT;
	// 			// 	}
	// 			// 	else{
	// 			// 		requested_Iq = HIGH_TEST_CURRENT;
	// 			// 	}
	// 			// }

	// 			//requested_Iq = -((float)comm.get_current_command_milliamps()) / 500.0;

	// 			if(requested_Iq > 8.0)requested_Iq = 8.0;
	// 			if(requested_Iq < -8.0)requested_Iq = -8.0;

	// 			///////////////////// Current Control Section /////////////////////

	// 			if(phase_pwm_updated_flag){		// this flag signifies the main PWM counter update has occured
					
	// 				currentSense.get_currents();
	// 				currentSense.start_sample();

					
	// 				float measured_Id;
	// 				float measured_Iq;

	// 				Clarke_and_Park_Transform(theta, (float)currentSense.phase_U_milliamps/1000.0, (float)currentSense.phase_V_milliamps/1000.0, (float)currentSense.phase_W_milliamps/1000.0, &measured_Id, &measured_Iq);

	// 				float error_Iq = requested_Iq - measured_Iq;
	// 				float error_Id = requested_Id - measured_Id;

	// 				float requested_Vq = Current_Controller(error_Iq);
	// 				float requested_Vd = Current_Controller(error_Id);

	// 				float requested_duty_cycle_Q = requested_Vq / (0.5*((float)adc.get_dc_bus_millivolts())/1000.0);
	// 				float requested_duty_cycle_D = requested_Vd / (0.5*((float)adc.get_dc_bus_millivolts())/1000.0);

	// 				float requested_duty_cycle_U;
	// 				float requested_duty_cycle_V;
	// 				float requested_duty_cycle_W;

	// 				Inverse_Carke_and_Park_Transform(theta, requested_duty_cycle_D, requested_duty_cycle_Q, &requested_duty_cycle_U, &requested_duty_cycle_V, &requested_duty_cycle_W);

	// 				if (requested_duty_cycle_U > (float)0.95){
	// 					requested_duty_cycle_U = 0.95;
	// 				}
	// 				if (requested_duty_cycle_U < (float)-0.95){
	// 					requested_duty_cycle_U = -0.95;
	// 				}

	// 				if (requested_duty_cycle_V > (float)0.95){
	// 					requested_duty_cycle_V = 0.95;
	// 				}
	// 				if (requested_duty_cycle_V < (float)-0.95){
	// 					requested_duty_cycle_V = -0.95;
	// 				}

	// 				if (requested_duty_cycle_W > (float)0.95){
	// 					requested_duty_cycle_W = 0.95;
	// 				}
	// 				if (requested_duty_cycle_W < (float)-0.95){
	// 					requested_duty_cycle_W = -0.95;
	// 				}

	// 				requested_duty_cycle_U = (requested_duty_cycle_U + 1)/2;
	// 				requested_duty_cycle_V = (requested_duty_cycle_V + 1)/2;
	// 				requested_duty_cycle_W = (requested_duty_cycle_W + 1)/2;

	// 				CAPTURE_COMP_U = requested_duty_cycle_U * 500*SYSCLK/PWMCLK;
	// 				CAPTURE_COMP_V = requested_duty_cycle_V * 500*SYSCLK/PWMCLK;
	// 				CAPTURE_COMP_W = requested_duty_cycle_W * 500*SYSCLK/PWMCLK;

	// 				adc.start_sample();

	// 				phase_pwm_updated_flag = 0;
	// 			}

	// 			//////////////////////////////////////////////////////////////////

	// 			if(requested_state == FAULT){
	// 				switch(enter_fault_step){
	// 					case DISABLE_PWM:
	// 						phasePWM.disable();
	// 						currentSense.disable_short_circuit_detection();
	// 						enter_fault_step = DISABLE_STO;
	// 					break;

	// 					case DISABLE_STO:
	// 						STO.disable();
	// 						enter_fault_step = FAULT_;
	// 					break;

	// 					case FAULT_:
	// 						userIO.set_led_state(0b0001, userIO.on);
	// 						enter_fault_step = DISABLE_PWM;
	// 						current_state = FAULT;
	// 				};
	// 			}
	// 			break;
	// 	}

	// 	// currentSense.start_sample();
	// 	// delay_us(1000);
	// 	// currentSense.get_currents();
	// 	//adc.start_sample();
	// 	//userIO.set_led_state(userIO.get_switch_states(), userIO.blink_fast);
	// 	//userIO.set_led_state(~userIO.get_switch_states(), userIO.off);

	// }
//}

// extern "C" {

// void SysTick_Handler(void){
	
// 	userIO.update();
// 	sysTick_counter++;
// }

// void I2C1_EV_IRQHandler(void){
// 	userIO.I2C1_EV_IRQHandler();
// }

// void DMA2_Stream0_IRQHandler(void){
// 	adc.dma_interrupt_handler();
// }

// void DMA2_Stream1_IRQHandler(void){
// 	comm.dma_stream1_interrupt_handler();
// }

// void TIM1_UP_TIM10_IRQHandler(void) {
// 	if (TIM1->SR & TIM_SR_UIF) { // Check if update interrupt flag is set
//  		TIM1->SR &= ~TIM_SR_UIF; // Clear update interrupt flag
// 		TIM1->CCR1 = CAPTURE_COMP_U;
// 		TIM1->CCR2 = CAPTURE_COMP_V;
// 		TIM1->CCR3 = CAPTURE_COMP_W;
// 		phase_pwm_updated_flag = true;	
// 	}
// }

// void USART6_IRQHandler(void){
// 	comm.usart6_interrupt_handler();
// }   
// }

// extern "C" {

// void USART6_Error_Interrupt_Handler(void){

// 	if (USART6->SR & USART_SR_PE) {
//         Parity_Error_Callback();
// 		// Cleared by read or write to USART_DR_DR
//     }
// 	if (USART6->SR & USART_SR_FE) {
//         Framing_Error_Callback();
// 		// Cleared by reading USART_DR_DR 
//     }
// 	if (USART6->SR & USART_SR_NE) {
//         Noise_Detected_Error_Callback();
// 		// Cleared by reading USART_DR_DR 
//     }
// 	if (USART6->SR & USART_SR_ORE) {
//         Overrun_Error_Callback();
// 		// Cleared by reading USART_DR_DR 
//     }
// }
// }


// void TIM2_init(){

// 	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;		// Enable TIM2   Clock

// 	TIM2->PSC = 0; // Prescaler
// 	TIM2->ARR = 0xFFFFFFFF;  // Max auto-reload value
// 	TIM2->CR1 |= TIM_CR1_CEN; // Enable TIM2
// }



// int Clarke_and_Park_Transform(float theta, float A, float B, float C, float *D, float *Q){
//     // Amplitude invarient

//     // assert((0 <= theta) && (theta <= 2*pi));    // Assert theta
//     // assert(fabs(A + B + C) < 0.1);    // Assert ABC currents agree
    
//     float X = (2 * A - B - C) * (1 / 3);
//     float Y = (B - C) * (sqrt(3) / 3);
    
//     float co = cos(theta);
//     float si = sin(theta);
    
//     *D = co*X + si*Y;
//     *Q = co*Y - si*X;
    
//     return 0;
// }


// int Inverse_Carke_and_Park_Transform(float theta, float D, float Q, float *A, float *B, float *C){
//     // Amplitude invarient
	
//     // Inputs
//     // theta = electrical angle [0, 2*pi] radians
//     // D     = direct current [] amps
//     // Q     = quadrature current [] amps
    
//     // Outputs
//     // A, B, C
    
//     // assert((0 <= theta) && (theta <= 2*pi));    // Assert theta
    
//     float co = cos(theta);
//     float si = sin(theta);
    
//     float X = co*D - si*Q;
//     float Y = si*D + co*Q;
    
//     *A = X;
//     *B = -(1.0 / 2.0) * X;
//     *C = *B - (sqrt(3.0) / 2.0) * Y;
//     *B += (sqrt(3.0) / 2.0) * Y;
    
//     // assert(fabs(*A + *B + *C) < 0.001);    // Assert ABC currents agree
    
//     return 0;
// }


// float Current_Controller(float EI){
// 	// Input:  EI = Error Current (A)
// 	// Output: RV = Requested Voltage (V)

// 	float P_Term = Kp * EI;
// 	I_Term += Ki *EI;

// 	if (I_Term > I_term_limit){
// 		I_Term = I_term_limit;
// 	}
// 	else if (I_Term < -I_term_limit){
// 		I_Term = -I_term_limit;
// 	}

// 	float RV = P_Term + I_Term;

// 	return RV;
// }


extern "C" {
// void TIM1_UP_TIM10_IRQHandler(void) {
//     if (TIM1->SR & TIM_SR_UIF) { // Check if update interrupt flag is set
// 		TIM1->SR &= ~TIM_SR_UIF; // Clear update interrupt flag
		
// 		if(!(DFSDM2_Filter1->FLTISR & DFSDM_FLTISR_REOCF)){		// check if DFSDM conversion is complete
// 			//TODO: handle missing conversion, this should ony happen once at startup since there is not previous data
// 			return;
// 		}
// 		if(!(DFSDM2_Filter2->FLTISR & DFSDM_FLTISR_REOCF)){		// check if DFSDM conversion is complete
// 			//TODO: handle missing conversion, this should ony happen once at startup since there is not previous data
// 			return;
// 		}
// 		if(!(DFSDM2_Filter3->FLTISR & DFSDM_FLTISR_REOCF)){		// check if DFSDM conversion is complete
// 			//TODO: handle missing conversion, this should ony happen once at startup since there is not previous data
// 			return;
// 		}

// 		// set pwm exactly on edge based on data from last cycle
// 		TIM1->CCR1 = CAPTURE_COMP_U;
// 		TIM1->CCR2 = CAPTURE_COMP_V;
// 		TIM1->CCR3 = CAPTURE_COMP_W;

// 		theta += electrical_rads_per_second * (2*pi) * (.5/(PWMCLK*1000));

// 		if (theta >= (2*pi)){
// 			theta = 0.0;
// 		}


// 		// get conversion data from DFSDM
// 		// TODO: consider shifting a more optimal number of bit to get higher resolution
// 		int32_t MI_U_Raw = ((int32_t)(DFSDM2_Filter1->FLTRDATAR & 0xFFFFFF00));
// 		int32_t MI_V_Raw = ((int32_t)(DFSDM2_Filter2->FLTRDATAR & 0xFFFFFF00));
// 		int32_t MI_W_Raw = ((int32_t)(DFSDM2_Filter3->FLTRDATAR & 0xFFFFFF00));

// 		// start all conversions for next cycle
//         // TODO: Ensure DFSDM conversions occur on PWM timer direction flips
// 		DFSDM2_Filter1->FLTCR1 |= DFSDM_FLTCR1_RSWSTART;	// Start Filter 1 Conversion
// 		DFSDM2_Filter2->FLTCR1 |= DFSDM_FLTCR1_RSWSTART;	// Start Filter 2 Conversion
// 		DFSDM2_Filter3->FLTCR1 |= DFSDM_FLTCR1_RSWSTART;	// Start Filter 3 Conversion

// 		float measured_current_U = -MI_U_Raw;
// 		measured_current_U *= ADC_VOLTAGE;
// 		measured_current_U /= (float)(pow(DFSDM_FOSR+1, 3) * (1.0 - ADC_MIN_VALUE_PERC*2.0) * SHUNT_RESISTANCE);

// 		float measured_current_V = -MI_V_Raw;
// 		measured_current_V *= ADC_VOLTAGE;
// 		measured_current_V /= (float)(pow(DFSDM_FOSR+1, 3) * (1.0 - ADC_MIN_VALUE_PERC*2.0) * SHUNT_RESISTANCE);

// 		float measured_current_W = -MI_W_Raw;
// 		measured_current_W *= ADC_VOLTAGE;
// 		measured_current_W /= (float)(pow(DFSDM_FOSR+1, 3) * (1.0 - ADC_MIN_VALUE_PERC*2.0) * SHUNT_RESISTANCE);

// 		float measured_Id;
// 		float measured_Iq;

// 		Clarke_and_Park_Transform(theta, measured_current_U, measured_current_V, measured_current_W, &measured_Id, &measured_Iq);

// 		float error_Iq = requested_Iq - measured_Iq;
// 		float error_Id = requested_Id - measured_Id;

// 		float requested_Vq = Current_Controller(error_Iq);
// 		float requested_Vd = Current_Controller(error_Id);

// 		float requested_duty_cycle_Q = requested_Vq / (0.5*VBUS);
// 		float requested_duty_cycle_D = requested_Vd / (0.5*VBUS);

// 		float requested_duty_cycle_U;
// 		float requested_duty_cycle_V;
// 		float requested_duty_cycle_W;

// 		Inverse_Carke_and_Park_Transform(theta, requested_duty_cycle_D, requested_duty_cycle_Q, &requested_duty_cycle_U, &requested_duty_cycle_V, &requested_duty_cycle_W);

// 		if (requested_duty_cycle_U > (float)0.95){
// 			requested_duty_cycle_U = 0.95;
// 		}
// 		if (requested_duty_cycle_U < (float)-0.95){
// 			requested_duty_cycle_U = -0.95;
// 		}

// 		if (requested_duty_cycle_V > (float)0.95){
// 			requested_duty_cycle_V = 0.95;
// 		}
// 		if (requested_duty_cycle_V < (float)-0.95){
// 			requested_duty_cycle_V = -0.95;
// 		}

// 		if (requested_duty_cycle_W > (float)0.95){
// 			requested_duty_cycle_W = 0.95;
// 		}
// 		if (requested_duty_cycle_W < (float)-0.95){
// 			requested_duty_cycle_W = -0.95;
// 		}

// 		requested_duty_cycle_U = (requested_duty_cycle_U + 1)/2;
// 		requested_duty_cycle_V = (requested_duty_cycle_V + 1)/2;
// 		requested_duty_cycle_W = (requested_duty_cycle_W + 1)/2;

// 		CAPTURE_COMP_U = requested_duty_cycle_U * 500*SYSCLK/PWMCLK;
// 		CAPTURE_COMP_V = requested_duty_cycle_V * 500*SYSCLK/PWMCLK;
// 		CAPTURE_COMP_W = requested_duty_cycle_W * 500*SYSCLK/PWMCLK;
//     }
// }


void Parity_Error_Callback(void){
	while(1);
}

void Framing_Error_Callback(void){
	while(1);
}

void Noise_Detected_Error_Callback(void){
	while(1);
}

void Overrun_Error_Callback(void){
	while(1);
}


}
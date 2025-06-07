#include "sto.h"

sto::sto(logging* log){
	this->log = log;
}

/*!
    \brief Initialize hardware
    
    \note Run this after clocks are configured but before the main loop is started
*/
void sto::init(){
    
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;    // Enable GPIOC  Clock

	GPIOC->MODER = (GPIOC->MODER & ~GPIO_MODER_MODER10) | GPIO_MODER_MODER10_0;		// set STO_EN (PC10) to output
	GPIOC->MODER = (GPIOC->MODER & ~GPIO_MODER_MODER11);		// set STO_CH1_FBK (PC11) to input
	GPIOC->MODER = (GPIOC->MODER & ~GPIO_MODER_MODER12);		// set STO_CH2_FBK (PC12) to input

}


/*!
    \brief Enable drive
*/
bool sto::enable(){
	auto fault = check_fault();
	if(fault){
		return false;		// STO fault detected, do not enable
	}

	GPIOC->BSRR |= GPIO_BSRR_BS10;	// turn on STO_EN output (note this also needs both external STO channels to be on to allow the drive to output)
	return true;
}


/*!
    \brief Disable drive
*/
void sto::disable(){
	GPIOC->BSRR |= GPIO_BSRR_BR10;	// turn off STO_EN output (note this also needs both external STO channels to be on to allow the drive to output)

}


/*!
    \brief Check if the STO feedback makes sense (this NOT checking if the drive is enabled, just that the hardware seems good)

	\returns 0 when OK
*/
bool sto::check_fault(){
	bool fault = false;
	// check both feedback signals are the same
	if((GPIOC->IDR & GPIO_IDR_ID11)>>GPIO_IDR_ID11_Pos != (GPIOC->IDR & GPIO_IDR_ID12)>>GPIO_IDR_ID12_Pos){
		log->log_persistent_active((uint32_t)sto_messages::sto_fault_matching_channels);		// return signifying a fault, feedback signals do not match
		fault = true;
	}
	else{
		log->log_persistent_inactive((uint32_t)sto_messages::sto_fault_matching_channels);		// clear the fault
	}

	if(!(GPIOC->ODR & GPIO_ODR_OD10)){		// MCU STO output enable is off
		if(!(GPIOC->IDR & GPIO_IDR_ID11) || !(GPIOC->IDR & GPIO_IDR_ID12)){
			log->log_persistent_active((uint32_t)sto_messages::sto_hardware_fault);		// return signifying a fault, feedback is not allowed to be on(inverted) if the enable is off
			fault = true;
		}
		else{
			log->log_persistent_inactive((uint32_t)sto_messages::sto_hardware_fault);		// clear the fault
		}
	}

	return fault;
}


/*!
    \brief Check if the drive is allowed to enable PWM output
*/
bool sto::output_allowed(){

	bool fault = check_fault();

	if(fault){
		return false;
	}

	if(GPIOC->IDR & GPIO_IDR_ID11 || GPIOC->IDR & GPIO_IDR_ID12){
		return false;	// one or both feedback signals are not active (inverted)
	}

	return true;
}
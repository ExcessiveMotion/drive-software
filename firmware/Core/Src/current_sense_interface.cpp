#include "current_sense_interface.h"


current_sense_interface::current_sense_interface(logging* logs){
    this->logs = logs;
}


/*!
    \brief Initialize hardware
    
    \note Run this after clocks are configured but before the main loop is started
*/
void current_sense_interface::init(){

	RCC->APB2ENR |= RCC_APB2ENR_DFSDM2EN;   // Enable DFSDM2 Clock
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;    // Enable GPIOB  Clock
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;    // Enable GPIOC  Clock

	// GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED10_1;

	GPIOB->MODER |= (2 << (2 * 9));				// Set U_Data (PB9) to AF mode
	GPIOC->MODER |= (2 << (2 * 5));				// Set V_Data (PC5) to AF mode
	GPIOC->MODER |= (2 << (2 * 9));				// Set W_Data (PC9) to AF mode
	GPIOB->MODER |= (2 << (2 * 10));			// Set ADC_CLK (PB10) to AF mode
	GPIOB->AFR[1] |= (6  << (4 * (9  - 8)));	// Set PB9  AF to DFSDM2_ DATIN1
	GPIOC->AFR[0] |= (3  << (4 * (5  - 0)));	// Set PC5  AF to DFSDM2_ DATIN2
	GPIOC->AFR[1] |= (7  << (4 * (9  - 8)));	// Set PC9  AF to DFSDM2_ DATIN3
	GPIOB->AFR[1] |= (10 << (4 * (10 - 8)));	// Set PB10 AF to DFSDM2_CKOUT
    GPIOB->OSPEEDR |= 0b10 << GPIO_OSPEEDR_OSPEED10_Pos;	// set ADC_CLK as fast speed output

	DFSDM2_Channel0->CHCFGR1 |= ((SYSCLK/ADCCLK - 1) << DFSDM_CHCFGR1_CKOUTDIV_Pos);

	DFSDM2_Channel1->CHCFGR1 |= (1 << DFSDM_CHCFGR1_SPICKSEL_Pos);	// Set channel 1 clock source CKOUT
	DFSDM2_Channel2->CHCFGR1 |= (1 << DFSDM_CHCFGR1_SPICKSEL_Pos);	// Set channel 2 clock source CKOUT
	DFSDM2_Channel3->CHCFGR1 |= (1 << DFSDM_CHCFGR1_SPICKSEL_Pos);	// Set channel 3 clock source CKOUT

	// DFSDM2_Filter1->FLTCR1 |= (1 << DFSDM_FLTCR1_RCH_Pos);	// Set filter 1 to channel 1
	// DFSDM2_Filter2->FLTCR1 |= (2 << DFSDM_FLTCR1_RCH_Pos);	// Set filter 2 to channel 2
	// DFSDM2_Filter3->FLTCR1 |= (3 << DFSDM_FLTCR1_RCH_Pos);	// Set filter 3 to channel 3

    DFSDM2_Filter1->FLTJCHGR = 0b1 << 1;	// Set filter 1 to channel 1
    DFSDM2_Filter2->FLTJCHGR = 0b1 << 2;	// Set filter 2 to channel 2
    DFSDM2_Filter3->FLTJCHGR = 0b1 << 3;	// Set filter 3 to channel 3

	DFSDM2_Filter1->FLTFCR |= (3 << DFSDM_FLTFCR_FORD_Pos);	// Set Filter 1 to Sinc^3
	DFSDM2_Filter2->FLTFCR |= (3 << DFSDM_FLTFCR_FORD_Pos);	// Set Filter 2 to Sinc^3
	DFSDM2_Filter3->FLTFCR |= (3 << DFSDM_FLTFCR_FORD_Pos);	// Set Filter 3 to Sinc^3

	DFSDM2_Filter1->FLTFCR |= (DFSDM_FOSR << DFSDM_FLTFCR_FOSR_Pos);	// Set Filter 1 oversample
	DFSDM2_Filter2->FLTFCR |= (DFSDM_FOSR << DFSDM_FLTFCR_FOSR_Pos);	// Set Filter 2 oversample
	DFSDM2_Filter3->FLTFCR |= (DFSDM_FOSR << DFSDM_FLTFCR_FOSR_Pos);	// Set Filter 3 oversample

	DFSDM2_Channel1->CHCFGR2 |= 8 << DFSDM_CHCFGR2_DTRBS_Pos; 	// Right-shift data by 8 to fit 32bit into 24bit
	DFSDM2_Channel2->CHCFGR2 |= 8 << DFSDM_CHCFGR2_DTRBS_Pos; 	// Right-shift data by 8 to fit 32bit into 24bit
	DFSDM2_Channel3->CHCFGR2 |= 8 << DFSDM_CHCFGR2_DTRBS_Pos; 	// Right-shift data by 8 to fit 32bit into 24bit

    // DFSDM2_Filter1->FLTCR1 |= DFSDM_FLTCR1_RSYNC;	// Sync with filter 0
    // DFSDM2_Filter2->FLTCR1 |= DFSDM_FLTCR1_RSYNC;	// Sync with filter 0
    // DFSDM2_Filter3->FLTCR1 |= DFSDM_FLTCR1_RSYNC;	// Sync with filter 0

    DFSDM2_Filter1->FLTCR1 |= 0x0 << DFSDM_FLTCR1_JEXTSEL_Pos;    // Use TIM1_TRGO3 as the trigger
    DFSDM2_Filter2->FLTCR1 |= 0x0 << DFSDM_FLTCR1_JEXTSEL_Pos;    // Use TIM1_TRGO3 as the trigger
    DFSDM2_Filter3->FLTCR1 |= 0x0 << DFSDM_FLTCR1_JEXTSEL_Pos;    // Use TIM1_TRGO3 as the trigger

    DFSDM2_Filter1->FLTCR1 |= 0b01 << DFSDM_FLTCR1_JEXTEN_Pos;    // Rising edge trigger
    DFSDM2_Filter2->FLTCR1 |= 0b01 << DFSDM_FLTCR1_JEXTEN_Pos;    // Rising edge trigger
    DFSDM2_Filter3->FLTCR1 |= 0b01 << DFSDM_FLTCR1_JEXTEN_Pos;    // Rising edge trigger

    // Set short circuit detection threshold
    DFSDM2_Channel1->CHAWSCDR |= DFSDM_SHORT_CIRCUIT_BIT_COUNT << DFSDM_CHAWSCDR_SCDT_Pos;
    DFSDM2_Channel2->CHAWSCDR |= DFSDM_SHORT_CIRCUIT_BIT_COUNT << DFSDM_CHAWSCDR_SCDT_Pos;
    DFSDM2_Channel3->CHAWSCDR |= DFSDM_SHORT_CIRCUIT_BIT_COUNT << DFSDM_CHAWSCDR_SCDT_Pos;

    // Set short circuit detect break output signal to TIM1 break input
    DFSDM2_Channel1->CHAWSCDR |= 0b1 << DFSDM_CHAWSCDR_BKSCD_Pos;
    DFSDM2_Channel2->CHAWSCDR |= 0b1 << DFSDM_CHAWSCDR_BKSCD_Pos;
    DFSDM2_Channel3->CHAWSCDR |= 0b1 << DFSDM_CHAWSCDR_BKSCD_Pos;

    // TODO: set analog watchdog thresholds

	DFSDM2_Channel1->CHCFGR1 |= DFSDM_CHCFGR1_CHEN; 	// Enable Channel 1
	DFSDM2_Channel2->CHCFGR1 |= DFSDM_CHCFGR1_CHEN; 	// Enable Channel 2
	DFSDM2_Channel3->CHCFGR1 |= DFSDM_CHCFGR1_CHEN; 	// Enable Channel 3
	DFSDM2_Channel0->CHCFGR1 |= DFSDM_CHCFGR1_DFSDMEN; 	// Global enable DFSDM

	DFSDM2_Filter1->FLTCR1 |= DFSDM_FLTCR1_DFEN;	// Enable Filter 1
	DFSDM2_Filter2->FLTCR1 |= DFSDM_FLTCR1_DFEN;	// Enable Filter 2
	DFSDM2_Filter3->FLTCR1 |= DFSDM_FLTCR1_DFEN;	// Enable Filter 3
}


/*!
    \brief Enable short circuit detection to immediately disable PWM output
*/
void current_sense_interface::enable_short_circuit_detection(){
    // Enable short circuit detection
    DFSDM2_Channel1->CHCFGR1 |= DFSDM_CHCFGR1_SCDEN;
    DFSDM2_Channel2->CHCFGR1 |= DFSDM_CHCFGR1_SCDEN;
    DFSDM2_Channel3->CHCFGR1 |= DFSDM_CHCFGR1_SCDEN;
}


/*!
    \brief Disable short circuit detection to immediately disable PWM output
*/
void current_sense_interface::disable_short_circuit_detection(){
    // Disable short circuit detection
    DFSDM2_Channel1->CHCFGR1 &= ~DFSDM_CHCFGR1_SCDEN;
    DFSDM2_Channel2->CHCFGR1 &= ~DFSDM_CHCFGR1_SCDEN;
    DFSDM2_Channel3->CHCFGR1 &= ~DFSDM_CHCFGR1_SCDEN;
}


/*!
    \brief Read and convert data from DFSDM filters
    
    \note Returns 1 if a sample is not completed at the time of call
*/
uint32_t current_sense_interface::get_currents(){

    //  check if conversions are all done
    //if(!(DFSDM2_Filter1->FLTISR & DFSDM_FLTISR_REOCF) || !(DFSDM2_Filter2->FLTISR & DFSDM_FLTISR_REOCF) || !(DFSDM2_Filter3->FLTISR & DFSDM_FLTISR_REOCF)){
    if(!(DFSDM2_Filter1->FLTISR & DFSDM_FLTISR_JEOCF) || !(DFSDM2_Filter2->FLTISR & DFSDM_FLTISR_JEOCF) || !(DFSDM2_Filter3->FLTISR & DFSDM_FLTISR_JEOCF)){   // check if DFSDM conversions are complete
        incomplete_conversion_count++;
        return 1;
    }
    incomplete_conversion_count = 0;

    // get conversion data from DFSDM
    // TODO: verify conversion divisor
    // TODO: consider shifting a more optimal number of bits to get higher resolution
    // phase_U_milliamps = ((int32_t)(DFSDM2_Filter1->FLTRDATAR & 0xFFFFFF00)) / conversion_divisor_milliamps;
    // phase_V_milliamps = ((int32_t)(DFSDM2_Filter2->FLTRDATAR & 0xFFFFFF00)) / conversion_divisor_milliamps;
    // phase_W_milliamps = ((int32_t)(DFSDM2_Filter3->FLTRDATAR & 0xFFFFFF00)) / conversion_divisor_milliamps;

    phase_U_milliamps = ((int32_t)(DFSDM2_Filter1->FLTJDATAR & 0xFFFFFF00)) / conversion_divisor_milliamps;
    phase_V_milliamps = ((int32_t)(DFSDM2_Filter2->FLTJDATAR & 0xFFFFFF00)) / conversion_divisor_milliamps;
    phase_W_milliamps = ((int32_t)(DFSDM2_Filter3->FLTJDATAR & 0xFFFFFF00)) / conversion_divisor_milliamps;

    return 0;

}


/*!
    \brief Start sample
*/
void current_sense_interface::start_sample(){

    // start all conversions for next cycle
    // TODO: Ensure DFSDM conversions occur on PWM timer direction flips
    DFSDM2_Filter1->FLTCR1 |= DFSDM_FLTCR1_RSWSTART;	// Start Filter 1 Conversion
    DFSDM2_Filter2->FLTCR1 |= DFSDM_FLTCR1_RSWSTART;	// Start Filter 2 Conversion
    DFSDM2_Filter3->FLTCR1 |= DFSDM_FLTCR1_RSWSTART;	// Start Filter 3 Conversion

}


/*!
    \brief Check which channels have detected a short circuit
*/
uint32_t current_sense_interface::short_circuit_detected(){
    return (DFSDM2_Filter0->FLTISR & DFSDM_FLTISR_SCDF_Msk) >> (DFSDM_FLTISR_SCDF_Pos+1);
}

/*!
    \brief Clear short circuit detections
*/
void current_sense_interface::clear_short_circuit_detected(){
    DFSDM2_Filter0->FLTICR &= ~(DFSDM_FLTICR_CLRSCDF_Msk);
}

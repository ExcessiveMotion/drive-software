#include "adc_interface.h"


adc_interface::adc_interface(logging* logs){
    this->logs = logs;
}


/*!
    \brief Initialize hardware and start continuous conversions
    
    \note Run this after clocks are configured but before the main loop is started
*/
void adc_interface::init(){

    // Setup GPIO pins for analog inputs

    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;	// Enable GPIOA Peripheral Clock
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;	// Enable GPIOB Peripheral Clock
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;	// Enable GPIOC Peripheral Clock

    /*
    ADC input pins:
    PA0
    PA1
    PA2
    PA3
    PA4
    PA5
    PA6
    PA7

    PB1

    PC0
    PC1
    */

    // Set to analog mode
    GPIOA->MODER |=  (  GPIO_MODER_MODER0 |
                        GPIO_MODER_MODER1 |
                        GPIO_MODER_MODER2 |
                        GPIO_MODER_MODER3 |
                        GPIO_MODER_MODER4 |
                        GPIO_MODER_MODER5 |
                        GPIO_MODER_MODER6 | 
                        GPIO_MODER_MODER7);
    GPIOB->MODER |=  (GPIO_MODER_MODER1);
    GPIOC->MODER |=  (GPIO_MODER_MODER0 | GPIO_MODER_MODER1);

    // Clear pull-up/pull-down bits
    GPIOA->PUPDR &= ~(  GPIO_PUPDR_PUPD0 |
                        GPIO_PUPDR_PUPD1 |
                        GPIO_PUPDR_PUPD2 |
                        GPIO_PUPDR_PUPD3 |
                        GPIO_PUPDR_PUPD4 |
                        GPIO_PUPDR_PUPD5 |
                        GPIO_PUPDR_PUPD6 |
                        GPIO_PUPDR_PUPD7);
    GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPD1);
    GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPD0 | GPIO_PUPDR_PUPD1);

    // ADC is driven by the APB2 clock
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;     // enable ADC clock

    // ADC sample clock is derived from the APB2 clock and a configurable divider. 36MHz MAX
    // APB2 clock is 100MHz so we must set the divider to /4 which gives a 25Mhz ADC sample clock
    ADC->CCR = ADC_CCR_TSVREFE | (0b1 << ADC_CCR_ADCPRE_Pos);   // enable internal temp sensor and set clock prescaler

    ADC1->CR1 |= ADC_CR1_SCAN;  // enable scan mode

    ADC1->CR2 |= ADC_CR2_DMA | ADC_CR2_DDS;   // Enable DMA

    //ADC1->SMPR1 and ADC1->SMPR2 may be used to configure the number of samples taken on each ADC channel, default is the lowest (3x)

    // setup sample sequence
    ADC1->SQR3 |= (0 << ADC_SQR3_SQ1_Pos);  // Phase U voltage
    ADC1->SQR3 |= (1 << ADC_SQR3_SQ2_Pos);  // Phase V voltage
    ADC1->SQR3 |= (2 << ADC_SQR3_SQ3_Pos);  // Phase W voltage
    ADC1->SQR3 |= (3 << ADC_SQR3_SQ4_Pos);  // Phase U PFC sense voltage
    ADC1->SQR3 |= (4 << ADC_SQR3_SQ5_Pos);  // Phase V PFC sense voltage
    ADC1->SQR3 |= (5 << ADC_SQR3_SQ6_Pos);  // Phase W PFC sense voltage

    ADC1->SQR2 |= (6 << ADC_SQR2_SQ7_Pos);  // DC bus voltage
    ADC1->SQR2 |= (7 << ADC_SQR2_SQ8_Pos);  // Gate drive voltage UNIMPLEMENTED ON DRIVE
    ADC1->SQR2 |= (9 << ADC_SQR2_SQ9_Pos);  // Heatsink temp 1
    ADC1->SQR2 |= (10 << ADC_SQR2_SQ10_Pos);  // Heatsink temp 2
    ADC1->SQR2 |= (11 << ADC_SQR2_SQ11_Pos);  // Board temp 1
    ADC1->SQR2 |= (18 << ADC_SQR2_SQ12_Pos);  // MCU internal temp

    ADC1->SQR1 |= ((16-1) << ADC_SQR1_L_Pos);  // Set to do 16 total conversions (the ones set above + 4 extra unassigned to match DMA burst size)


    // setup DMA2 stream0 for ADC1

    RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN; // Enable DMA clock

    DMA2_Stream0->CR &= ~(DMA_SxCR_EN); // Disable DMA stream

    while(DMA2_Stream0->CR & (DMA_SxCR_EN_Msk));    // Wait for stream to disable

    // channel 0 is selected by default

    DMA2_Stream0->CR |= DMA_SxCR_CIRC;  // enable circular mode

    DMA2_Stream0->CR |= 0b01 << DMA_SxCR_PSIZE_Pos;     // set peripheral data size to 16bit
    DMA2_Stream0->CR |= 0b10 << DMA_SxCR_MSIZE_Pos;     // set memory data size to 32bit

    DMA2_Stream0->CR |= DMA_SxCR_MINC;      // auto-increment memory address (by set memory size)

    DMA2_Stream0->PAR = (uint32_t)&ADC1->DR;        // use adc data register
    DMA2_Stream0->M0AR = (uint32_t)&raw_adc_data;   // use raw_adc_data as the target memory TODO: verify this works

    DMA2_Stream0->NDTR = 16;    // transfer 16 cycles

    DMA2_Stream0->CR |= 0b01 << DMA_SxCR_MBURST_Pos;    // use 4 beat bursts

    DMA2_Stream0->CR |= 0b10 << DMA_SxCR_PL_Pos;    // High priority

    DMA2_Stream0->FCR |= DMA_SxFCR_DMDIS;   // Disable direcet transfer mode

    DMA2_Stream0->FCR |= 0b10 << DMA_SxFCR_FTH_Pos;     // Set FIFO threshold to full

    DMA2_Stream0->CR |= DMA_SxCR_TCIE;  // Trigger interrupt when transfer to memory is complete

    NVIC_SetPriority(DMA2_Stream0_IRQn, 10); // Set DMA interrupt priority to low
    NVIC_EnableIRQ(DMA2_Stream0_IRQn);  // Configure NVIC for DMA Inturrpt

    DMA2_Stream0->CR |= DMA_SxCR_EN; // Enable DMA stream
    ADC1->CR2 |= ADC_CR2_ADON;  // turn on ADC
}

/*!
    \brief Starts sampling ADC inputs
    
    \note Call this function in sync with the PWM output update
*/
void adc_interface::start_sample(){
    ADC1->CR2 |= ADC_CR2_SWSTART;   // trigger conversion start
}

/*!
    \brief Convert raw ADC data into usable values
*/
void adc_interface::convert_data(){

    // for adc value to voltage:
    // (ADC_VALUE * SENSE_DIVIDER * 3300) / 4095
    
    phase_U_millivolts = ((raw_adc_data[0] & 0xFFFF) * PHASE_SENSE_DIVIDER * 3300) / 4095;
    phase_V_millivolts = ((raw_adc_data[0] >> 16 & 0xFFFF) * PHASE_SENSE_DIVIDER * 3300) / 4095;
    phase_W_millivolts = ((raw_adc_data[1] & 0xFFFF) * PHASE_SENSE_DIVIDER * 3300) / 4095;

    pfc_U_millivolts = ((raw_adc_data[1] >> 16 & 0xFFFF) * PFC_SENSE_DIVIDER * 3300) / 4095;
    pfc_V_millivolts = ((raw_adc_data[2] & 0xFFFF) * PFC_SENSE_DIVIDER * 3300) / 4095;
    pfc_W_millivolts = ((raw_adc_data[2] >> 16 & 0xFFFF) * PFC_SENSE_DIVIDER * 3300) / 4095;
    
    dc_bus_millivolts = ((raw_adc_data[3] & 0xFFFF) * DC_BUS_SENSE_DIVIDER * 3300) / 4095;

    // TODO: implement temperature sensors
}

/*!
    \brief Converts raw ADC data to usable values
    
    \note Call this function in the ADC sample done callback
*/
void adc_interface::dma_interrupt_handler(){

    volatile bool dma_transfer_complete = 0;

    if(DMA2->LISR & DMA_LISR_TCIF0){
        dma_transfer_complete = 1;
        DMA2->LIFCR |= DMA_LIFCR_CTCIF0;
    }


    if(dma_transfer_complete){
        convert_data();
    }
    
}


/*!
    \brief Get DC bus voltage
*/
message_severities adc_interface::get_dc_bus_millivolts(uint32_t* millivolts){
    *millivolts = dc_bus_millivolts;
    return fault;
}

/*!
    \brief Get phase U voltage
*/
message_severities adc_interface::get_phase_U_millivolts(uint32_t* millivolts){
    *millivolts = phase_U_millivolts;
    return fault;
}

/*!
    \brief Get phase V voltage
*/
message_severities adc_interface::get_phase_V_millivolts(uint32_t* millivolts){
    *millivolts = phase_V_millivolts;
    return fault;
}

/*!
    \brief Get phase W voltage
*/
message_severities adc_interface::get_phase_W_millivolts(uint32_t* millivolts){
    *millivolts = phase_W_millivolts;
    return fault;
}

/*!
    \brief Get PFC phase U voltage
*/
message_severities adc_interface::get_pfc_U_millivolts(uint32_t* millivolts){
    *millivolts = pfc_U_millivolts;
    return fault;
}

/*!
    \brief Get PFC phase V voltage
*/
message_severities adc_interface::get_pfc_V_millivolts(uint32_t* millivolts){
    *millivolts = pfc_V_millivolts;
    return fault;
}

/*!
    \brief Get PFC phase W voltage
*/
message_severities adc_interface::get_pfc_W_millivolts(uint32_t* millivolts){
    *millivolts = pfc_W_millivolts;
    return fault;
}

/*!
    \brief Get board temperature
*/
message_severities adc_interface::get_board_temp(int32_t* temp){
    *temp = board_temp;
    return fault;
}

/*!
    \brief Get MCU temperature
*/
message_severities adc_interface::get_mcu_temp(int32_t* temp){
    *temp = mcu_temp;
    return fault;
}

/*!
    \brief Get heatsink 1 temperature
*/
message_severities adc_interface::get_heatsink_1_temp(int32_t* temp){
    *temp = heatsink_1_temp;
    return fault;
}

/*!
    \brief Get heatsink 2 temperature
*/
message_severities adc_interface::get_heatsink_2_temp(int32_t* temp){
    *temp = heatsink_2_temp;
    return fault;
}
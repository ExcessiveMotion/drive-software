#include "adc_interface.h"
#include "math.h"

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
                        GPIO_MODER_MODER6);
    GPIOB->MODER |=  (GPIO_MODER_MODER1);
    GPIOC->MODER |=  (GPIO_MODER_MODER0 | GPIO_MODER_MODER1);

    // Clear pull-up/pull-down bits
    GPIOA->PUPDR &= ~(  GPIO_PUPDR_PUPD0 |
                        GPIO_PUPDR_PUPD1 |
                        GPIO_PUPDR_PUPD2 |
                        GPIO_PUPDR_PUPD3 |
                        GPIO_PUPDR_PUPD4 |
                        GPIO_PUPDR_PUPD5 |
                        GPIO_PUPDR_PUPD6);
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

    // mcu temp sensor requires at least 250 clock cycles to sample
    ADC1->SMPR1 |= (0b111 << ADC_SMPR1_SMP18_Pos); // set sampling time for internal temp sensor to 480 cycles (max)

    // phases U, V, W set to 15 cycles
    ADC1->SMPR2 |= (0b001 << ADC_SMPR2_SMP0_Pos);
    ADC1->SMPR2 |= (0b001 << ADC_SMPR2_SMP1_Pos);
    ADC1->SMPR2 |= (0b001 << ADC_SMPR2_SMP2_Pos);

    // pfc sense set to 3 cycles
    ADC1->SMPR2 |= (0b000 << ADC_SMPR2_SMP3_Pos);
    ADC1->SMPR2 |= (0b000 << ADC_SMPR2_SMP4_Pos);
    ADC1->SMPR2 |= (0b000 << ADC_SMPR2_SMP5_Pos);

    // DC bus set to 15 cycles
    ADC1->SMPR2 |= (0b001 << ADC_SMPR2_SMP6_Pos);

    // temp sensors set to 15 cycles
    ADC1->SMPR2 |= (0b001 << ADC_SMPR2_SMP9_Pos);
    ADC1->SMPR1 |= (0b001 << ADC_SMPR1_SMP10_Pos);
    ADC1->SMPR1 |= (0b001 << ADC_SMPR1_SMP11_Pos);

    // setup sample sequence
    ADC1->SQR3 |= (0 << ADC_SQR3_SQ1_Pos);  // Phase U voltage
    ADC1->SQR3 |= (1 << ADC_SQR3_SQ2_Pos);  // Phase V voltage
    ADC1->SQR3 |= (2 << ADC_SQR3_SQ3_Pos);  // Phase W voltage
    //ADC1->SQR3 |= (3 << ADC_SQR3_SQ4_Pos);  // Phase U PFC sense voltage
    //ADC1->SQR3 |= (4 << ADC_SQR3_SQ5_Pos);  // Phase V PFC sense voltage
    //ADC1->SQR3 |= (5 << ADC_SQR3_SQ6_Pos);  // Phase W PFC sense voltage

    ADC1->SQR3 |= (6 << ADC_SQR3_SQ4_Pos);  // DC bus voltage
    //ADC1->SQR2 |= (7 << ADC_SQR2_SQ8_Pos);  // Gate drive voltage UNIMPLEMENTED ON DRIVE
    ADC1->SQR3 |= (9 << ADC_SQR3_SQ5_Pos);  // Ambient air temp
    ADC1->SQR3 |= (10 << ADC_SQR3_SQ6_Pos);  // Heatsink temp
    ADC1->SQR2 |= (11 << ADC_SQR2_SQ7_Pos);  // Board temp
    //ADC1->SQR2 |= (18 << ADC_SQR2_SQ12_Pos);  // MCU internal temp

    ADC1->SQR1 |= ((8-1) << ADC_SQR1_L_Pos);  // Set to do 8 total conversions (the ones set above + 1 extra unassigned to match DMA burst size)

    // PFC needs injected conversions since it is sensitive to PWM timing
    ADC1->CR1 |= ADC_CR1_JDISCEN;  // enable discontinuous inject mode
    ADC1->JSQR |= (3 << ADC_JSQR_JSQ1_Pos); // Phase U PFC sense voltage
    ADC1->JSQR |= (4 << ADC_JSQR_JSQ2_Pos); // Phase V PFC sense voltage
    ADC1->JSQR |= (5 << ADC_JSQR_JSQ3_Pos); // Phase W PFC sense voltage
    ADC1->JSQR |= (1-1 << ADC_JSQR_JL_Pos); // 1 conversion per trigger (not enough time for more)

    ADC1->CR2 |= (0b01 << ADC_CR2_JEXTEN_Pos); // trigger on rising edge
    ADC1->CR2 |= (0b0000 << ADC_CR2_JEXTSEL_Pos); // trigger from TIM1_CH4 event



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
    DMA2_Stream0->M0AR = (uint32_t)&raw_adc_data.data_32;   // use raw_adc_data as the target memory

    DMA2_Stream0->NDTR = 8;    // transfer 8 cycles

    DMA2_Stream0->CR |= 0b01 << DMA_SxCR_MBURST_Pos;    // use 4 beat bursts

    DMA2_Stream0->CR |= 0b10 << DMA_SxCR_PL_Pos;    // High priority

    DMA2_Stream0->FCR |= DMA_SxFCR_DMDIS;   // Disable direcet transfer mode

    DMA2_Stream0->FCR |= 0b10 << DMA_SxFCR_FTH_Pos;     // Set FIFO threshold to full

    DMA2_Stream0->CR |= DMA_SxCR_TCIE;  // Trigger interrupt when transfer to memory is complete

    NVIC_SetPriority(DMA2_Stream0_IRQn, 10); // Set DMA interrupt priority to low
    NVIC_EnableIRQ(DMA2_Stream0_IRQn);  // Configure NVIC for DMA Inturrpt

    DMA2_Stream0->CR |= DMA_SxCR_EN; // Enable DMA stream
    ADC1->CR2 |= ADC_CR2_ADON;  // turn on ADC


    // get cal values for internal temp sensor
    mcu_temp_val_at_30C_cal = *(uint16_t*)(0x1FFF7A2C); // memory locations according to datasheet
    mcu_temp_val_at_110C_cal = *(uint16_t*)(0x1FFF7A2E);

    mcu_temp_cnt_per_C = (mcu_temp_val_at_110C_cal - mcu_temp_val_at_30C_cal) / 80; // 80 degrees between the two calibration points
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

    // if main pwm timer is in up-counting mode, get the last injected conversion for pfc sense
    if(!(TIM1->CR1 & TIM_CR1_DIR)){
        pfc_millivolts[pfc_sense_index] = (ADC1->JDR1 * ADC_HV_SENSE_DIVIDER * 3300) / 4095;
        pfc_sense_index++;
        pfc_sense_index &= 0b11; // wrap around to 0 after 3 samples (technically shouldn't need this, but just in case)
        if(ADC1->SR & ADC_SR_JEOC){ // reached end of all 3 injected conversions
            ADC1->SR &= ~ADC_SR_JEOC; // clear injected conversion complete flag
            pfc_sense_index = 0; // reset index to 0
        }
    }
    
    phase_U_millivolts = (raw_adc_data.data_16[0] * ADC_HV_SENSE_DIVIDER * 3300) / 4095;
    phase_V_millivolts = (raw_adc_data.data_16[1] * ADC_HV_SENSE_DIVIDER * 3300) / 4095;
    phase_W_millivolts = (raw_adc_data.data_16[2] * ADC_HV_SENSE_DIVIDER * 3300) / 4095;
    
    dc_bus_millivolts = (raw_adc_data.data_16[3] * ADC_HV_SENSE_DIVIDER * 3300) / 4095;

    if(dc_bus_millivolts > MAX_DC_BUS_VOLTAGE*1000){
        logs->log_persistent_active((uint32_t)system_messages::overvoltage);
    }
    else{
        logs->log_persistent_inactive((uint32_t)system_messages::overvoltage);
    }
    if(dc_bus_millivolts < MIN_DC_BUS_VOLTAGE*1000){
        logs->log_persistent_active((uint32_t)system_messages::undervoltage);
    }
    else{
        logs->log_persistent_inactive((uint32_t)system_messages::undervoltage);
    }


    // mcu temp is not used since it requires a long sample time and is not very accurate
    //mcu_temp = ((int16_t(raw_adc_data.data_16[10]) - int16_t(mcu_temp_val_at_30C_cal)) / mcu_temp_cnt_per_C + 30.0f); // convert to degrees C

    // convert temp sensors to volts
    float v_board_temp = (raw_adc_data.data_16[6] * 3.3f) / 4095.0f;
    float v_heatsink_temp = (raw_adc_data.data_16[5] * 3.3f) / 4095.0f;
    float v_air_in_temp = (raw_adc_data.data_16[4] * 3.3f) / 4095.0f;


    if(v_board_temp < .01f){
        board_temp = 200.0f; // set to a high value to trigger a fault
        logs->add((uint32_t)system_messages::temp_sensor_fail);
    }
    else{
        float r_board_temp = 10000.0f *((3.3f / v_board_temp) - 1.0f);
        board_temp = 1.0f / (log(r_board_temp / 10000.0f) / BOARD_NTC_THERM_BETA + 1.0f / 298.15f) - 273.15f;
    }

    if(v_heatsink_temp < .01f){
        heatsink_temp = 200.0f; // set to a high value to trigger a fault
        logs->add((uint32_t)system_messages::temp_sensor_fail);
    }
    else{
        float r_heatsink_temp = 10000.0f *((3.3f / v_heatsink_temp) - 1.0f);
        heatsink_temp = 1.0f / (log(r_heatsink_temp / 10000.0f) / HEATSINK_NTC_THERM_BETA + 1.0f / 298.15f) - 273.15f;
    }

    if(v_air_in_temp < .01f){
        air_in_temp = 200.0f; // set to a high value to trigger a fault
        logs->add((uint32_t)system_messages::temp_sensor_fail);
    }
    else{
        float r_air_in_temp = 10000.0f *((3.3f / v_air_in_temp) - 1.0f);
        air_in_temp = 1.0f / (log(r_air_in_temp / 10000.0f) / HEATSINK_NTC_THERM_BETA + 1.0f / 298.15f) - 273.15f;
    }

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
    *millivolts = pfc_millivolts[0];
    return fault;
}

/*!
    \brief Get PFC phase V voltage
*/
message_severities adc_interface::get_pfc_V_millivolts(uint32_t* millivolts){
    *millivolts = pfc_millivolts[1];
    return fault;
}

/*!
    \brief Get PFC phase W voltage
*/
message_severities adc_interface::get_pfc_W_millivolts(uint32_t* millivolts){
    *millivolts = pfc_millivolts[2];
    return fault;
}

/*!
    \brief Get board temperature
*/
message_severities adc_interface::get_board_temp(float* temp){
    *temp = board_temp;
    return fault;
}

/*!
    \brief Get MCU temperature
*/
message_severities adc_interface::get_mcu_temp(float* temp){
    *temp = mcu_temp;
    return fault;
}

/*!
    \brief Get heatsink 1 temperature
*/
message_severities adc_interface::get_heatsink_temp(float* temp){
    *temp = heatsink_temp;
    return fault;
}

/*!
    \brief Get heatsink 2 temperature
*/
message_severities adc_interface::get_air_in_temp(float* temp){
    *temp = air_in_temp;
    return fault;
}
#include "communication.h"
#include <math.h>


communication::communication(logging* logs){
	this->logs = logs;
}

/*!
    \brief Initialize hardware
    
    \note Run this after clocks are configured but before the main loop is started
*/
void communication::init(){
    
	RCC->APB2ENR |= RCC_APB2ENR_USART6EN;     // enable clock (100 MHz)
	RCC->AHB1ENR |= RCC_AHB1ENR_CRCEN;	// enable CRC clock

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;    // Enable GPIOA  Clock
	GPIOA->MODER |= GPIO_MODER_MODER11_1;	// set PA11 (TX) as alternate function
	GPIOA->MODER |= GPIO_MODER_MODER12_1;	// set PA12 (RX) as alternate function
	GPIOA->OSPEEDR |= 0b10 << GPIO_OSPEEDR_OSPEED11_Pos;	// set TX as fast speed output
	GPIOA->AFR[1] |= 8 << GPIO_AFRH_AFSEL11_Pos;	// set PA11 alternate function to 8 (USART6)
	GPIOA->AFR[1] |= 8 << GPIO_AFRH_AFSEL12_Pos;	// set PA12 alternate function to 8 (USART6)

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;	// Enable GPIOC  Clock
	GPIOC->MODER = (GPIOC->MODER & ~GPIO_MODER_MODER8) | GPIO_MODER_MODER8_0;		// set TX_EN (PC8) to output


	USART6->CR1 |= USART_CR1_OVER8;	// set 8x oversampling

	USART6->CR1 |= USART_CR1_UE;	// Enable USART6


	// set USARTDIV to 1 (maximum baudrate of 12.5 Mbit/s)
	USART6->BRR |= 1<<USART_BRR_DIV_Mantissa_Pos;	// Program Baud rate Mantissa
	USART6->BRR |= 0<<USART_BRR_DIV_Fraction;	// Program Baud rate fraction

	USART6->CR1 |= USART_CR1_IDLEIE;	// enable idle detect interrupt
	USART6->CR1 |= USART_CR1_TCIE;	// enable transmission complete interrupt

	USART6->CR1 |= USART_CR1_RE;	// Enable Receiver

	// setup DMA2 stream1 for USART6 RX (stream 2 is also available)
	// setup DMA2 stream6 for USART6 TX	(stream 7 is also available)

    RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN; // Enable DMA clock

    DMA2_Stream1->CR &= ~(DMA_SxCR_EN); // Disable DMA stream
	DMA2_Stream6->CR &= ~(DMA_SxCR_EN); // Disable DMA stream

    while(DMA2_Stream1->CR & (DMA_SxCR_EN_Msk)){}    // Wait for stream to disable

	while(DMA2_Stream6->CR & (DMA_SxCR_EN_Msk)){}    // Wait for stream to disable

	DMA2_Stream1->CR |= 5 << DMA_SxCR_CHSEL_Pos;	// select channel 5 for rx stream
	DMA2_Stream6->CR |= 5 << DMA_SxCR_CHSEL_Pos;	// select channel 5 for tx stream

    DMA2_Stream1->CR |= 0b00 << DMA_SxCR_PSIZE_Pos;     // set peripheral data size to 8bit (default)
	DMA2_Stream6->CR |= 0b00 << DMA_SxCR_PSIZE_Pos;     // set peripheral data size to 8bit (default)
    DMA2_Stream1->CR |= 0b10 << DMA_SxCR_MSIZE_Pos;     // set memory data size to 32bit
	DMA2_Stream6->CR |= 0b10 << DMA_SxCR_MSIZE_Pos;     // set memory data size to 32bit

    DMA2_Stream1->CR |= DMA_SxCR_MINC;      // auto-increment memory address (by set memory size)
	DMA2_Stream6->CR |= DMA_SxCR_MINC;      // auto-increment memory address (by set memory size)

    DMA2_Stream1->PAR = (uint32_t)&USART6->DR;        // use USART6 data register (rx)
	DMA2_Stream6->PAR = (uint32_t)&USART6->DR;        // use USART6 data register (tx)

    DMA2_Stream1->M0AR = (uint32_t)&rx.data_words;   // use rx_data as the memory for RX data
	DMA2_Stream6->M0AR = (uint32_t)&tx.data_words;   // use tx_data as the memory for TX data

	DMA2_Stream6->CR |= DMA_SxCR_DIR_0;		// configure TX stream for memory-to-peripheral

    set_rx_packet_length(4);	// default packet size
	set_tx_packet_length(4);

    //DMA2_Stream1->CR |= 0b01 << DMA_SxCR_MBURST_Pos;    // use 4 beat bursts
	//DMA2_Stream6->CR |= 0b01 << DMA_SxCR_MBURST_Pos;    // use 4 beat bursts

    DMA2_Stream1->CR |= 0b11 << DMA_SxCR_PL_Pos;    // Very high priority
	DMA2_Stream6->CR |= 0b11 << DMA_SxCR_PL_Pos;    // Very high priority

    DMA2_Stream1->FCR |= DMA_SxFCR_DMDIS;   // Disable direct transfer mode
	DMA2_Stream6->FCR |= DMA_SxFCR_DMDIS;   // Disable direct transfer mode

    //DMA2_Stream1->FCR |= 0b10 << DMA_SxFCR_FTH_Pos;     // Set FIFO threshold to full
	//DMA2_Stream6->FCR |= 0b10 << DMA_SxFCR_FTH_Pos;     // Set FIFO threshold to full

    DMA2_Stream1->CR |= DMA_SxCR_TCIE;  // Trigger interrupt when RX transfer to memory is complete
	//DMA2_Stream6->CR |= DMA_SxCR_TCIE;  // Trigger interrupt when TX transfer to peripheral is complete

	USART6->CR3 |= USART_CR3_DMAT;	// Enable DMA for tx

    NVIC_EnableIRQ(DMA2_Stream1_IRQn);  // Configure NVIC for DMA RX Interrupt	TODO: make sure this priority is lower than the PWM update IRQ
	//NVIC_EnableIRQ(DMA2_Stream6_IRQn);  // Configure NVIC for DMA TX Interrupt
	NVIC_SetPriority(DMA2_Stream1_IRQn, 5);
	NVIC_SetPriority(USART6_IRQn, 2);
	NVIC_EnableIRQ(USART6_IRQn);	// Enable USART6 interrupts
	

    DMA2_Stream1->CR |= DMA_SxCR_EN; // Enable DMA RX stream
	//DMA2_Stream6->CR |= DMA_SxCR_EN; // Enable DMA TX stream

	//TODO: verify no AHB error will be generated due to crossing a 1Kbyte boundary

	// setup us timer for synchronization
	timer_us_init();
}

bool communication::is_ok(){
	return !timed_out;
}

void communication::update_timeout(){
	// needs to be signed because the comm interrupt could fire part way through this function and result in a negative diff
	int64_t diff = get_microseconds() - last_valid_packet_time_us;
	if(diff > timeout_limit_us){
		if(!timed_out){
			//logs->add_log("Communication timeout", message_severities::warning);	// TODO: add a log message
			reset_communication();	// reset configs so the device is in a known state for reconnection
		}
		timed_out = true;
	}
}

void communication::timer_us_init(){
	// setup us counting
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;		// Enable TIM2 Clock
    __DSB();
	TIM2->PSC = (SYSCLK*1e6 / 1e6) - 1; // Prescaler to count in microseconds
    TIM2->EGR |= TIM_EGR_UG; // Generate an update event to update the prescaler
	TIM2->ARR = 0xFFFFFFFF;  // Max auto-reload value (2^32 - 1)
    TIM2->DIER |= TIM_DIER_UIE; // Enable update interrupt

	// setup input capture to measure serial RX starting edges
	// TIM2_CH1 pin PA15 is externally connected to USART6 RX pin PA12
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;    // Enable GPIOA  Clock
	GPIOA->MODER |= GPIO_MODER_MODER15_1;	// set PA15 (CH2) as alternate function
	GPIOA->AFR[1] |= 1 << GPIO_AFRH_AFSEL15_Pos;	// set PA15 alternate function to 1 (TIM2)

	TIM2->CCMR1 |= 0b01 << TIM_CCMR1_CC1S_Pos;	// set CH1 to input capture
	TIM2->CCER |= 0b01 << TIM_CCER_CC1P_Pos;	// set to capture on falling edge
	TIM2->CCER |= TIM_CCER_CC1E;	// enable capture

	TIM2->CR2 |= 0b101 << TIM_CR2_MMS_Pos;	// use output compare 2 as TRGO output (this is used to reset the main PWM timer)


	TIM2->CR1 |= TIM_CR1_CEN; // Enable TIM2
	NVIC_SetPriority(TIM2_IRQn, 14); // Set TIM2 interrupt priority to low
    NVIC_EnableIRQ(TIM2_IRQn); // Enable TIM2 interrupt
    microseconds = 0; // Initialize the microseconds counter
}

void communication::restart_rx_sync_capture(){
	//volatile uint32_t capture = TIM2->CCR2;	// read the capture register
	TIM2->SR &= ~TIM_SR_CC1IF;	// clear the capture interrupt flag
	TIM2->SR &= ~TIM_SR_CC1OF;	// clear the capture overflow flag
}

void communication::sync_timer_us(){
	// uint32_t capture = TIM2->CCR1;	// read the capture register
	// uint32_t diff = capture % target_rx_period;
	// int8_t trim = 0;

	// if(diff > target_rx_period/2){
	// 	trim = 1;
	// }
	// else if(diff < target_rx_period/2){
	// 	trim = -1;
	// }
}

void communication::set_sync_frequency(uint16_t frequency_hz){
	target_rx_period = 1e6 / frequency_hz;
	allowed_period_error = target_rx_period / 20;	// 5% error allowed
}

void communication::set_pwm_timer_sync_offset_us(uint16_t offset_us){
	pwm_timer_sync_offset_us = offset_us;
}

void communication::resync_system(){
	if(!enable_resync) return;
	enable_resync = false;
	SysTick->VAL = 0;	// reset the systick counter (note this is not a hardware sync so it is not perfect, but close enough)

	// using the last time of an rx broadcast packet, we can calculate when we want to restart the PWM timer
	// this we do want to be exact, so we will use hardware sync

	TIM1->BDTR &= ~TIM_BDTR_MOE;	// disable PWM outputs

	uint32_t reset_time = get_microseconds() + target_rx_period + pwm_timer_sync_offset_us;	// target time

	TIM2->CCER &= ~TIM_CCER_CC2E;	// disable output
	TIM2->CCMR1 |= 0b100 << TIM_CCMR1_OC2M_Pos;	// force output low

	TIM2->CCR2 = reset_time;	// set the reset time
	TIM2->CCMR1 |= 0b001 << TIM_CCMR1_OC2M_Pos;	// trigger output high on match
	TIM2->CCER |= TIM_CCER_CC2E;	// enable output
}

void communication::TIM2_IRQHandler(){
	us_overflow = true;
}

uint64_t communication::get_microseconds(){
	volatile uint64_t us_temp = microseconds & 0xFFFFFFFF00000000; // Clear the lower 32 bits of the microseconds counter
	us_temp = us_overflow ? us_temp + 0x100000000 : us_temp;	// Add 2^32 to the microseconds counter if there was an overflow
	us_overflow = false; // Clear the overflow flag
	us_temp += (TIM2->CNT & 0xFFFFFFFF); // Add the current timer value to the microseconds counter
    microseconds = us_temp; // copy to global variable
    return microseconds;
}

void communication::reset_timeout(){
	timed_out = false;
	last_valid_packet_time_us = get_microseconds();
}

void communication::enable_tx(){
	GPIOC->BSRR |= GPIO_BSRR_BS8;
}

void communication::disable_tx(){
	GPIOC->BSRR |= GPIO_BSRR_BR8;
}

/*!
    \brief Configure DMA for a new TX packet size in 32 bit words, the maximum size is defined by MAX_PACKET_SIZE
*/
void communication::set_tx_packet_length(uint32_t length){

	// bool stream_enabled = DMA2_Stream6->CR & (DMA_SxCR_EN_Msk);

	// if(stream_enabled){
	// 	DMA2_Stream6->CR &= ~(DMA_SxCR_EN); // Disable DMA stream
	// 	while(DMA2_Stream6->CR & (DMA_SxCR_EN_Msk));    // Wait for stream to disable
	// }

	DMA2_Stream6->NDTR = length * 4;	// set number of 8 bit transfer cycles
}


/*!
    \brief Configure DMA for a new RX packet size in 32 bit words, the maximum size is defined by MAX_PACKET_SIZE
*/
void communication::set_rx_packet_length(uint32_t length){

	bool stream_enabled = DMA2_Stream1->CR & (DMA_SxCR_EN_Msk);

	if(stream_enabled){
		DMA2_Stream1->CR &= ~(DMA_SxCR_EN); // Disable DMA stream
		while(DMA2_Stream1->CR & (DMA_SxCR_EN_Msk));    // Wait for stream to disable
	}

	DMA2_Stream1->NDTR = length * 4;	// set number of 8 bit transfer cycles

	if(stream_enabled){
		DMA2_Stream1->CR |= DMA_SxCR_EN; // Enable DMA stream
	}
	expected_rx_length = length;
}


/*!
	\brief Set device address offset from DEVICE_STARTING_ADDRESS
*/
void communication::set_device_address(uint8_t address){
	device_address = address + DEVICE_STARTING_ADDRESS;
}


/*!
    \brief Receive data packet
*/
void communication::start_receive(){
	receive_complete = false;
	receive_started = true;
	USART6->CR3 |= USART_CR3_DMAR;	// Enable DMA for rx
	//USART6->CR3 |= USART_CR3_DMAT;	// Enable DMA for tx
}


/*!
    \brief Send data packet
*/
void communication::start_transmit(){
	
	set_tx_packet_length(expected_tx_length);	// set the number of 32 bit words to transfer
	
	// clear DMA flags
	DMA2->HIFCR |= DMA_HIFCR_CDMEIF6 | DMA_HIFCR_CTEIF6 | DMA_HIFCR_CFEIF6 | DMA_HIFCR_CHTIF6 | DMA_HIFCR_CTCIF6;
	USART6->CR1 &= ~USART_CR1_TE;	// Disable Transmitter
	USART6->SR &= ~USART_SR_TC_Msk;	// clear transmission complete flag

	DMA2_Stream6->CR |= DMA_SxCR_EN; // Enable DMA TX stream

	//USART6->DR = 0b10101010;	// send dummy byte
	enable_tx();
	USART6->CR1 |= USART_CR1_TE;	// Enable Transmitter
	
}


/*!
    \brief RX line is in idle state
*/
bool communication::rx_idle_detected(){
	return (USART6->SR & USART_SR_IDLE_Msk) >> USART_SR_IDLE_Pos;	// return true if IDLE flag is set
}


/*!
    \brief Handle DMA interrupt
*/
void communication::dma_stream1_interrupt_handler(){

	if(DMA2->LISR & DMA_LISR_TCIF1){
        USART6->CR3 &= ~USART_CR3_DMAR;	// Disable DMA for rx
		receive_complete = true;
		receive_started = false;
        DMA2->LIFCR |= DMA_LIFCR_CTCIF1;	// clear trasfer finished flag
    }
}


/*!
    \brief Handle USART6 interrupt
*/
void communication::usart6_interrupt_handler(){

	if(USART6->SR & USART_SR_IDLE_Msk){		// RX IDLE state detected (incomming transmission over)
        
		int8_t result = verify_rx_packet();
		if(result == 0){	// packet addressed to this device
			// interpret sequential data
			interpret_rx_sequential_data();
			// cyclic data can be interpreted outside of the rx handler

			// start TX transmission
			generate_tx_cyclic_data();
			generate_tx_sequential_data();
			start_transmit();
			reset_timeout();
			interpret_rx_cyclic_data();
		}
		else if(result == 1){	// broadcast packet
			reset_timeout();
		}
		else{	// invalid packet
			// do nothing
		}

		//restart_rx_sync_capture();
		clear_rx_idle_flag();
		restart_rx_dma();	// TODO: only do this on invalid packet receive? (maybe)
		start_receive();
    }

	if(USART6->SR & USART_SR_TC){	// transmission complete
		disable_tx();
		USART6->SR &= ~USART_SR_TC_Msk;	// clear transmission complete flag
	}
	
}

uint32_t communication::calculate_crc(uint32_t *data, uint8_t data_length){
	CRC->CR = 0b1;	// clear and reset previous calculation
	for(uint8_t i=0; i<data_length; i++){
		CRC->DR = data[i];
	}
	return CRC->DR;
}

int8_t communication::verify_rx_packet(){
	if(calculate_crc(rx.data_words, expected_rx_length-1) == rx.data_words[expected_rx_length-1]){	// verify CRC is correct
		
		if(rx.data_bytes[0] == 0xFF){	// broadcast address
			return 1;	// broadcast mode
		}
		if(rx.data_bytes[0] == device_address){	// verify device address matches
			//TODO: update registers
			return 0;	// address match
		}
	}
	return -1;	// invalid packet
}

void communication::generate_tx_cyclic_data(){
	// this is meant to be called BEFORE generate_tx_sequential_data

	tx.data_bytes[0] = device_address;	// set device address

	uint16_t offset = 1 + 4 + 4;	// leave room for device address, sequential data control/address and response
	
	if(comm_vars->enable_cyclic_data){
		
		for(uint16_t i = CYCLIC_READ_ADDRESS_POINTER_START; i<CYCLIC_ADDRESS_COUNT+CYCLIC_READ_ADDRESS_POINTER_START; i++){
			uint16_t data_address = *reinterpret_cast<uint16_t*>(comm_var_pointers[i]);
			if(data_address >= VAR_COUNT){
				break;	// invalid address, either end of list or address out of range
			}

			uint8_t data_size = data_info[data_address] >> 4;
			memcpy(&(tx.data_bytes[offset]), comm_var_pointers[data_address], data_size);
			offset += data_size;
		}

		if(!cyclic_mode_enabled){
			//calculate_rx_expected_size();
			cyclic_mode_enabled = true;
		}
	}
	else{
		if(cyclic_mode_enabled){
			//calculate_rx_expected_size();
			cyclic_mode_enabled = false;
		}
	}

	if(offset % 4 != 0){	// pack additional blank bytes to ensure packet is a multiple of 32bits
		memset(&(tx.data_bytes[offset]), 0, 4 - (offset % 4));
		offset += 4 - (offset % 4);
	}
	
	expected_tx_length = offset/4;	// set the number of 32 bit words to transfer
	expected_tx_length++;	// add 1 for the CRC
}

void communication::generate_tx_sequential_data(){
	// this is meant to be called AFTER generate_tx_cyclic_data, and must be inside of the communication rx handler
	
	memcpy(&(tx.data_bytes[1]), &sequential_register_control, 4);	// set sequential data control/address
	memcpy(&(tx.data_bytes[5]), &sequential_register_data_response, 4);	// set sequential data response

	tx.data_words[expected_tx_length-1] = calculate_crc(tx.data_words, expected_tx_length-1);
}

void communication::interpret_rx_sequential_data(){
	memcpy(&sequential_register_control, &(rx.data_bytes[1]), 4);	// get sequential data control/address
	memcpy(&sequential_register_data_input, &(rx.data_bytes[5]), 4);	// get sequential data input
	uint16_t raw_address = sequential_register_control & 0xFFFF;
	if(sequential_register_control & (1 << 16)){	// bit 16 is the write flag
		// attempt to write
		if(controller_set_register(raw_address, &sequential_register_data_input, &sequential_register_data_response) == controller_register_access_result::SUCCESS){
			//sequential_register_data_response = *reinterpret_cast<uint32_t*>(comm_var_pointers[raw_address]);	// success, return the value
			sequential_register_control = sequential_register_control & ~(0xFF << 24);	// clear bits 24-31 (device response)
			sequential_register_control |= 1 << 24;	// set bit 24 to indicate success
		}
		else{	// fail
			//sequential_register_data_response = 0;
			sequential_register_control = sequential_register_control & ~(0xFF << 24);	// clear bits 24-31 (device response)
			sequential_register_control |= 1 << 25;	// set bit 25 to indicate failure
			logs->add(communication_messages::invalid_address);
		}
	}
	else{
		// attempt to read
		if(controller_get_register(raw_address, &sequential_register_data_response) == controller_register_access_result::SUCCESS){
			sequential_register_control = sequential_register_control & ~(0xFF << 24);	// clear bits 24-31 (device response)
			sequential_register_control |= 1 << 24;	// set bit 24 to indicate success
		}
		else{	// fail
			sequential_register_data_response = 0;
			sequential_register_control = sequential_register_control & ~(0xFF << 24);	// clear bits 24-31 (device response)
			sequential_register_control |= 1 << 25;	// set bit 25 to indicate failure
			logs->add(communication_messages::invalid_address);
		}
	}

	if(comm_vars->enable_cyclic_data && !cyclic_mode_enabled){
		calculate_rx_expected_size();
	}
	else if (!comm_vars->enable_cyclic_data && cyclic_mode_enabled)
	{
		calculate_rx_expected_size();
	}
	

}

void communication::interpret_rx_cyclic_data(){
	uint16_t offset = 1 + 4 + 4;	// start after device address, sequential data control/address and response
	
	if(!comm_vars->enable_cyclic_data){
		return;	// cyclic data is disabled, do nothing
	}

	bool error = false;
	
	for(uint16_t i = CYCLIC_WRITE_ADDRESS_POINTER_START; i<CYCLIC_ADDRESS_COUNT+CYCLIC_WRITE_ADDRESS_POINTER_START; i++){
		uint16_t data_address = *reinterpret_cast<uint16_t*>(comm_var_pointers[i]);
		if(data_address >= VAR_COUNT){
			break;	// invalid address, either end of list or address out of range
		}

		if(controller_set_register(data_address, &(rx.data_bytes[offset])) == controller_register_access_result::SUCCESS){
			// success
		}
		else{
			error = true;
		}
		
		offset += data_info[data_address] >> 4;
	}

	if(error){
		logs->add(communication_messages::invalid_cyclic_config);
	}
}

void communication::calculate_rx_expected_size(){
	uint16_t offset = 1 + 4 + 4;	// start after device address, sequential data control/address and response

	for(uint16_t i = CYCLIC_WRITE_ADDRESS_POINTER_START; i<CYCLIC_ADDRESS_COUNT+CYCLIC_WRITE_ADDRESS_POINTER_START; i++){

		if(!comm_vars->enable_cyclic_data){
			break;
		}

		uint16_t data_address = *reinterpret_cast<uint16_t*>(comm_var_pointers[i]);
		if(data_address >= VAR_COUNT){
			break;	// invalid address, either end of list or address out of range
		}		
		offset += data_info[data_address] >> 4;
	}

	if(offset % 4 != 0){	// pack additional blank bytes to ensure packet is a multiple of 32bits
		offset += 4 - (offset % 4);
	}
	
	offset /= 4;	// convert to the number of 32 bit words to transfer
	offset++;	// add 1 for the CRC

	set_rx_packet_length(offset);

}

/*!
    \brief Restart RX dma to re-sync with next data packet
*/
void communication::restart_rx_dma(){
	receive_started = false;
	receive_complete = false;
	DMA2_Stream1->CR &= ~(DMA_SxCR_EN); // Disable DMA stream
	while(DMA2_Stream1->CR & (DMA_SxCR_EN_Msk));    // Wait for stream to disable
	DMA2->LIFCR |= DMA_LIFCR_CTCIF1 | DMA_LIFCR_CHTIF1 | DMA_LIFCR_CTEIF1 | DMA_LIFCR_CDMEIF1 | DMA_LIFCR_CFEIF1;	// clear any interrupt flags
	DMA2_Stream1->CR |= DMA_SxCR_EN; // Enable DMA stream
}


/*!
    \brief Clear RX IDLE detection flag
*/
void communication::clear_rx_idle_flag(){
	[[maybe_unused]] volatile uint32_t temp = USART6->SR;	// read SR register
	temp = USART6->DR;	// read DR register
}


/*!
	\brief Reset cyclic data and disable cyclic mode
*/
void communication::reset_communication(){

	cyclic_mode_enabled = false;
	comm_vars->enable_cyclic_data = false;
	for(uint16_t i = CYCLIC_READ_ADDRESS_POINTER_START; i<CYCLIC_ADDRESS_COUNT+CYCLIC_READ_ADDRESS_POINTER_START; i++){
		*reinterpret_cast<uint16_t*>(comm_var_pointers[i]) = 65535;	// set to invalid address (not configured)
	}
	for(uint16_t i = CYCLIC_WRITE_ADDRESS_POINTER_START; i<CYCLIC_ADDRESS_COUNT+CYCLIC_WRITE_ADDRESS_POINTER_START; i++){
		*reinterpret_cast<uint16_t*>(comm_var_pointers[i]) = 65535;	// set to invalid address (not configured)
	}
	calculate_rx_expected_size();
}


communication::controller_register_access_result communication::controller_set_register(uint16_t raw_address, void* raw_value){
	if(raw_address >= VAR_COUNT){	// ouside of range
		return controller_register_access_result::FAIL;
	}
	if(data_info[raw_address] & 0b10){	// write allowed
		memcpy(comm_var_pointers[raw_address], raw_value, data_info[raw_address] >> 4);
		return controller_register_access_result::SUCCESS;
	}
	return controller_register_access_result::FAIL;
}

communication::controller_register_access_result communication::controller_set_register(uint16_t raw_address, void* raw_value, void* response){
	if(raw_address >= VAR_COUNT){	// ouside of range
		return controller_register_access_result::FAIL;
	}
	if(data_info[raw_address] & 0b10){	// write allowed
		memcpy(comm_var_pointers[raw_address], raw_value, data_info[raw_address] >> 4);

		// also copy to response
		memset(response, 0, 4);	// clear response
		memcpy(response, comm_var_pointers[raw_address], data_info[raw_address] >> 4);

		return controller_register_access_result::SUCCESS;
	}
	return controller_register_access_result::FAIL;
}

communication::controller_register_access_result communication::controller_get_register(uint16_t raw_address, void* raw_value){
	if(raw_address >= VAR_COUNT){	// ouside of range
		return controller_register_access_result::FAIL;
	}
	if(data_info[raw_address] & 0b01){	// read allowed
		memcpy(raw_value, comm_var_pointers[raw_address], data_info[raw_address] >> 4);
		return controller_register_access_result::SUCCESS;
	}
	return controller_register_access_result::FAIL;
}
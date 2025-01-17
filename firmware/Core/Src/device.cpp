#include "device.h"

// extern device_struct vars;
// extern void* var_pointers[86];

device::device(){
    comm_vars = &vars;
    comm_var_pointers = var_pointers;
}

void device::init(){
    CPU_init();
    sysTick_init();
 
    #ifdef RELEASE_MODE
        watchdog_init();
        PhasePWM.release_mode();
    #endif

    PhasePWM.release_mode();    // bypass safeties... only do this if testing with a low voltage current limited supply

    logs.init();
    // Initialize all the low level classes
    Comm.init();
    Fans.init();
    UserIO.init();
    CurrentSense.init();
    PhasePWM.init();
    Adc.init();
    Sto.init();


    micros = Comm.micros;
    logs.microseconds = Comm.micros;
    Comm.comm_vars = comm_vars;
    Comm.comm_var_pointers = comm_var_pointers;
    UserIO.micros = Comm.micros;


    // todo: get this from the controller
    Comm.set_sync_frequency(1000); // 1kHz sync frequency
    Comm.set_pwm_timer_sync_offset_us(0); // no offset

    delay_ms(500); // wait for userIO to update

    // wait until we have a valid communication address before initializing communication
    Comm.set_device_address(UserIO.get_switch_states());
    
    delay_ms(500); // wait for communication to update
    Comm.enable_resync = true; // enable resync


    current_mode->request_state(Mode::States::RUN);
    
}

void device::CPU_init(){

    // 100MHz SYSCLK
    assert(SYSCLK == 100);

	FLASH->ACR |= FLASH_ACR_ICEN			// Enable intruction cache
			    | FLASH_ACR_DCEN 			// Enable Date Cache
			    | FLASH_ACR_PRFTEN 			// Enable prefetch
			    | FLASH_ACR_LATENCY_3WS;	// Set Flash latency to 3 wait states

    // HSI clock is used as the PLL input clock (16MHz)
    // set VCO to 2Mhz
    // set PLL_N to get SYSCLK*2
    // set PLL_P to 2 to get SYSCLK for they system
    // set PLL_Q to 5 (SYSCLK*2 / 5) for USB, SDIO, RNG, must be 48Mhz or lower
    // set PLL_R to 2 (SYSCLK*2 / 2) for I2S, DFSDM, must be 96Mhz or lower

	RCC->PLLCFGR = (8 << 0)    // Set PLL_M to 8. The input clock frequency is divided by this value.
	             | (SYSCLK << 6)  // Set PLL_N, the multiplication factor for the PLL. SYSCLK is presumably defined elsewhere, representing the desired system clock frequency.
	             | (0 << 16)   // Set PLL_P to 2 (0 in register corresponds to PLL_P = 2). The PLL output frequency is divided by this value to get the system clock.
	             | (5 << 24)  // Set PLL_Q to 5. This value is used for USB, SDIO, and random number generator clocks
                 | (2 << 28); // Set PLL_R to 2. This value is used for I2S and DFSDM clocks
	
    // Turn on the PLL and wait for it to become stable
	RCC->CR |= RCC_CR_PLLON;  // Enable the PLL
	while (!(RCC->CR & RCC_CR_PLLRDY)); // Wait for PLL to be ready (PLL ready flag)

	// Switch the system clock source to the PLL
	RCC->CFGR &= ~RCC_CFGR_SW;  // Clear the clock switch bits
	RCC->CFGR |= RCC_CFGR_SW_PLL;  // Set the clock source to PLL
	while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL); // Wait until PLL is used as the system clock source

	RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;	// divide by 2 to get 50Mhz for APB1 peripherals (max allowed)

	// Update the SystemCoreClock variable to the new clock speed
	SystemCoreClockUpdate(); // Update the SystemCoreClock global variable with the new clock frequency
	
	SystemInit();	// Initialize system
}

void device::delay_us(uint32_t time_us){
    uint64_t end_time = Comm.get_microseconds() + time_us;
    while (Comm.get_microseconds() < end_time){    // Wait until the desired time has passed
        watchdog_reload();
    }
}

void device::delay_ms(uint32_t time_ms){
    for (uint32_t i = 0; i < time_ms; i++){
        delay_us(1e3); // delay 1ms
    }
}

void device::sysTick_init(){
    SysTick->LOAD = ((SYSCLK*1e6) / SYSTICK_FREQUENCY) - 1; // Set the SysTick timer to count at the desired frequency
    NVIC_SetPriority(USART6_IRQn, 15);  // low priority
    NVIC_EnableIRQ(SysTick_IRQn); // Enable the SysTick interrupt
    SysTick->CTRL = 0b111;	// Enable counter, interrupt, and set clock source to system clock
}

void device::watchdog_init(){
    IWDG->KR = 0x5555; // Enable write access to the IWDG_PR and IWDG_RLR registers
    IWDG->PR = 0b000; // Set the prescaler to 4 (LSI at 32khz / 4)
    IWDG->RLR = 8000 / 1000; // Set the reload value to 1ms
    IWDG->KR = 0xAAAA; // Reload the watchdog timer
    IWDG->KR = 0xCCCC; // Start the watchdog timer
}

void device::watchdog_reload(){
    IWDG->KR = 0xAAAA; // Reload the watchdog timer
}

void device::startup_demo(){
    // Test LEDs and fans
    delay_ms(1000);    // Wait 1 second

    UserIO.set_led_state(0b1111, UserIO.on);
    Fans.set_speed(Fans.max_rpm);

    delay_ms(2000);    // Wait 2 seconds
    UserIO.set_led_state(0b1111, UserIO.blink_slow);
    Fans.set_speed(0);

    // TODO: Add more startup tests
}

void device::run(){
    while(1){   // main loop

        // run flag interrupt handlers
        if(sysTick_flag){
            flagged_sysTick();
        }
        if(tim2_flag){
            flagged_tim2();
        }
        if(i2c1_ev_flag){
            flagged_i2c1_ev();
        }
        if(i2c1_er_flag){
            flagged_i2c1_er();
        }
        if(dma2_stream0_flag){
            flagged_dma2_stream0();
        }
        if(dma2_stream1_flag){
            flagged_dma2_stream1();
        }
        if(tim1_up_tim10_flag){
            flagged_tim1_up_tim10();
        }

        // run additional mode functions
        current_mode->default_run();
        current_mode->run();
        UserIO.run();

    }
}

void device::update(){
    Fans.set_speed(Fans.max_rpm*vars.fan_speed_cmd/0xffff);
    vars.fan_speed_measured = uint16_t(Fans.get_fan_1_speed_rpm() + Fans.get_fan_2_speed_rpm())/2;

    uint32_t dc_mv;
    Adc.get_dc_bus_millivolts(&dc_mv);
    vars.dc_bus_voltage = dc_mv/1000;

    update_leds();
}

void device::update_leds(){
    // LED 1 shown connection status
    if(Comm.is_ok()){
        UserIO.set_led_state(0b0001, UserIO.blink_fast);
    }
    else{
        UserIO.set_led_state(0b0001, UserIO.blink_slow);
    }

    if(PhasePWM.is_enabled()){
        UserIO.set_led_state(0b0010, UserIO.on);
    }
    else{
        UserIO.set_led_state(0b0010, UserIO.off);
    }

    bool sto_good;
    Sto.output_allowed(&sto_good);
    if(sto_good){
        UserIO.set_led_state(0b0100, UserIO.on);
    }
    else{
        UserIO.set_led_state(0b0100, UserIO.off);
    }
}

void device::critical_shutdown(){
    // Disable all hardware and enter a safe state
    UserIO.set_led_state(0b1000, UserIO.blink_fast);
    Fans.set_speed(Fans.max_rpm);

    PhasePWM.disable();
}

void device::SysTick_Handler(void){
    sysTick_flag = true;
}

void device::flagged_sysTick(void){
    Fans.SysTick_Handler();
    UserIO.SysTick_Handler();

    update();

    current_mode->default_systick_handler();
    current_mode->systick_handler();

    sysTick_flag = false;
}

void device::TIM2_IRQHandler(void){
    TIM2->SR &= ~TIM_SR_UIF; // Clear the update interrupt flag
    tim2_flag = true;
}

void device::flagged_tim2(void){
    Comm.TIM2_IRQHandler();
    tim2_flag = false;
}

void device::I2C1_EV_IRQHandler(void){
    //UserIO.I2C1_EV_IRQHandler();    // TODO: make this able to run from the flag handler
    i2c1_ev_flag = true;
}

void device::flagged_i2c1_ev(void){
    i2c1_ev_flag = false;
}

void device::I2C1_ER_IRQHandler(void){
    //UserIO.I2C1_ER_IRQHandler();    // TODO: make this able to run from the flag handler
    i2c1_er_flag = true;
}

void device::flagged_i2c1_er(void){
    i2c1_er_flag = false;
}

void device::DMA2_Stream0_IRQHandler(void){
    if(DMA2->LISR & DMA_LISR_TCIF0){
        DMA2->LIFCR |= DMA_LIFCR_CTCIF0;    // clear trasfer finished flag
    }
    dma2_stream0_flag = true;
}

void device::flagged_dma2_stream0(void){
    dma2_stream0_flag = false;
    Adc.dma_interrupt_handler();
}

void device::DMA2_Stream1_IRQHandler(void){
    Comm.dma_stream1_interrupt_handler();
    dma2_stream1_flag = true;
}

void device::flagged_dma2_stream1(void){
    dma2_stream1_flag = false;
}

void device::TIM1_UP_TIM10_IRQHandler(void){
    // called at 2x the PWM frequency (each time the counter changes direction)

    if (TIM1->SR & TIM_SR_UIF) { // Check if update interrupt flag is set
        TIM1->SR &= ~TIM_SR_UIF; // Clear update interrupt flag
        tim1_up_tim10_flag = true;
    }
}

void device::flagged_tim1_up_tim10(void){
    current_mode->default_tim1_up_irq_handler();
    current_mode->tim1_up_irq_handler();

    if(logs.get_active_severity() == message_severities::critical){
        critical_shutdown();
    }

    Comm.update_timeout();
    watchdog_reload();
    tim1_up_tim10_flag = false;
}

void device::USART6_IRQHandler(void){   // this handler is not flagged since it needs to be called immediately to ensure lowest jitter
    Comm.usart6_interrupt_handler();
}

device::IRQ device::missed_irq = IRQ::NONE;
void device::missed_irq_handler(IRQ irq){
    missed_irq = irq;
}
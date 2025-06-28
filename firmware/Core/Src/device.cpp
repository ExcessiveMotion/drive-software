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


    logs.init();
    logs.comm_vars = comm_vars;
    // Initialize all the low level classes
    Comm.init();
    Fans.init();
    UserIO.init();
    CurrentSense.init();
    Adc.init();
    Sto.init();


    #ifdef RELEASE_MODE
        watchdog_init();
        PhasePWM.release_mode();
    #endif

    //PhasePWM.release_mode();    // bypass safeties... only do this if testing with a low voltage current limited supply

    PhasePWM.init();

    micros = Comm.micros;
    last_comm_time = Comm.last_comm_time;
    logs.microseconds = Comm.micros;
    Comm.comm_vars = comm_vars;
    Comm.comm_var_pointers = comm_var_pointers;
    UserIO.micros = Comm.micros;
    UserIO.sync_micros = Comm.sync_micros;

    Default_Mode.set_time_ptrs(micros, last_comm_time);
    FOC_Current.set_time_ptrs(micros, last_comm_time);
    PFC_Mode.set_time_ptrs(micros, last_comm_time);

    delay_ms(500); // allow system to stabilize before starting
    
    current_mode->request_state(Mode::States::IDLE);

    logs.clear_all(); // clear any faults from undefined startup

    // check for watchdog reset flag
    if(RCC->CSR & RCC_CSR_IWDGRSTF){ // watchdog reset flag is set
        RCC->CSR |= RCC_CSR_RMVF; // clear the reset flag
        logs.add((uint32_t)system_messages::watchdog_timeout);
    }
}

void device::CPU_init(){

    // 100MHz SYSCLK
    assert(SYSCLK == 100);

	FLASH->ACR |= FLASH_ACR_ICEN			// Enable intruction cache
			    | FLASH_ACR_DCEN 			// Enable Date Cache
			    | FLASH_ACR_PRFTEN 			// Enable prefetch
			    | FLASH_ACR_LATENCY_3WS;	// Set Flash latency to 3 wait states

    // attempt to use HSE (25MHz) as the clock source
    RCC->CR |= RCC_CR_HSEBYP; // Bypass HSE since we are using an external clock source (not a crystal)
    RCC->CR |= RCC_CR_HSEON; // Enable HSE clock

    for(int i = 0; i < 1000; i++){
        __NOP(); // wait for HSE to stabilize
    }

    if(!(RCC->CR & RCC_CR_HSERDY)){ // HSE not available
        RCC->CR &= ~RCC_CR_HSEON; // disable HSE clock
    }
    else{
        hse_vcxo_available = true; // HSE is available
    }

    if(!hse_vcxo_available){
    // internal HSI clock mode

    // HSI clock is used as the PLL input clock (16MHz)
    // set VCO to 2Mhz
    // set PLL_N to get SYSCLK*2
    // set PLL_P to 2 to get SYSCLK for the system
    // set PLL_Q to 5 (SYSCLK*2 / 5) for USB, SDIO, RNG, must be 48Mhz or lower
    // set PLL_R to 2 (SYSCLK*2 / 2) for I2S, DFSDM, must be 96Mhz or lower

	RCC->PLLCFGR = (8 << 0)    // Set PLL_M to 8. The input clock frequency is divided by this value.
	             | (SYSCLK << 6)  // Set PLL_N, the multiplication factor for the PLL. SYSCLK is presumably defined elsewhere, representing the desired system clock frequency.
	             | (0 << 16)   // Set PLL_P to 2 (0 in register corresponds to PLL_P = 2). The PLL output frequency is divided by this value to get the system clock.
	             | (5 << 24)  // Set PLL_Q to 5. This value is used for USB, SDIO, and random number generator clocks
                 | (2 << 28); // Set PLL_R to 2. This value is used for I2S and DFSDM clocks
    }
    else{
    // external HSE clock mode (25MHz)

    // HSE clock is used as the PLL input clock (25MHz)
    // set VCO to 1.66667 Mhz
    // set PLL_N to get SYSCLK*2
    // set PLL_P to 2 to get SYSCLK for the system
    // set PLL_Q to 5 (SYSCLK*2 / 5) for USB, SDIO, RNG, must be 48Mhz or lower
    // set PLL_R to 2 (SYSCLK*2 / 2) for I2S, DFSDM, must be 96Mhz or lower

    constexpr uint8_t PLL_N = uint8_t(float(SYSCLK*2.0) / 1.666666666666);

    RCC->PLLCFGR = (15 << 0)    // Set PLL_M to 25. The input clock frequency is divided by this value.
                 | (PLL_N << 6)  // Set PLL_N, the multiplication factor for the PLL. SYSCLK is presumably defined elsewhere, representing the desired system clock frequency.
                 | (0 << 16)   // Set PLL_P to 2 (0 in register corresponds to PLL_P = 2). The PLL output frequency is divided by this value to get the system clock.
                 | (5 << 24)  // Set PLL_Q to 5. This value is used for USB, SDIO, and random number generator clocks
                 | (2 << 28) // Set PLL_R to 2. This value is used for I2S and DFSDM clocks
                 | (RCC_PLLCFGR_PLLSRC_HSE); // Set PLL source to HSE
    }

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
    tim1_update_missed = false; // reset missed update flag
    tim1_up_tim10_flag = false;

    while(1){   // main loop

        // run flag interrupt handlers
        if(sysTick_flag){
            flagged_sysTick();
        }
        if(tim2_flag){
            flagged_tim2();
        }
        if(tim5_flag){
            flagged_tim5();
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
        if(tim1_update_missed){
            logs.add((uint32_t)system_messages::control_deadline_missed);
        }

        // handle requested state changes from controller
        if(vars.requested_state != last_controller_requested_state){    // new requested state
            switch(vars.requested_state){
                case 0:
                    break; // nothing
                case 1:
                    current_mode->request_state(Mode::States::IDLE);
                    break;
                case 2:
                    if(logs.get_active_severity() >= message_severities::error || current_mode == &Default_Mode){
                        // TODO: add an error for trying to start in default mode
                        break; // don't try to start if in error state or default mode
                    }
                    Comm.enable_resync = false; // no resync allowed while running
                    current_mode->request_state(Mode::States::RUN);
                    break;
                case 3:
                    logs.clear_all(); // clear all faults
                    break;
                case 4:
                    if(current_mode->get_state() == Mode::States::IDLE){    // mode changes only allowed if in IDLE state
                        switch(vars.device_mode){
                            case 0:
                                current_mode = &Default_Mode;
                                vars.device_mode = 0;
                                break;
                            case 1:
                                current_mode = &FOC_Current;
                                vars.device_mode = 1;
                                break;
                            case 2:
                                current_mode = &PFC_Mode;
                                vars.device_mode = 2;
                                break;
                            default:
                                current_mode = &Default_Mode;
                                vars.device_mode = 0;
                                logs.add((uint32_t)system_messages::invalid_mode); // invalid mode requested
                                break;
                        }

                        if(current_mode->set_sub_mode(vars.device_mode_sub_config)){
                            logs.add((uint32_t)system_messages::invalid_sub_mode); // invalid sub mode requested
                            vars.device_mode_sub_config = 0; // reset to default sub mode
                        }
                    }
                    break;
                case 5: // system time synchronization
                    if(current_mode->get_state() == Mode::States::IDLE){    // only allowed if in IDLE state
                        if(comm_vars->enable_controller_syncronization && comm_vars->controller_syncronization_valid){
                            Comm.enable_resync = true; // enable resync
                        }
                    }
                    comm_vars->requested_state = 0; // reset requested state
                    break;
                default:
                    logs.add((uint32_t)system_messages::invalid_state); // invalid state requested
                    break;
            }
            last_controller_requested_state = vars.requested_state;
        }

        // handle error states
        if(logs.get_active_severity() == message_severities::error){
            UserIO.set_led_state(0b1000, UserIO.blink_medium);
            current_mode->request_state(Mode::States::IDLE); // stop the current mode
        }
        else if(logs.get_active_severity() == message_severities::critical){
            UserIO.set_led_state(0b1000, UserIO.blink_fast);
            current_mode->request_state(Mode::States::IDLE); // stop the current mode
            critical_shutdown();
        }
        else{
            UserIO.set_led_state(0b1000, UserIO.off); // turn off error LED
        }

        // run additional mode functions
        current_mode->default_run();
        current_mode->run();
        UserIO.run();

        // wait for switches to be read before setting the address and enabling communication
        if(!Comm.is_enabled() && UserIO.valid_switch_states()){
            Comm.set_device_address(UserIO.get_switch_states());
            Comm.enable(); // enable communication
        }

    }
}

void device::update(){

    // update temps
    Adc.get_heatsink_temp(&vars.heatsink_temp);
    Adc.get_board_temp(&vars.board_temp);
    Adc.get_air_in_temp(&vars.ambient_temp);

    {
    float temp = fmax(vars.board_temp, vars.heatsink_temp);  // use whichever temperature is higher
    if(vars.fan_auto_speed_enable){
        // set fan speed based on temperature
        // ramp between 0 and max speed based on temperature
        if(temp < vars.fan_zero_speed_temp){
            vars.fan_speed_cmd = 0;
        }
        else if(temp > vars.fan_max_speed_temp){
            vars.fan_speed_cmd = 0xffff;
        }
        else{
            // ramp between 0 and max speed based on temperature
            float temp_range = float(vars.fan_max_speed_temp - vars.fan_zero_speed_temp);
            float temp_offset = float(temp - vars.fan_zero_speed_temp);
            vars.fan_speed_cmd = uint16_t(0xffff * (temp_offset/temp_range));
        }
    }
    else{
        // set fan speed based on command only
        // no calculations needed
    }
    if(logs.get_active_severity() == message_severities::critical){
        vars.fan_speed_cmd = 0xffff; // set fan speed to max if in critical state
    }
    Fans.set_speed(Fans.max_rpm*vars.fan_speed_cmd/0xffff);
    vars.fan_speed_measured = uint16_t(Fans.get_fan_1_speed_rpm() + Fans.get_fan_2_speed_rpm())/2;
    }

    if(current_mode->get_error_on_comm_timeout() && !Comm.is_ok()){
        logs.add((uint32_t)communication_messages::timeout_error); // communication timeout error
    }

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


    if(Sto.output_allowed()){
        UserIO.set_led_state(0b0100, UserIO.on);
    }
    else{
        UserIO.set_led_state(0b0100, UserIO.off);
    }
}

void device::critical_shutdown(){
    // Disable power immediately
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

void device::TIM5_IRQHandler(void){
    TIM5->SR &= ~TIM_SR_UIF; // Clear the update interrupt flag
    tim5_flag = true;
}

void device::flagged_tim2(void){
    Comm.TIM2_IRQHandler();
    tim2_flag = false;
}

void device::flagged_tim5(void){
    Comm.TIM5_IRQHandler();
    tim5_flag = false;
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
        tim1_update_missed |= tim1_up_tim10_flag;
        tim1_up_tim10_flag = true;
    }
}

void device::flagged_tim1_up_tim10(void){
    current_mode->default_tim1_up_irq_handler();
    current_mode->tim1_up_irq_handler();

    Comm.update_timeout();
    watchdog_reload();
    tim1_up_tim10_flag = false;
    tim1_update_missed = false;
}

void device::USART6_IRQHandler(void){   // this handler is not flagged since it needs to be called immediately to ensure lowest jitter
    Comm.usart6_interrupt_handler();
}

device::IRQ device::missed_irq = IRQ::NONE;
void device::missed_irq_handler(IRQ irq){
    missed_irq = irq;
}
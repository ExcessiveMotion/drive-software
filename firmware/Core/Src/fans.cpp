#include "fans.h"

#define APB1_Timer_Clock_Frequency 100000000    // TODO: get this from some higher level configuration

fans::fans(logging* logs){
    this->logs = logs;
}

void fans::configure_GPIOB6_for_PWM(void){

    // Enable clock for GPIOB
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

    // Configure PB6 as alternate function (AF2 for TIM4_CH1)
    GPIOB->MODER &= ~(GPIO_MODER_MODER6);
    GPIOB->MODER |= (GPIO_MODER_MODER6_1);
    GPIOB->AFR[0] |= (2 << GPIO_AFRL_AFSEL6_Pos);
}

void fans::configure_TIM4_for_PWM(void){

    // Enable clock for TIM4
    RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;

    // Set prescaler to get 25kHz PWM frequency
    TIM4->PSC = (APB1_Timer_Clock_Frequency / 25000 / 1000) - 1;

    // Set auto-reload register for 1000 ticks (for 25kHz PWM)
    TIM4->ARR = 1000 - 1;

    // Set compare value to 0 (0% duty cycle initially)
    TIM4->CCR1 = 0;

    // Configure TIM4 channel 1 in PWM mode 1
    TIM4->CCMR1 &= ~(TIM_CCMR1_OC1M);
    TIM4->CCMR1 |= (TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2);
    TIM4->CCMR1 |= TIM_CCMR1_OC1PE;

    // Enable capture/compare channel 1
    TIM4->CCER |= TIM_CCER_CC1E;

    // Enable counter
    TIM4->CR1 |= TIM_CR1_CEN;
}


void fans::configure_GPIOC6_for_tachometer(void){

    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;   // Enable clock for GPIOC
    GPIOC->MODER &= ~(0x3 << (6 * 2));     // Clear MODER6[1:0]
    GPIOC->MODER |= (0x2 << (6 * 2));      // Set MODER6[1:0] to '10' (AF mode)
    GPIOC->PUPDR &= ~(0x3 << (6 * 2));     // No pull-up, no pull-down
    GPIOC->OSPEEDR &= ~(0x3 << (6 * 2));   // Clear speed bits
    GPIOC->OSPEEDR |= (0x1 << (6 * 2));    // Medium speed (optional)
    GPIOC->AFR[0] &= ~(0xF << (6 * 4));    // Clear AFRL6[3:0]
    GPIOC->AFR[0] |= (0x2 << (6 * 4));     // Set AFRL6[3:0] to AF2 (TIM3_CH1)
}
void fans::configure_TIM3_for_tachometer(void){

    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;    // Enable clock for TIM3
    TIM3->CR1 &= ~TIM_CR1_CEN;             // Disable TIM3
    TIM3->PSC = 0;                         // Set prescaler to 0
    TIM3->ARR = 0xFFFF;                    // Set auto-reload value to maximum
    TIM3->CR1 &= ~TIM_CR1_DIR;             // Up-counting mode

    TIM3->CCMR1 &= ~TIM_CCMR1_CC1S;        // Clear CC1S bits
    TIM3->CCMR1 |= TIM_CCMR1_CC1S_0;       // CC1S = '01' (TI1)
    TIM3->CR1 |= 0b10 << TIM_CR1_CKD_Pos;   // Set input clock division to 4
    TIM3->CCMR1 |= 0b1111 << TIM_CCMR1_IC1F_Pos;        // maximum filter

    TIM3->SMCR &= ~(TIM_SMCR_SMS | TIM_SMCR_TS); // Clear SMS and TS bits
    TIM3->SMCR |= TIM_SMCR_SMS_2 | TIM_SMCR_SMS_1 | TIM_SMCR_SMS_0; // SMS = '111'
    TIM3->SMCR |= TIM_SMCR_TS_2 | TIM_SMCR_TS_0; // TS = '101' (TI1FP1)

    TIM3->CR1 |= TIM_CR1_CEN;              // Enable TIM3 counter
}

void fans::configure_GPIOC7_for_tachometer(void){

    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;   // Enable clock for GPIOC
    GPIOC->MODER &= ~(0x3 << (7 * 2));     // Clear MODER7[1:0]
    GPIOC->MODER |= (0x2 << (7 * 2));      // Set MODER7[1:0] to '10' (AF mode)
    GPIOC->PUPDR &= ~(0x3 << (7 * 2));     // No pull-up, no pull-down
    GPIOC->OSPEEDR &= ~(0x3 << (7 * 2));   // Clear speed bits
    GPIOC->OSPEEDR |= (0x1 << (7 * 2));    // Medium speed (optional)
    GPIOC->AFR[0] &= ~(0xF << (7 * 4));    // Clear AFRL7[3:0]
    GPIOC->AFR[0] |= (0x3 << (7 * 4));     // Set AFRL7[3:0] to AF3 (TIM8_CH2)
}
void fans::configure_TIM8_for_tachometer(void){

    RCC->APB2ENR |= RCC_APB2ENR_TIM8EN;    // Enable clock for TIM8
    TIM8->CR1 &= ~TIM_CR1_CEN;             // Disable TIM8
    TIM8->PSC = 0;                         // Set prescaler to 0
    TIM8->ARR = 0xFFFF;                    // Set auto-reload value to maximum
    TIM8->CR1 &= ~TIM_CR1_DIR;             // Up-counting mode

    TIM8->CCMR1 &= ~TIM_CCMR1_CC2S;        // Clear CC2S bits
    TIM8->CCMR1 |= TIM_CCMR1_CC2S_0;       // CC2S = '01' (TI2)
    TIM8->CR1 |= 0b10 << TIM_CR1_CKD_Pos;   // Set input clock division to 4
    TIM8->CCMR1 |= 0b1111 << TIM_CCMR1_IC2F_Pos;        // maximum filter

    TIM8->SMCR &= ~(TIM_SMCR_SMS | TIM_SMCR_TS);       // Clear SMS and TS bits
    TIM8->SMCR |= TIM_SMCR_SMS_0 | TIM_SMCR_SMS_1 | TIM_SMCR_SMS_2; // SMS = '111'
    TIM8->SMCR |= TIM_SMCR_TS_1 | TIM_SMCR_TS_2;    // TS = '110' (TI2FP2)

    TIM8->CR1 |= TIM_CR1_CEN;              // Enable TIM8 counter
}


void fans::init()
{
    configure_GPIOB6_for_PWM();
    configure_TIM4_for_PWM();

    configure_GPIOC6_for_tachometer();
    configure_TIM3_for_tachometer();

    configure_GPIOC7_for_tachometer();
    configure_TIM8_for_tachometer();
}

uint32_t fans::set_speed(uint32_t speed_rpm)
{
    // Limit speed
    if (speed_rpm > MAX_FAN_SPEED_RPM) speed_rpm = MAX_FAN_SPEED_RPM;

    set_speed_rpm = speed_rpm;

    return 0;
}

uint32_t fans::get_fan_1_speed_rpm(void)
{
    return tachometer_1_rpm;
}

uint32_t fans::get_fan_2_speed_rpm(void)
{
    return tachometer_2_rpm;
}

void fans::SysTick_Handler()
{   
    if(update_cycle_count >= tach_sample_count)
    {
        // Set fan speed
        uint32_t duty_cycle = (set_speed_rpm * 1000) / MAX_FAN_SPEED_RPM;
        TIM4->CCR1 = duty_cycle;

        // Read tachometer 1 values
        tachometer_1_rpm = TIM3->CNT * SYSTICK_FREQUENCY * (60/2) / tach_sample_count;  // 2 pulses per revolution, convert to RPM
        TIM3->CNT = 0;

        // Read tachometer 2 values
        tachometer_2_rpm = TIM8->CNT * SYSTICK_FREQUENCY * (60/2) / tach_sample_count;  // 2 pulses per revolution, convert to RPM
        TIM8->CNT = 0;

        update_cycle_count = 0;
    }
    else{
        update_cycle_count++;
    }
}
\
#pragma once

#include "stm32f413xx.h"

#include "default_mode.h"
// other modes should be included here
#include "resistance_calib_mode.h"
#include "dc_voltage_mode.h"
#include "dc_current_mode.h"
#include "foc_current_mode.h"

#include "communication.h"
#include "device_descriptor.h"

//#define RELEASE_MODE    // enables watchdog and main phase PWM outputs


// main device class that contains all the other classes

class device {

    public:
        device();
        void init(); // initialize the device
        void run(); // main loop

    private:

        device_struct* comm_vars = nullptr;
        void** comm_var_pointers = nullptr;

        logging logs; // error/warning handling

        const uint64_t* micros = nullptr; // pointer to the microseconds variable
        void delay_us(uint32_t time_us); // blocking delay for a specified time in microseconds
        void delay_ms(uint32_t time_ms); // blocking delay for a specified time in milliseconds

        // create all low level classes
        communication Comm = communication(&logs); // communication with the controller
        fans Fans = fans(&logs); // fan control
        user_io UserIO = user_io(&logs); // user interface (DIP switches and LEDs)
        current_sense_interface CurrentSense = current_sense_interface(&logs); // current sensing
        phase_pwm PhasePWM = phase_pwm(&logs); // PWM generation
        adc_interface Adc = adc_interface(&logs); // ADC sampling
        sto Sto = sto(&logs); // safe torque off
        
        // create all modes
        

        //Mode Default_Mode = Mode(&logs, &Fans, &CurrentSense, &PhasePWM, &Sto, &UserIO, &Adc, comm_vars);
        //resistance_calib_mode Resistance_Calib = resistance_calib_mode(&logs, &Fans, &CurrentSense, &PhasePWM, &Sto, &UserIO, &Adc, comm_vars);
        //dc_voltage_mode DC_Voltage = dc_voltage_mode(&logs, &Fans, &CurrentSense, &PhasePWM, &Sto, &UserIO, &Adc, comm_vars);
        //dc_current_mode DC_Current = dc_current_mode(&logs, &Fans, &CurrentSense, &PhasePWM, &Sto, &UserIO, &Adc, comm_vars);
        foc_current_mode FOC_Current = foc_current_mode(&logs, &Fans, &CurrentSense, &PhasePWM, &Sto, &UserIO, &Adc, &comm_vars);
        
        //Mode PMSM_torque_control; // torque control mode for PMSM motor (TODO: implement this mode class)
        
        //Mode* current_mode = &Default_Mode; // pointer to the current mode
        //Mode* current_mode = &Resistance_Calib; // pointer to the current mode
        //Mode* current_mode = &DC_Voltage; // pointer to the current mode
        //Mode* current_mode = &DC_Current; // pointer to the current mode
        Mode* current_mode = &FOC_Current; // pointer to the current mode

        void update(); // low frequency update, called from SysTick_Handler

        void update_leds(); // update the LEDs

        void critical_shutdown(); // immediately disable system and enter a safe state (also exits the current mode)

        void enter_mode(Mode* mode); // enter a new mode

        void CPU_init(); // initialize the CPU

        void watchdog_init(); // initialize the watchdog timer
        void watchdog_reload(); // reload the watchdog timer, must be called above 1khz to prevent a reset

        void sysTick_init(); // initialize the system tick timer

        void startup_demo(); // run a startup demo to test all hardware



        // setup flags for each interrupt
        // it is best to use these flags to trigger events in the main loop,
        // rather than calling functions directly from the interrupt handlers if possible

        bool sysTick_flag = false; // flag set by the SysTick_Handler
        void flagged_sysTick(); // called when the sysTick_flag is set

        bool tim2_flag = false; // flag set by the TIM2_IRQHandler
        void flagged_tim2(); // called when the tim2_flag is set

        bool i2c1_ev_flag = false; // flag set by the I2C1_EV_IRQHandler
        void flagged_i2c1_ev(); // called when the i2c1_ev_flag is set

        bool i2c1_er_flag = false; // flag set by the I2C1_ER_IRQHandler
        void flagged_i2c1_er(); // called when the i2c1_er_flag is set

        bool dma2_stream0_flag = false; // flag set by the DMA2_Stream0_IRQHandler
        void flagged_dma2_stream0(); // called when the dma2_stream0_flag is set

        bool dma2_stream1_flag = false; // flag set by the DMA2_Stream1_IRQHandler
        void flagged_dma2_stream1(); // called when the dma2_stream1_flag is set

        bool tim1_up_tim10_flag = false; // flag set by the TIM1_UP_TIM10_IRQHandler
        void flagged_tim1_up_tim10(); // called when the tim1_up_tim10_flag is set
        

    public: // interrupt handlers, every possible interrupt should be defined here
        void _estack(void);
        static void Reset_Handler(void);
        void NMI_Handler(void);
        void HardFault_Handler(void);
        void MemManage_Handler(void);
        void BusFault_Handler(void);
        void UsageFault_Handler(void);
        void SVC_Handler(void);
        void DebugMon_Handler(void);
        void PendSV_Handler(void);
        void SysTick_Handler(void);
        void WWDG_IRQHandler(void);
        void PVD_IRQHandler(void);
        void TAMP_STAMP_IRQHandler(void);
        void RTC_WKUP_IRQHandler(void);
        void FLASH_IRQHandler(void);
        void RCC_IRQHandler(void);
        void EXTI0_IRQHandler(void);
        void EXTI1_IRQHandler(void);
        void EXTI2_IRQHandler(void);
        void EXTI3_IRQHandler(void);
        void EXTI4_IRQHandler(void);
        void DMA1_Stream0_IRQHandler(void);
        void DMA1_Stream1_IRQHandler(void);
        void DMA1_Stream2_IRQHandler(void);
        void DMA1_Stream3_IRQHandler(void);
        void DMA1_Stream4_IRQHandler(void);
        void DMA1_Stream5_IRQHandler(void);
        void DMA1_Stream6_IRQHandler(void);
        void ADC_IRQHandler(void);
        void CAN1_TX_IRQHandler(void);
        void CAN1_RX0_IRQHandler(void);
        void CAN1_RX1_IRQHandler(void);
        void CAN1_SCE_IRQHandler(void);
        void EXTI9_5_IRQHandler(void);
        void TIM1_BRK_TIM9_IRQHandler(void);
        void TIM1_UP_TIM10_IRQHandler(void);
        void TIM1_TRG_COM_TIM11_IRQHandler(void);
        void TIM1_CC_IRQHandler(void);
        void TIM2_IRQHandler(void);
        void TIM3_IRQHandler(void);
        void TIM4_IRQHandler(void);
        void I2C1_EV_IRQHandler(void);
        void I2C1_ER_IRQHandler(void);
        void I2C2_EV_IRQHandler(void);
        void I2C2_ER_IRQHandler(void);
        void SPI1_IRQHandler(void);
        void SPI2_IRQHandler(void);
        void USART1_IRQHandler(void);
        void USART2_IRQHandler(void);
        void USART3_IRQHandler(void);
        void EXTI15_10_IRQHandler(void);
        void RTC_Alarm_IRQHandler(void);
        void OTG_FS_WKUP_IRQHandler(void);
        void TIM8_BRK_TIM12_IRQHandler(void);
        void TIM8_UP_TIM13_IRQHandler(void);
        void TIM8_TRG_COM_TIM14_IRQHandler(void);
        void TIM8_CC_IRQHandler(void);
        void DMA1_Stream7_IRQHandler(void);
        void FSMC_IRQHandler(void);
        void SDIO_IRQHandler(void);
        void TIM5_IRQHandler(void);
        void SPI3_IRQHandler(void);
        void UART4_IRQHandler(void);
        void UART5_IRQHandler(void);
        void TIM6_DAC_IRQHandler(void);
        void TIM7_IRQHandler(void);
        void DMA2_Stream0_IRQHandler(void);
        void DMA2_Stream1_IRQHandler(void);
        void DMA2_Stream2_IRQHandler(void);
        void DMA2_Stream3_IRQHandler(void);
        void DMA2_Stream4_IRQHandler(void);
        void DFSDM1_FLT0_IRQHandler(void);
        void DFSDM1_FLT1_IRQHandler(void);
        void CAN2_TX_IRQHandler(void);
        void CAN2_RX0_IRQHandler(void);
        void CAN2_RX1_IRQHandler(void);
        void CAN2_SCE_IRQHandler(void);
        void OTG_FS_IRQHandler(void);
        void DMA2_Stream5_IRQHandler(void);
        void DMA2_Stream6_IRQHandler(void);
        void DMA2_Stream7_IRQHandler(void);
        void USART6_IRQHandler(void);
        void I2C3_EV_IRQHandler(void);
        void I2C3_ER_IRQHandler(void);
        void CAN3_TX_IRQHandler(void);
        void CAN3_RX0_IRQHandler(void);
        void CAN3_RX1_IRQHandler(void);
        void CAN3_SCE_IRQHandler(void);
        void RNG_IRQHandler(void);
        void FPU_IRQHandler(void);
        void UART7_IRQHandler(void);
        void UART8_IRQHandler(void);
        void SPI4_IRQHandler(void);
        void SPI5_IRQHandler(void);
        void SAI1_IRQHandler(void);
        void UART9_IRQHandler(void);
        void UART10_IRQHandler(void);
        void QUADSPI_IRQHandler(void);
        void FMPI2C1_EV_IRQHandler(void);
        void FMPI2C1_ER_IRQHandler(void);
        void LPTIM1_IRQHandler(void);
        void DFSDM2_FLT0_IRQHandler(void);
        void DFSDM2_FLT1_IRQHandler(void);
        void DFSDM2_FLT2_IRQHandler(void);
        void DFSDM2_FLT3_IRQHandler(void);

    private:
        enum class IRQ: int16_t{
            NONE = -1,
            MISSED_IRQ__estack = 0,
            MISSED_IRQ_Reset_Handler = 1,
            MISSED_IRQ_NMI_Handler = 2,
            MISSED_IRQ_HardFault_Handler = 3,
            MISSED_IRQ_MemManage_Handler = 4,
            MISSED_IRQ_BusFault_Handler = 5,
            MISSED_IRQ_UsageFault_Handler = 6,
            MISSED_IRQ_SVC_Handler = 7,
            MISSED_IRQ_DebugMon_Handler = 8,
            MISSED_IRQ_PendSV_Handler = 9,
            MISSED_IRQ_SysTick_Handler = 10,
            MISSED_IRQ_WWDG_IRQHandler = 11,
            MISSED_IRQ_PVD_IRQHandler = 12,
            MISSED_IRQ_TAMP_STAMP_IRQHandler = 13,
            MISSED_IRQ_RTC_WKUP_IRQHandler = 14,
            MISSED_IRQ_FLASH_IRQHandler = 15,
            MISSED_IRQ_RCC_IRQHandler = 16,
            MISSED_IRQ_EXTI0_IRQHandler = 17,
            MISSED_IRQ_EXTI1_IRQHandler = 18,
            MISSED_IRQ_EXTI2_IRQHandler = 19,
            MISSED_IRQ_EXTI3_IRQHandler = 20,
            MISSED_IRQ_EXTI4_IRQHandler = 21,
            MISSED_IRQ_DMA1_Stream0_IRQHandler = 22,
            MISSED_IRQ_DMA1_Stream1_IRQHandler = 23,
            MISSED_IRQ_DMA1_Stream2_IRQHandler = 24,
            MISSED_IRQ_DMA1_Stream3_IRQHandler = 25,
            MISSED_IRQ_DMA1_Stream4_IRQHandler = 26,
            MISSED_IRQ_DMA1_Stream5_IRQHandler = 27,
            MISSED_IRQ_DMA1_Stream6_IRQHandler = 28,
            MISSED_IRQ_ADC_IRQHandler = 29,
            MISSED_IRQ_CAN1_TX_IRQHandler = 30,
            MISSED_IRQ_CAN1_RX0_IRQHandler = 31,
            MISSED_IRQ_CAN1_RX1_IRQHandler = 32,
            MISSED_IRQ_CAN1_SCE_IRQHandler = 33,
            MISSED_IRQ_EXTI9_5_IRQHandler = 34,
            MISSED_IRQ_TIM1_BRK_TIM9_IRQHandler = 35,
            MISSED_IRQ_TIM1_UP_TIM10_IRQHandler = 36,
            MISSED_IRQ_TIM1_TRG_COM_TIM11_IRQHandler = 37,
            MISSED_IRQ_TIM1_CC_IRQHandler = 38,
            MISSED_IRQ_TIM2_IRQHandler = 39,
            MISSED_IRQ_TIM3_IRQHandler = 40,
            MISSED_IRQ_TIM4_IRQHandler = 41,
            MISSED_IRQ_I2C1_EV_IRQHandler = 42,
            MISSED_IRQ_I2C1_ER_IRQHandler = 43,
            MISSED_IRQ_I2C2_EV_IRQHandler = 44,
            MISSED_IRQ_I2C2_ER_IRQHandler = 45,
            MISSED_IRQ_SPI1_IRQHandler = 46,
            MISSED_IRQ_SPI2_IRQHandler = 47,
            MISSED_IRQ_USART1_IRQHandler = 48,
            MISSED_IRQ_USART2_IRQHandler = 49,
            MISSED_IRQ_USART3_IRQHandler = 50,
            MISSED_IRQ_EXTI15_10_IRQHandler = 51,
            MISSED_IRQ_RTC_Alarm_IRQHandler = 52,
            MISSED_IRQ_OTG_FS_WKUP_IRQHandler = 53,
            MISSED_IRQ_TIM8_BRK_TIM12_IRQHandler = 54,
            MISSED_IRQ_TIM8_UP_TIM13_IRQHandler = 55,
            MISSED_IRQ_TIM8_TRG_COM_TIM14_IRQHandler = 56,
            MISSED_IRQ_TIM8_CC_IRQHandler = 57,
            MISSED_IRQ_DMA1_Stream7_IRQHandler = 58,
            MISSED_IRQ_FSMC_IRQHandler = 59,
            MISSED_IRQ_SDIO_IRQHandler = 60,
            MISSED_IRQ_TIM5_IRQHandler = 61,
            MISSED_IRQ_SPI3_IRQHandler = 62,
            MISSED_IRQ_UART4_IRQHandler = 63,
            MISSED_IRQ_UART5_IRQHandler = 64,
            MISSED_IRQ_TIM6_DAC_IRQHandler = 65,
            MISSED_IRQ_TIM7_IRQHandler = 66,
            MISSED_IRQ_DMA2_Stream0_IRQHandler = 67,
            MISSED_IRQ_DMA2_Stream1_IRQHandler = 68,
            MISSED_IRQ_DMA2_Stream2_IRQHandler = 69,
            MISSED_IRQ_DMA2_Stream3_IRQHandler = 70,
            MISSED_IRQ_DMA2_Stream4_IRQHandler = 71,
            MISSED_IRQ_DFSDM1_FLT0_IRQHandler = 72,
            MISSED_IRQ_DFSDM1_FLT1_IRQHandler = 73,
            MISSED_IRQ_CAN2_TX_IRQHandler = 74,
            MISSED_IRQ_CAN2_RX0_IRQHandler = 75,
            MISSED_IRQ_CAN2_RX1_IRQHandler = 76,
            MISSED_IRQ_CAN2_SCE_IRQHandler = 77,
            MISSED_IRQ_OTG_FS_IRQHandler = 78,
            MISSED_IRQ_DMA2_Stream5_IRQHandler = 79,
            MISSED_IRQ_DMA2_Stream6_IRQHandler = 80,
            MISSED_IRQ_DMA2_Stream7_IRQHandler = 81,
            MISSED_IRQ_USART6_IRQHandler = 82,
            MISSED_IRQ_I2C3_EV_IRQHandler = 83,
            MISSED_IRQ_I2C3_ER_IRQHandler = 84,
            MISSED_IRQ_CAN3_TX_IRQHandler = 85,
            MISSED_IRQ_CAN3_RX0_IRQHandler = 86,
            MISSED_IRQ_CAN3_RX1_IRQHandler = 87,
            MISSED_IRQ_CAN3_SCE_IRQHandler = 88,
            MISSED_IRQ_RNG_IRQHandler = 89,
            MISSED_IRQ_FPU_IRQHandler = 90,
            MISSED_IRQ_UART7_IRQHandler = 91,
            MISSED_IRQ_UART8_IRQHandler = 92,
            MISSED_IRQ_SPI4_IRQHandler = 93,
            MISSED_IRQ_SPI5_IRQHandler = 94,
            MISSED_IRQ_SAI1_IRQHandler = 95,
            MISSED_IRQ_UART9_IRQHandler = 96,
            MISSED_IRQ_UART10_IRQHandler = 97,
            MISSED_IRQ_QUADSPI_IRQHandler = 98,
            MISSED_IRQ_FMPI2C1_EV_IRQHandler = 99,
            MISSED_IRQ_FMPI2C1_ER_IRQHandler = 100,
            MISSED_IRQ_LPTIM1_IRQHandler = 101,
            MISSED_IRQ_DFSDM2_FLT0_IRQHandler = 102,
            MISSED_IRQ_DFSDM2_FLT1_IRQHandler = 103,
            MISSED_IRQ_DFSDM2_FLT2_IRQHandler = 104,
            MISSED_IRQ_DFSDM2_FLT3_IRQHandler = 105

        };

        static IRQ missed_irq; // last missed IRQ
        static void missed_irq_handler(IRQ irq_enum); // handle missed IRQs
};


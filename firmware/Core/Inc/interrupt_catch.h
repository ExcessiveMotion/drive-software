/* interrupt_catch->h */
/* Catch all interrupts*/

extern "C" {

// void _estack(void){
//     Device->_estack();
// }

//void Reset_Handler(void){
    //Device->Reset_Handler();
//}

// void NMI_Handler(void){
//     Device->NMI_Handler();
// }

// void HardFault_Handler(void){
//     Device->HardFault_Handler();
// }

// void MemManage_Handler(void){
//     Device->MemManage_Handler();
// }

// void BusFault_Handler(void){
//     Device->BusFault_Handler();
// }

// void UsageFault_Handler(void){
//     Device->UsageFault_Handler();
// }

// void SVC_Handler(void){
//     Device->SVC_Handler();
// }

// void DebugMon_Handler(void){
//     Device->DebugMon_Handler();
// }

// void PendSV_Handler(void){
//     Device->PendSV_Handler();
// }

void SysTick_Handler(void){
    Device->SysTick_Handler();
}

void WWDG_IRQHandler(void){
    Device->WWDG_IRQHandler();
}

void PVD_IRQHandler(void){
    Device->PVD_IRQHandler();
}

void TAMP_STAMP_IRQHandler(void){
    Device->TAMP_STAMP_IRQHandler();
}

void RTC_WKUP_IRQHandler(void){
    Device->RTC_WKUP_IRQHandler();
}

void FLASH_IRQHandler(void){
    Device->FLASH_IRQHandler();
}

void RCC_IRQHandler(void){
    Device->RCC_IRQHandler();
}

void EXTI0_IRQHandler(void){
    Device->EXTI0_IRQHandler();
}

void EXTI1_IRQHandler(void){
    Device->EXTI1_IRQHandler();
}

void EXTI2_IRQHandler(void){
    Device->EXTI2_IRQHandler();
}

void EXTI3_IRQHandler(void){
    Device->EXTI3_IRQHandler();
}

void EXTI4_IRQHandler(void){
    Device->EXTI4_IRQHandler();
}

void DMA1_Stream0_IRQHandler(void){
    Device->DMA1_Stream0_IRQHandler();
}

void DMA1_Stream1_IRQHandler(void){
    Device->DMA1_Stream1_IRQHandler();
}

void DMA1_Stream2_IRQHandler(void){
    Device->DMA1_Stream2_IRQHandler();
}

void DMA1_Stream3_IRQHandler(void){
    Device->DMA1_Stream3_IRQHandler();
}

void DMA1_Stream4_IRQHandler(void){
    Device->DMA1_Stream4_IRQHandler();
}

void DMA1_Stream5_IRQHandler(void){
    Device->DMA1_Stream5_IRQHandler();
}

void DMA1_Stream6_IRQHandler(void){
    Device->DMA1_Stream6_IRQHandler();
}

void ADC_IRQHandler(void){
    Device->ADC_IRQHandler();
}

void CAN1_TX_IRQHandler(void){
    Device->CAN1_TX_IRQHandler();
}

void CAN1_RX0_IRQHandler(void){
    Device->CAN1_RX0_IRQHandler();
}

void CAN1_RX1_IRQHandler(void){
    Device->CAN1_RX1_IRQHandler();
}

void CAN1_SCE_IRQHandler(void){
    Device->CAN1_SCE_IRQHandler();
}

void EXTI9_5_IRQHandler(void){
    Device->EXTI9_5_IRQHandler();
}

void TIM1_BRK_TIM9_IRQHandler(void){
    Device->TIM1_BRK_TIM9_IRQHandler();
}

void TIM1_UP_TIM10_IRQHandler(void){
    Device->TIM1_UP_TIM10_IRQHandler();
}

void TIM1_TRG_COM_TIM11_IRQHandler(void){
    Device->TIM1_TRG_COM_TIM11_IRQHandler();
}

void TIM1_CC_IRQHandler(void){
    Device->TIM1_CC_IRQHandler();
}

void TIM2_IRQHandler(void){
    Device->TIM2_IRQHandler();
}

void TIM3_IRQHandler(void){
    Device->TIM3_IRQHandler();
}

void TIM4_IRQHandler(void){
    Device->TIM4_IRQHandler();
}

void I2C1_EV_IRQHandler(void){
    Device->I2C1_EV_IRQHandler();
}

void I2C1_ER_IRQHandler(void){
    Device->I2C1_ER_IRQHandler();
}

void I2C2_EV_IRQHandler(void){
    Device->I2C2_EV_IRQHandler();
}

void I2C2_ER_IRQHandler(void){
    Device->I2C2_ER_IRQHandler();
}

void SPI1_IRQHandler(void){
    Device->SPI1_IRQHandler();
}

void SPI2_IRQHandler(void){
    Device->SPI2_IRQHandler();
}

void USART1_IRQHandler(void){
    Device->USART1_IRQHandler();
}

void USART2_IRQHandler(void){
    Device->USART2_IRQHandler();
}

void USART3_IRQHandler(void){
    Device->USART3_IRQHandler();
}

void EXTI15_10_IRQHandler(void){
    Device->EXTI15_10_IRQHandler();
}

void RTC_Alarm_IRQHandler(void){
    Device->RTC_Alarm_IRQHandler();
}

void OTG_FS_WKUP_IRQHandler(void){
    Device->OTG_FS_WKUP_IRQHandler();
}

void TIM8_BRK_TIM12_IRQHandler(void){
    Device->TIM8_BRK_TIM12_IRQHandler();
}

void TIM8_UP_TIM13_IRQHandler(void){
    Device->TIM8_UP_TIM13_IRQHandler();
}

void TIM8_TRG_COM_TIM14_IRQHandler(void){
    Device->TIM8_TRG_COM_TIM14_IRQHandler();
}

void TIM8_CC_IRQHandler(void){
    Device->TIM8_CC_IRQHandler();
}

void DMA1_Stream7_IRQHandler(void){
    Device->DMA1_Stream7_IRQHandler();
}

void FSMC_IRQHandler(void){
    Device->FSMC_IRQHandler();
}

void SDIO_IRQHandler(void){
    Device->SDIO_IRQHandler();
}

void TIM5_IRQHandler(void){
    Device->TIM5_IRQHandler();
}

void SPI3_IRQHandler(void){
    Device->SPI3_IRQHandler();
}

void UART4_IRQHandler(void){
    Device->UART4_IRQHandler();
}

void UART5_IRQHandler(void){
    Device->UART5_IRQHandler();
}

void TIM6_DAC_IRQHandler(void){
    Device->TIM6_DAC_IRQHandler();
}

void TIM7_IRQHandler(void){
    Device->TIM7_IRQHandler();
}

void DMA2_Stream0_IRQHandler(void){
    Device->DMA2_Stream0_IRQHandler();
}

void DMA2_Stream1_IRQHandler(void){
    Device->DMA2_Stream1_IRQHandler();
}

void DMA2_Stream2_IRQHandler(void){
    Device->DMA2_Stream2_IRQHandler();
}

void DMA2_Stream3_IRQHandler(void){
    Device->DMA2_Stream3_IRQHandler();
}

void DMA2_Stream4_IRQHandler(void){
    Device->DMA2_Stream4_IRQHandler();
}

void DFSDM1_FLT0_IRQHandler(void){
    Device->DFSDM1_FLT0_IRQHandler();
}

void DFSDM1_FLT1_IRQHandler(void){
    Device->DFSDM1_FLT1_IRQHandler();
}

void CAN2_TX_IRQHandler(void){
    Device->CAN2_TX_IRQHandler();
}

void CAN2_RX0_IRQHandler(void){
    Device->CAN2_RX0_IRQHandler();
}

void CAN2_RX1_IRQHandler(void){
    Device->CAN2_RX1_IRQHandler();
}

void CAN2_SCE_IRQHandler(void){
    Device->CAN2_SCE_IRQHandler();
}

void OTG_FS_IRQHandler(void){
    Device->OTG_FS_IRQHandler();
}

void DMA2_Stream5_IRQHandler(void){
    Device->DMA2_Stream5_IRQHandler();
}

void DMA2_Stream6_IRQHandler(void){
    Device->DMA2_Stream6_IRQHandler();
}

void DMA2_Stream7_IRQHandler(void){
    Device->DMA2_Stream7_IRQHandler();
}

void USART6_IRQHandler(void){
    Device->USART6_IRQHandler();
}

void I2C3_EV_IRQHandler(void){
    Device->I2C3_EV_IRQHandler();
}

void I2C3_ER_IRQHandler(void){
    Device->I2C3_ER_IRQHandler();
}

void CAN3_TX_IRQHandler(void){
    Device->CAN3_TX_IRQHandler();
}

void CAN3_RX0_IRQHandler(void){
    Device->CAN3_RX0_IRQHandler();
}

void CAN3_RX1_IRQHandler(void){
    Device->CAN3_RX1_IRQHandler();
}

void CAN3_SCE_IRQHandler(void){
    Device->CAN3_SCE_IRQHandler();
}

void RNG_IRQHandler(void){
    Device->RNG_IRQHandler();
}

void FPU_IRQHandler(void){
    Device->FPU_IRQHandler();
}

void UART7_IRQHandler(void){
    Device->UART7_IRQHandler();
}

void UART8_IRQHandler(void){
    Device->UART8_IRQHandler();
}

void SPI4_IRQHandler(void){
    Device->SPI4_IRQHandler();
}

void SPI5_IRQHandler(void){
    Device->SPI5_IRQHandler();
}

void SAI1_IRQHandler(void){
    Device->SAI1_IRQHandler();
}

void UART9_IRQHandler(void){
    Device->UART9_IRQHandler();
}

void UART10_IRQHandler(void){
    Device->UART10_IRQHandler();
}

void QUADSPI_IRQHandler(void){
    Device->QUADSPI_IRQHandler();
}

void FMPI2C1_EV_IRQHandler(void){
    Device->FMPI2C1_EV_IRQHandler();
}

void FMPI2C1_ER_IRQHandler(void){
    Device->FMPI2C1_ER_IRQHandler();
}

void LPTIM1_IRQHandler(void){
    Device->LPTIM1_IRQHandler();
}

void DFSDM2_FLT0_IRQHandler(void){
    Device->DFSDM2_FLT0_IRQHandler();
}

void DFSDM2_FLT1_IRQHandler(void){
    Device->DFSDM2_FLT1_IRQHandler();
}

void DFSDM2_FLT2_IRQHandler(void){
    Device->DFSDM2_FLT2_IRQHandler();
}

void DFSDM2_FLT3_IRQHandler(void){
    Device->DFSDM2_FLT3_IRQHandler();
}

} // extern "C"
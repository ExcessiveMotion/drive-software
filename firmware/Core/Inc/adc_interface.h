/**
  ******************************************************************************
  * @file           : adc_interface.h
  * @brief          : Header for adc_interface.cpp file.
  *                   adc_interface files contain the requred functions to setup and use the ADC to read various analog voltages
  ******************************************************************************
**/

#pragma once

#include "stm32f413xx.h"
#include <assert.h>
#include "device_descriptor.h"
#include "logging.h"


// Class for managing ADC hardware
class adc_interface{
    private:

        logging* logs;

        message_severities fault = message_severities::none;
        union adc_data{
            uint32_t data_32[8];   // rx bytes are packed into this array by the DMA
            uint16_t data_16[16];  // same data as rx_data, but as bytes
        } raw_adc_data;

        uint32_t phase_U_millivolts = 0;
        uint32_t phase_V_millivolts = 0;
        uint32_t phase_W_millivolts = 0;

        uint32_t pfc_millivolts[3] = {0, 0, 0};
        uint8_t pfc_sense_index = 0; // index of the pfc sense voltage to use for the next sample

        uint32_t dc_bus_millivolts = 0;

        float board_temp = 0;       // deg C
        float mcu_temp = 0;
        float air_in_temp = 0;
        float heatsink_temp = 0;

        uint16_t mcu_temp_val_at_30C_cal = 0;
        uint16_t mcu_temp_val_at_110C_cal = 0;
        uint16_t mcu_temp_cnt_per_C = 0;
        
    public:
        adc_interface(logging* logs);

        void init(void);

        void start_sample(void);

        void convert_data(void);

        void dma_interrupt_handler(void);

        message_severities get_phase_U_millivolts(uint32_t* millivolts);
        message_severities get_phase_V_millivolts(uint32_t* millivolts);
        message_severities get_phase_W_millivolts(uint32_t* millivolts);

        message_severities get_pfc_U_millivolts(uint32_t* millivolts);
        message_severities get_pfc_V_millivolts(uint32_t* millivolts);
        message_severities get_pfc_W_millivolts(uint32_t* millivolts);

        message_severities get_dc_bus_millivolts(uint32_t* millivolts);
        
        message_severities get_board_temp(float* temp);
        message_severities get_mcu_temp(float* temp);
        message_severities get_heatsink_temp(float* temp);
        message_severities get_air_in_temp(float* temp);
};
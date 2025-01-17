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

// sense voltage dividers, integer values only
#define PHASE_SENSE_DIVIDER 1000000/5000
#define PFC_SENSE_DIVIDER 1000000/5000
#define DC_BUS_SENSE_DIVIDER 1000000/5000


// Class for managing ADC hardware
class adc_interface{
    private:

        logging* logs;

        message_severities fault = message_severities::none;

        uint32_t raw_adc_data[8];   // ADC data is packed from 16bit to 32bit and 16 samples are stored

        uint32_t phase_U_millivolts = 0;
        uint32_t phase_V_millivolts = 0;
        uint32_t phase_W_millivolts = 0;

        uint32_t pfc_U_millivolts = 0;
        uint32_t pfc_V_millivolts = 0;
        uint32_t pfc_W_millivolts = 0;

        uint32_t dc_bus_millivolts = 0;

        int32_t board_temp = 0;       // temp is in 0.001 deg C
        int32_t mcu_temp = 0;         // temp is in 0.001 deg C
        int32_t heatsink_1_temp = 0;  // temp is in 0.001 deg C
        int32_t heatsink_2_temp = 0;  // temp is in 0.001 deg C

        uint32_t gate_supply_millivolts = 0;    // not implemented in hardware
        
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
        
        message_severities get_board_temp(int32_t* temp);
        message_severities get_mcu_temp(int32_t* temp);
        message_severities get_heatsink_1_temp(int32_t* temp);
        message_severities get_heatsink_2_temp(int32_t* temp);

        message_severities get_gate_supply_millivolts(uint32_t* millivolts);  // not implemented on hardware
};
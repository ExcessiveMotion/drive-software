/**
  ******************************************************************************
  * @file           : communication.h
  * @brief          : Header for communication.cpp file.
  *                   communication files contain the requred functions to setup and use UART for communicating with the controller
  ******************************************************************************
**/

#pragma once

#include "stm32f413xx.h"
#include <assert.h>
#include <memory.h>
#include "device_descriptor.h"
#include "logging.h"

#define MAX_PACKET_SIZE 4+CYCLIC_ADDRESS_COUNT
// Class for managing uart hardware
class communication{
    private:

        logging* logs;

        //TODO: find out if the 32bit array is even needed by the DMA
        union rx_data{
          uint32_t data_words[MAX_PACKET_SIZE];   // rx bytes are packed into this array by the DMA
          uint8_t data_bytes[MAX_PACKET_SIZE*4];  // same data as rx_data, but as bytes
        };
        
        union tx_data{
          uint32_t data_words[MAX_PACKET_SIZE];   // tx bytes are unpacked from this array by the DMA
          uint8_t data_bytes[MAX_PACKET_SIZE*4];  // same data as tx_data, but as bytes
        };

        rx_data rx;
        tx_data tx;

        uint8_t expected_rx_length = 4;   // 4 x 32bit words is the smallest possible packet
        uint8_t expected_tx_length = 4;   // 4 x 32bit words is the smallest possible packet
        uint8_t device_address = 255;
        
        bool us_overflow = false;
        uint64_t microseconds = 0;
        bool timed_out = true;
        uint64_t last_valid_packet_time_us = 0;
        const uint32_t timeout_limit_us = 10 * 1e3; // time between valid packets before timeout
        void reset_timeout(void);

        // sequential data transfer
        uint32_t sequential_register_control = 0;
        uint32_t sequential_register_data_input = 0;
        uint32_t sequential_register_data_response = 0;

        // cyclic data transfer
        // it is critical cyclic config registers are only modified while cyclic mode is disabled
        bool cyclic_mode_enabled = false;
        uint16_t cyclic_read_count = 0;
        uint16_t cyclic_write_count = 0;
        uint16_t cyclic_read_addresses[CYCLIC_ADDRESS_COUNT];
        uint16_t cyclic_read_sizes[CYCLIC_ADDRESS_COUNT];
        uint16_t cyclic_write_addresses[CYCLIC_ADDRESS_COUNT];
        uint16_t cyclic_write_sizes[CYCLIC_ADDRESS_COUNT];

        void set_tx_packet_length(uint32_t length);
        void set_rx_packet_length(uint32_t length);

        uint32_t calculate_crc(uint32_t *data, uint8_t data_length);
        int8_t verify_rx_packet();
        void interpret_rx_sequential_data();
        void interpret_rx_cyclic_data();
        void calculate_rx_expected_size();
        void generate_tx_cyclic_data(); // prepares tx packet with device address and cyclic data
        void generate_tx_sequential_data(); // finalizes tx packet with sequential data and crc

        void enable_tx(void);
        void disable_tx(void);

        enum controller_register_access_result: uint8_t{
          SUCCESS,
          FAIL  // invalid address or permissions
        };

        //errors
        bool error_reading_cyclic_address = false;

        controller_register_access_result controller_get_register(uint16_t raw_address, void* raw_value);
        controller_register_access_result controller_set_register(uint16_t raw_address, void* raw_value);
        controller_register_access_result controller_set_register(uint16_t raw_address, void* raw_value, void* response);


        void timer_us_init(void);
        void sync_timer_us(void);
        void restart_rx_sync_capture(void);
        //void save_rx_sync_time(void); // this should be called right after a packet is received
        void resync_system(void); // restarts all timers to sync with the controller
        uint32_t rx_edge_time = 0;
        uint32_t rx_period = 0;
        uint32_t target_rx_period = 0;
        uint32_t allowed_period_error = 0;  // maximum syncronization error that will allow clock adjustment
        uint16_t pwm_timer_sync_offset_us = 0; // offset to sync pwm timer with controller

        void reset_communication(void); // resets cylic configs and disables cyclic mode
        


    public:
        // the DEVICE may read/write to ALL registers, regardless of their read/write setting in device_descriptor.h
        // the CONTROLLER however can only read/write to/from the register if the permission is set

        bool receive_complete = false;
        bool receive_started = false;

        communication(logging* logs);

        device_struct* comm_vars = nullptr;
        void** comm_var_pointers = nullptr;
        
        void init(void);

        void set_device_address(uint8_t address);
        void set_sync_frequency(uint16_t frequency_hz);
        void set_pwm_timer_sync_offset_us(uint16_t offset_us);
        bool enable_resync = false;  // resets all timers on the next broascast packet
        

        const uint64_t* micros = &microseconds;
        uint64_t get_microseconds(void);

        void update_timeout(void);

        bool is_ok(void);  // check if communication is working correctly (no timeout)

        void start_receive(void);
        bool rx_idle_detected(void);
        void clear_rx_idle_flag(void);
        void start_transmit(void);

        void restart_rx_dma(void);

        void dma_stream1_interrupt_handler(void);
        void usart6_interrupt_handler(void);

        void TIM2_IRQHandler(void);

};
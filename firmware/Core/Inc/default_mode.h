#include "phase_pwm.h"
#include "sto.h"
#include "logging.h"
#include "fans.h"
#include "current_sense_interface.h"
#include "user_io.h"
#include "adc_interface.h"
#include "device_descriptor.h"
#include <stdint.h>

#pragma once

// base class that all modes inherit
class Mode {
    public:

        Mode(logging* logs, fans* Fans, current_sense_interface* CurrentSense, phase_pwm* PhasePWM, sto* Sto, user_io* UserIO, adc_interface* Adc, device_struct** comm_vars);

        void default_tim1_up_irq_handler(void);
        virtual void tim1_up_irq_handler(void){}

        void default_systick_handler(void);
        virtual void systick_handler(void){}

        void default_run(void);
        virtual void run(void){}
        virtual uint32_t set_sub_mode(uint32_t sub_mode){
            return 0;
        } // set the sub mode for the current mode

        enum class States {
            NONE,
            IDLE,
            RUN
        };
        void request_state(States state);
        States get_state(void) { return current_state; }
        bool get_error_on_comm_timeout(void) { return error_on_comm_timeout; }

        void set_time_ptrs(const uint64_t* micros, const uint64_t* last_comm_time){
            this->micros = micros;
            this->last_comm_time = last_comm_time;
        }

    private:
        void safe_start_pwm(void);  // attempt to start the PWM outputs
        void safe_stop_pwm(void);   // stop the PWM outputs
        uint32_t current_sense_tries = 0;
        uint32_t high_side_ready_count = 0;
        uint32_t current_sense_delay_cnt = 0;
        uint32_t gate_supply_final_cnt = 0;

        const uint32_t max_current_sense_tries = 60;
        const uint32_t high_side_ready_cycles = 2; // number of cycles to wait for high side gate supplies to charge up (high fets are held high for this period)
        const uint32_t gate_supply_final_cycles = 150; // number of cycles to wait for final gate driver charge up (50% duty cycle on all phases for this period)
        const uint32_t current_sense_delay = 0; // cycles to leave the low side on after ADC startup to fully charge the ADCs and gate drivers
        
        void check_current_limits(void);


    protected:

        logging* logs;
        fans* Fans;
        current_sense_interface* CurrentSense;
        phase_pwm* PhasePWM;
        sto* Sto;
        user_io* UserIO;
        adc_interface* Adc;
        device_struct** comm_vars;

        const uint64_t* micros = nullptr; // pointer to the microseconds variable
        const uint64_t* last_comm_time = nullptr; // pointer to the last communication time variable

        bool error_on_comm_timeout = true;

        States current_state = States::IDLE;
        States requested_state = States::IDLE;

        enum class safe_start_steps {
            OFF,
            ENABLE_STO, // enable the STO channels
            VERIFY_STO, // verify the STO inputs are valid
            CHECK_ADC_VOLTAGES, // ensure voltage on all phases and DC bus is within limits
            PULSE_PWM, // briefly enable PWM outputs to power up ADCs and gate drivers
            WAIT_ADC_VALID, // wait for ADCs to power up show valid data
            WAIT_ADC_CHARGED, // wait for ADC supplies to charge up
            WAIT_HIGH_SIDE_READY, // wait for high side gate supplies to charge up
            WAIT_FINAL_GATE_CHARGE, // wait for final gate driver charge up
            DONE,
            FAULT,
        } safe_start_step = safe_start_steps::OFF;

        enum class safe_stop_steps {
            ON,
            DISABLE_PWM,
            DONE,
            FAULT,
        } safe_stop_step = safe_stop_steps::ON;

        void set_pwm_voltage(float U, float V, float W){
            uint32_t dc;
            Adc->get_dc_bus_millivolts(&dc);

            PhasePWM->set_raw(U, V, W);
        }
        

};
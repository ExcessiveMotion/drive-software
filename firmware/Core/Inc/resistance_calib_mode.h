#include "default_mode.h"

/**
    @brief: This mode is used to calibrate the resistance of the motor windings.
            It outputs a fixed current bewtween each of the phases, measures the required voltage to do so, then calculates the resistance.
**/

class resistance_calib_mode : public Mode{
    public:
        resistance_calib_mode(logging* logs, fans* Fans, current_sense_interface* CurrentSense, phase_pwm* PhasePWM, sto* Sto, user_io* UserIO, adc_interface* Adc, device_struct** comm_vars) : Mode(logs, Fans, CurrentSense, PhasePWM, Sto, UserIO, Adc, comm_vars){}

        void tim1_up_irq_handler(void) override;

        void systick_handler(void) override{
            // do nothing
        }

        //void run(void) override;

    /*
    1: startup PWM
    2: ensure measured currents are all zero (within some tolerance)
    3: control phase U and V, increase voltage until the desired current is reached, record the voltage (PWM duty cycle)
    4: flip the current direction and #3
    5: switch to phase V and W, repeat setps 3 4 5 until all combinations are done
    6: repeat process a number of times to get an average (and verify no significant changes)
    */

   private:
        const float voltage_ramp_rate = 5; // V/s, rate at which the voltage is increased to reach the test current, this should be low enough that inductance effects are minimal
        const float volts_per_cycle = voltage_ramp_rate/(PWMCLK*1000*2); // V, the voltage increment per PWM cycle
        const float max_voltage = 20; // V, the maximum voltage to apply to the motor


        #define SAMPLES 4
        const uint16_t sample_count = SAMPLES; // number of samples to average

        float test_current = 3.0; // A, the current to test the resistance at
        float inductance_low_current = test_current/10; // lower threshold for inductance testing

        const uint16_t low_current_tries = 3000; // number of cycles to wait for low current before faulting

        struct resistance_test_data{
            float U_V_p[SAMPLES];
            float U_V_n[SAMPLES];
            float U_W_p[SAMPLES];
            float U_W_n[SAMPLES];
            float V_W_p[SAMPLES];
            float V_W_n[SAMPLES];
        };
        resistance_test_data resistance_data;

        struct inductance_test_data{
            float U_V_p[SAMPLES];
            float U_V_n[SAMPLES];
            float U_W_p[SAMPLES];
            float U_W_n[SAMPLES];
            float V_W_p[SAMPLES];
            float V_W_n[SAMPLES];
        };
        inductance_test_data inductance_data;

        float filtered_dc_bus_voltage = 0;

        bool R_test_current_reached = false;
        bool L_test_current_reached = false;

        float high_current = 0.0;

        float U_voltage = 0;
        float V_voltage = 0;
        float W_voltage = 0;
        float total_voltage = 0;

        uint16_t cycles_counter = 0;    // counter for the number of measurement cycles run
        uint16_t counter = 0;   // multi-purpose counter
        enum class test_states{
            IDLE,

            // these are in 6-step commutation order
            TEST_V_W_P,
            TEST_U_V_N,
            TEST_U_W_N,
            TEST_V_W_N,
            TEST_U_V_P,
            TEST_U_W_P,
            
            DONE,
            FAULT
        } test_state = test_states::IDLE;
        test_states next_state = test_states::IDLE;
};
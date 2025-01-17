#include "resistance_calib_mode.h"
#include <math.h>

void resistance_calib_mode::tim1_up_irq_handler(void){
    int32_t avg_current;
    float U_delta = 0;
    float V_delta = 0;
    float W_delta = 0;

    float* R_data = nullptr;
    float* L_data = nullptr;

    //PhasePWM->disable();
    
    switch (test_state){
        case test_states::IDLE:{
            if(safe_start_step == safe_start_steps::DONE){  // PWM enabled
                counter = 0;
                test_state = test_states::TEST_V_W_P;
            }
            return;
            break;
        }

        case test_states::TEST_V_W_P:{
            avg_current = CurrentSense->phase_V_milliamps + -CurrentSense->phase_W_milliamps;
            avg_current /= 2;

            V_delta = volts_per_cycle/2;
            W_delta = -volts_per_cycle/2;
            total_voltage += volts_per_cycle;

            R_data = &resistance_data.V_W_p[cycles_counter];
            L_data = &inductance_data.V_W_p[cycles_counter];
            next_state = test_states::TEST_U_V_N;
            break;
        }

        case test_states::TEST_U_V_N:{
            avg_current = -CurrentSense->phase_U_milliamps + CurrentSense->phase_V_milliamps;
            avg_current /= 2;

            U_delta = -volts_per_cycle/2;
            V_delta = volts_per_cycle/2;
            total_voltage += volts_per_cycle;

            R_data = &resistance_data.U_V_n[cycles_counter];
            L_data = &inductance_data.U_V_n[cycles_counter];
            next_state = test_states::TEST_U_W_N;
            break;
        }

        case test_states::TEST_U_W_N:{
            avg_current = -CurrentSense->phase_U_milliamps + CurrentSense->phase_W_milliamps;
            avg_current /= 2;

            U_delta = -volts_per_cycle/2;
            W_delta = volts_per_cycle/2;
            total_voltage += volts_per_cycle;

            R_data = &resistance_data.U_W_n[cycles_counter];
            L_data = &inductance_data.U_W_n[cycles_counter];
            next_state = test_states::TEST_V_W_N;
            break;
        }

        case test_states::TEST_V_W_N:{
            avg_current = -CurrentSense->phase_V_milliamps + CurrentSense->phase_W_milliamps;
            avg_current /= 2;

            V_delta = -volts_per_cycle/2;
            W_delta = volts_per_cycle/2;
            total_voltage += volts_per_cycle;

            R_data = &resistance_data.V_W_n[cycles_counter];
            L_data = &inductance_data.V_W_n[cycles_counter];
            next_state = test_states::TEST_U_V_P;
            break;
        }

        case test_states::TEST_U_V_P:{
            avg_current = CurrentSense->phase_U_milliamps + -CurrentSense->phase_V_milliamps;
            avg_current /= 2;

            U_delta = volts_per_cycle/2;
            V_delta = -volts_per_cycle/2;
            total_voltage += volts_per_cycle;

            R_data = &resistance_data.U_V_p[cycles_counter];
            L_data = &inductance_data.U_V_p[cycles_counter];
            next_state = test_states::TEST_U_W_P;
            break;
        }

        case test_states::TEST_U_W_P:{
            avg_current = CurrentSense->phase_U_milliamps + -CurrentSense->phase_W_milliamps;
            avg_current /= 2;

            U_delta = volts_per_cycle/2;
            W_delta = -volts_per_cycle/2;
            total_voltage += volts_per_cycle;

            R_data = &resistance_data.U_W_p[cycles_counter];
            L_data = &inductance_data.U_W_p[cycles_counter];
            next_state = test_states::DONE;
            break;
        }

        case test_states::FAULT:{
            U_voltage = 0;
            V_voltage = 0;
            W_voltage = 0;
            R_test_current_reached = false;
            L_test_current_reached = false;
            requested_state = States::IDLE;
            PhasePWM->set_voltage(U_voltage, V_voltage, W_voltage, filtered_dc_bus_voltage);
            return;
            break;
        }

        case test_states::DONE:{
            U_voltage = 0;
            V_voltage = 0;
            W_voltage = 0;
            R_test_current_reached = false;
            L_test_current_reached = false;
            requested_state = States::IDLE;
            PhasePWM->set_voltage(U_voltage, V_voltage, W_voltage, filtered_dc_bus_voltage);
            return;
            // TODO: save data somewhere
            break;
        }
    }


    if(!R_test_current_reached){  // ramp up voltage to reach test current

        U_voltage += U_delta;
        V_voltage += V_delta;
        W_voltage += W_delta;

        if(total_voltage > max_voltage){
            test_state = test_states::FAULT;    // could not reach the test current with max voltage
            logs->add(RL_calib_mode_messages::resistance_calib_fail);
        }

        if(avg_current > test_current*1000.0){
            R_test_current_reached = true;
            counter = 0;

            // calculate resistance
            if(R_data != nullptr)
                *R_data = (total_voltage / avg_current) * 1000.0;

            high_current = avg_current/1000.0;

            U_voltage = 0;
            V_voltage = 0;
            W_voltage = 0;
            total_voltage = 0;
        }

        PhasePWM->set_voltage(U_voltage, V_voltage, W_voltage, filtered_dc_bus_voltage);

    }
    else{   // time how long it takes to reach the low current threshold

        if(counter > low_current_tries){
            test_state = test_states::FAULT;    // could not reach the low current threshold in time
            logs->add(RL_calib_mode_messages::inductance_calib_fail);

        }

        if(avg_current <= inductance_low_current*1000.0){
            if(L_data != nullptr){
                *L_data = -(*R_data * counter*(1.0 / (PWMCLK*1000.0*2.0)));
                *L_data /= log(float(avg_current/1000.0) / high_current);
            }

            R_test_current_reached = false;
            L_test_current_reached = true;
        }
        counter++;
    }

    if(L_test_current_reached){
        cycles_counter++;
        if(cycles_counter >= sample_count){
            test_state = next_state;
            cycles_counter = 0;
        }
        L_test_current_reached = false;
    }

uint32_t bus_millivolts;
Adc->get_dc_bus_millivolts(&bus_millivolts);
filtered_dc_bus_voltage = 0.9*filtered_dc_bus_voltage + (0.1/1000.0)*bus_millivolts;

}
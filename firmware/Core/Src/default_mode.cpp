#include "default_mode.h"

Mode::Mode(logging* logs, fans* Fans, current_sense_interface* CurrentSense, phase_pwm* PhasePWM, sto* Sto, user_io* UserIO, adc_interface* Adc, device_struct** comm_vars){
    this->logs = logs;
    this->Fans = Fans;
    this->CurrentSense = CurrentSense;
    this->PhasePWM = PhasePWM;
    this->Sto = Sto;
    this->UserIO = UserIO;
    this->Adc = Adc;
    this->comm_vars = comm_vars;
}


void Mode::safe_start_pwm(void){
    switch (safe_start_step){
        case safe_start_steps::ENABLE_STO:{
            if(Sto->enable() == message_severities::none){
                safe_start_step = safe_start_steps::VERIFY_STO;
            }
            else{
                safe_start_step = safe_start_steps::FAULT;
            }
            break;
        }

        case safe_start_steps::VERIFY_STO:{
            bool output_allowed = false;
            if(Sto->output_allowed(&output_allowed) == message_severities::none && output_allowed){
                safe_start_step = safe_start_steps::CHECK_ADC_VOLTAGES;
            }
            else{
                safe_start_step = safe_start_steps::FAULT;
            }
            break;
        }

        case safe_start_steps::CHECK_ADC_VOLTAGES:{
            uint32_t dc = 0;
            uint32_t phase_U = 0;
            uint32_t phase_V = 0;
            uint32_t phase_W = 0;

            if(Adc->get_dc_bus_millivolts(&dc) != message_severities::none){
                safe_start_step = safe_start_steps::FAULT;
            }
            else if(Adc->get_phase_U_millivolts(&phase_U) != message_severities::none){
                safe_start_step = safe_start_steps::FAULT;
            }
            else if(Adc->get_phase_V_millivolts(&phase_V) != message_severities::none){
                safe_start_step = safe_start_steps::FAULT;
            }
            else if(Adc->get_phase_W_millivolts(&phase_W) != message_severities::none){
                safe_start_step = safe_start_steps::FAULT;
            }
            else if(dc > MAX_DC_BUS_VOLTAGE*1000){
                logs->add(system_messages::overvoltage);
                safe_start_step = safe_start_steps::FAULT;
            }
            #define MAX_FLOATING_VOLTAGE_MILLIVOLT 18000    // handle voltage induced by gate drivers, probably should find a better way
            else if(phase_U > MAX_FLOATING_VOLTAGE_MILLIVOLT || phase_V > MAX_FLOATING_VOLTAGE_MILLIVOLT || phase_W > MAX_FLOATING_VOLTAGE_MILLIVOLT){
                logs->add(system_messages::floating_voltage_too_high);
                safe_start_step = safe_start_steps::FAULT;
            }
            else{
                safe_start_step = safe_start_steps::PULSE_PWM;
                CurrentSense->disable_short_circuit_detection();
                CurrentSense->clear_short_circuit_detected();
                PhasePWM->set_raw(0, 0, 0);
                PhasePWM->enable();
            }
            break;
        }

        case safe_start_steps::PULSE_PWM:{
            current_sense_tries = 0;

            safe_start_step = safe_start_steps::WAIT_ADC_VALID;
            break;
        }

        case safe_start_steps::WAIT_ADC_VALID:
            if(abs(CurrentSense->phase_U_milliamps) > 1000 || abs(CurrentSense->phase_V_milliamps) > 1000 || abs(CurrentSense->phase_W_milliamps) > 1000){
                current_sense_tries++;
            }
            else{
                //PhasePWM->enable();
                CurrentSense->enable_short_circuit_detection();
                high_side_ready_count = 0;
                safe_start_step = safe_start_steps::WAIT_HIGH_SIDE_READY;
                PhasePWM->set_raw(PWM_ticks, PWM_ticks, PWM_ticks); // hold high side on
                //safe_start_step = safe_start_steps::DONE;
            }
            if(current_sense_tries > max_current_sense_tries){
                safe_start_step = safe_start_steps::FAULT;
                PhasePWM->disable();
            }
            break;

        case safe_start_steps::WAIT_HIGH_SIDE_READY:
            high_side_ready_count++;
            if(high_side_ready_count > high_side_ready_cycles){
                safe_start_step = safe_start_steps::DONE;
                PhasePWM->set_percentange(0.0, 0.0, 0.0);
            }
            break;
    }

}

void Mode::safe_stop_pwm(void){
    switch (safe_stop_step){
        case safe_stop_steps::ON:{
            safe_stop_step = safe_stop_steps::DISABLE_PWM;
            break;
        }

        case safe_stop_steps::DISABLE_PWM:{
            PhasePWM->disable();
            safe_stop_step = safe_stop_steps::DONE;
            break;
        }
    }
}

void Mode::default_tim1_up_irq_handler(void){
    if(CurrentSense->get_currents()){
        // fault, current measurement not complete
        return;
    }

    Adc->convert_data();

    Adc->start_sample();    // TODO: make syncronized by hardware

    check_current_limits();

    switch (current_state){
        case States::IDLE:{
            if(requested_state == States::RUN){
                if(safe_start_step == safe_start_steps::OFF){
                    safe_start_step = safe_start_steps::ENABLE_STO;
                }
                if(safe_start_step == safe_start_steps::FAULT){
                    safe_start_step = safe_start_steps::OFF;
                    requested_state = States::IDLE;
                    // TODO: trigger a fault
                }
                safe_start_pwm();

                if(safe_start_step == safe_start_steps::DONE){
                    current_state = States::RUN;
                    safe_stop_step = safe_stop_steps::ON;
                }
            }
            else if(requested_state == States::IDLE){
                safe_stop_step = safe_stop_steps::ON;
                safe_stop_pwm();

                if(safe_stop_step == safe_stop_steps::DONE){
                    current_state = States::IDLE;
                }
            }
            break;
        }

        case States::RUN:{
            if(requested_state == States::IDLE){
                safe_stop_pwm();

                if(safe_stop_step == safe_stop_steps::DONE){
                    current_state = States::IDLE;
                }
            }
            break;
        }
    }

}

void Mode::default_systick_handler(void){
}

void Mode::default_run(void){
}

void Mode::request_state(States state){
    requested_state = state;
}

void Mode::check_current_limits(void){
    if(safe_start_step != safe_start_steps::DONE){  // skip current checks if PWM is not on
        return;
    }

    int32_t U_ma = CurrentSense->phase_U_milliamps;
    int32_t V_ma = CurrentSense->phase_V_milliamps;
    int32_t W_ma = CurrentSense->phase_W_milliamps;

    // fault if any phase current is over the limit
    if(abs(U_ma) > MAX_PHASE_CURRENT || abs(V_ma) > MAX_PHASE_CURRENT || abs(W_ma) > MAX_PHASE_CURRENT){
        logs->add(current_sense_messages::overcurrent);
        request_state(States::IDLE);
    }

    // fault if imbalance is too high
    if(abs(U_ma + V_ma + W_ma) > MAX_IMBALANCE_CURRENT){
        logs->add(current_sense_messages::imbalance);
        request_state(States::IDLE);
    }
}
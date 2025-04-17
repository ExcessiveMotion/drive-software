#include "foc_current_mode.h"
#include <math.h>
#include <limits>


void foc_current_mode::tim1_up_irq_handler(void){
    if(safe_start_step != safe_start_steps::DONE){
        // uint32_t bus_millivolts;
        // Adc->get_dc_bus_millivolts(&bus_millivolts);
        // filtered_dc_bus_voltage = bus_millivolts/1000.0;

        // set gains
        current_controller_q.Kp = (*comm_vars)->current_loop_p_gain;
        current_controller_q.Ki = (*comm_vars)->current_loop_i_gain;
        current_controller_q.I_term_limit = (*comm_vars)->current_loop_i_limit;
        current_controller_d.Kp = (*comm_vars)->current_loop_p_gain;
        current_controller_d.Ki = (*comm_vars)->current_loop_i_gain;
        current_controller_d.I_term_limit = (*comm_vars)->current_loop_i_limit;


        // clear any remaining integrator windup
        current_controller_q.reset();
        current_controller_d.reset();


        // set all sub-modes to idle
        resistance_cal.state = resistance_calibration::states::IDLE;
        current_loop_cal.state = current_loop_calibration::states::IDLE;
        commutation_cal.state = commutation_calibration::states::IDLE;

        // set all calibration mode values
        resistance_cal.calib_current = float((*comm_vars)->calibration_current) / 1000.0;
        current_loop_cal.calib_current = 0.1 * float((*comm_vars)->calibration_current) / 1000.0; // 10% of calibration current
        commutation_cal.calib_current = float((*comm_vars)->calibration_current) / 1000.0;

        return; // skip update if startup is not complete
    }

    float theta = 0.0;

    switch(calibration_mode){
        case calibration_modes::NONE:
            // normal operation
            current_cmd_q = float((*comm_vars)->current_command_q) / 1000.0; // amps
            current_cmd_d = float((*comm_vars)->current_command_d) / 1000.0; // amps

            theta = float((*comm_vars)->commutation_command * (*comm_vars)->commutation_scale) / (65535.0 / (2.0 * M_PI));

            break;
        case calibration_modes::RESISTANCE:
            if(resistance_cal.state == resistance_calibration::states::IDLE){
                resistance_cal.state = resistance_calibration::states::START;
            }
            resistance_cal.run();
            theta = resistance_cal.cmd_theta;
            if(resistance_cal.state == resistance_calibration::states::DONE){
                (*comm_vars)->phase_resistance = resistance_cal.measured_resistance;
                request_state(States::IDLE);
            }
            if(resistance_cal.state == resistance_calibration::states::FAIL){
                //logs->add();  // TODO: add a message for this
                request_state(States::IDLE);
            }
            break;
        case calibration_modes::BEMF:
            break;
        case calibration_modes::COMMUTATION:
            if(commutation_cal.state == commutation_calibration::states::IDLE){
                commutation_cal.state = commutation_calibration::states::START;
            }
            commutation_cal.run();
            theta = commutation_cal.cmd_theta;
            if(commutation_cal.state == commutation_calibration::states::DONE){
                request_state(States::IDLE);
            }
            if(commutation_cal.state == commutation_calibration::states::FAIL){
                //logs->add();  // TODO: add a message for this
                request_state(States::IDLE);
            }
            break;
        case calibration_modes::CURRENT_LOOP:
            if(current_loop_cal.state == current_loop_calibration::states::IDLE){
                current_loop_cal.state = current_loop_calibration::states::START;
            }
            current_loop_cal.run();
            if(current_loop_cal.state == current_loop_calibration::states::DONE){
                request_state(States::IDLE);
            }
            if(current_loop_cal.state == current_loop_calibration::states::FAIL){
                //logs->add();  // TODO: add a message for this
                request_state(States::IDLE);
            }
            break;
    }

    run_foc(theta);

    (*comm_vars)->current_measured_q = current_fbk_q * 1000.0; // milliamps
    (*comm_vars)->current_measured_d = current_fbk_d * 1000.0; // milliamps

}

void foc_current_mode::run_foc(float theta){
    float U_current = CurrentSense->phase_U_milliamps / 1000.0;
    float V_current = CurrentSense->phase_V_milliamps / 1000.0;
    float W_current = CurrentSense->phase_W_milliamps / 1000.0;

    // limit current cmds
    float q_cmd = fmin(float((*comm_vars)->current_limit_q) / 1000.0, current_cmd_q);
    float d_cmd = fmin(float((*comm_vars)->current_limit_d) / 1000.0, current_cmd_d);

    // limit to max possible current
    q_cmd = fmin(float(MAX_PHASE_CURRENT) / 1000.0, q_cmd);
    d_cmd = fmin(float(MAX_PHASE_CURRENT) / 1000.0, d_cmd);

    // theta = float((*comm_vars)->commutation_command) / (65535.0 / (2.0 * M_PI));
    // theta += (2.0*M_PI * (-1.0/12.0));    // offset to match test fanuc encoder phase angle

    theta = fmod(theta, 2.0 * M_PI);    // wrap theta to 0 -> 2*pi

    clarke_and_park_transform(theta, U_current, V_current, W_current, &current_fbk_d, &current_fbk_q);

    // PI controllers
    applied_voltage_q = current_controller_q.update(q_cmd - current_fbk_q);
    applied_voltage_d = current_controller_d.update(d_cmd - current_fbk_d);

    // resistance feed forward
    float resistance = (*comm_vars)->phase_resistance; // ohms
    resistance *= (1.0/sqrt(3));   // convert from phase resistance
    applied_voltage_q += q_cmd * resistance; 
    applied_voltage_d += d_cmd * resistance;

    float max_voltage = float((*comm_vars)->max_ouput_voltage);

    // limit voltages to max voltage
    applied_voltage_d = fmax(-max_voltage, fmin(+max_voltage, applied_voltage_d));
    applied_voltage_q = fmax(-max_voltage, fmin(+max_voltage, applied_voltage_q));

    // limit voltage change rate
    // float voltage_change_limit = (*comm_vars)->current_loop_voltage_change_limit; // V/s
    // float delta_q = applied_voltage_q - previous_applied_voltage_q;
    // float delta_d = applied_voltage_d - previous_applied_voltage_d;
    // float delta_t = 2.0 / float((PWMCLK*1000)); // seconds
    // float max_delta = voltage_change_limit * delta_t;

    // delta_q = fmax(-max_delta, fmin(max_delta, delta_q));
    // delta_d = fmax(-max_delta, fmin(max_delta, delta_d));
    // applied_voltage_q = previous_applied_voltage_q + delta_q;
    // applied_voltage_d = previous_applied_voltage_d + delta_d;


    float U_voltage, V_voltage, W_voltage;

    Inverse_Carke_and_Park_Transform(theta, applied_voltage_d, applied_voltage_q, &U_voltage, &V_voltage, &W_voltage);

    // apply 3rd harmonic injection
    // float V_max = fmaxf(U_voltage, fmaxf(V_voltage, W_voltage));
    // float V_min = fminf(U_voltage, fminf(V_voltage, W_voltage));
    // float offset = (V_max + V_min) / 2.0;
    // U_voltage -= offset;
    // V_voltage -= offset;
    // W_voltage -= offset;

    PhasePWM->set_voltage(U_voltage, V_voltage, W_voltage, filtered_dc_bus_voltage);
    //PhasePWM->set_voltage(0, 0, 0, filtered_dc_bus_voltage);

    uint32_t bus_millivolts;
    Adc->get_dc_bus_millivolts(&bus_millivolts);
    filtered_dc_bus_voltage = 0.9*filtered_dc_bus_voltage + (0.1/1000.0)*bus_millivolts;
}

uint32_t foc_current_mode::clarke_and_park_transform(float theta, float A, float B, float C, float *D, float *Q){
    // Amplitude invarient
    
    /*
    Converts 3 phase to DQ
    A, B, C are the 3 phase inputs
    D, Q are the DQ outputs
    theta is the electrical angle input of the motor (radians 0 -> 2*pi)
    */

    float X = (2.0 * A - B - C) * (1.0 / 3.0);
    float Y = (B - C) * (sqrt(3.0) / 3.0);
    
    float co = cos(theta);
    float si = sin(theta);
    
    *D = co*X + si*Y;
    *Q = co*Y - si*X;
    
    return 0;
}

uint32_t foc_current_mode::Inverse_Carke_and_Park_Transform(float theta, float D, float Q, float *A, float *B, float *C){
    // Amplitude invarient
	
    /*
    Converts DQ to 3 phase
    D, Q are the DQ inputs
    A, B, C are the 3 phase outputs
    theta is the electrical angle input of the motor (radians 0 -> 2*pi)
    */
        
    float co = cos(theta);
    float si = sin(theta);
    
    float X = co*D - si*Q;
    float Y = si*D + co*Q;
    
    *A = X;
    *B = -(1.0 / 2.0) * X;
    *C = *B - (sqrt(3.0) / 2.0) * Y;
    *B += (sqrt(3.0) / 2.0) * Y;
        
    return 0;
}

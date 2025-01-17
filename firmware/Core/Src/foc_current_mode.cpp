#include "foc_current_mode.h"
#include <math.h>
#include <limits>


void foc_current_mode::tim1_up_irq_handler(void){
    if(safe_start_step != safe_start_steps::DONE){
        uint32_t bus_millivolts;
        Adc->get_dc_bus_millivolts(&bus_millivolts);
        filtered_dc_bus_voltage = bus_millivolts/1000.0;
        return; // skip update if startup is not complete
    }

    float U_current = CurrentSense->phase_U_milliamps / 1000.0;
    float V_current = CurrentSense->phase_V_milliamps / 1000.0;
    float W_current = CurrentSense->phase_W_milliamps / 1000.0;

    float current_cmd = float((*comm_vars)->current_command_q) / 1000.0;    // get cmd current from controller vars

    #define I_MAX 4.0

    current_cmd = fmax(-I_MAX, fmin(+I_MAX, current_cmd));    // TODO: use soft limit from controller vars

    float d_current_fbk;
    float q_current_fbk;

    theta = float((*comm_vars)->commutation_command) / (65535.0 / (2.0 * M_PI));
    theta += (2.0*M_PI * (-1.0/12.0));    // offset to match test fanuc encoder phase angle

    //const float electrical_rads_per_second = 2*M_PI;
    //theta += electrical_rads_per_second / (PWMCLK*1000*2);

    theta = fmod(theta, 2.0 * M_PI);

    clarke_and_park_transform(theta, U_current, V_current, W_current, &d_current_fbk, &q_current_fbk);

    float q_voltage = current_controller_q.update(current_cmd - q_current_fbk);
    float d_voltage = current_controller_d.update(0 - d_current_fbk);   // keep d current at 0 for now

    // limit voltages to max voltage
    d_voltage = fmax(-max_voltage, fmin(+max_voltage, d_voltage));
    q_voltage = fmax(-max_voltage, fmin(+max_voltage, q_voltage));

    // limit voltages to what can be achieved with the current bus voltage and modulation ability
    // TODO: implement this

    float U_voltage, V_voltage, W_voltage;

    Inverse_Carke_and_Park_Transform(theta, d_voltage, q_voltage, &U_voltage, &V_voltage, &W_voltage);

    // apply 3rd harmonic injection
    // TODO: make these ramp up based on required voltage
    // U_voltage += sin(3.0 * theta) * (1/6) * (filtered_dc_bus_voltage/2);
    // V_voltage += sin(3.0 * theta) * (1/6) * (filtered_dc_bus_voltage/2);
    // W_voltage += sin(3.0 * theta) * (1/6) * (filtered_dc_bus_voltage/2);

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

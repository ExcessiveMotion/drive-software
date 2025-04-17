#include "pfc_mode.h"
#include <math.h>
#include <limits>

void pfc_mode::tim1_up_irq_handler(void){
    if(safe_start_step != safe_start_steps::DONE){

        uint32_t bus_millivolts;
        Adc->get_dc_bus_millivolts(&bus_millivolts);
        filtered_dc_bus_voltage = bus_millivolts*1000.0; // convert to volts

        // load settings
        pfc_1_vars.max_rms_current = (*comm_vars)->pfc_max_positive_current / 1000.0; // convert to amps
        pfc_1_vars.min_rms_current = (*comm_vars)->pfc_max_negative_current / 1000.0; // convert to amps
        regen_1_vars.max_regen_current = (*comm_vars)->regen_max_current / 1000.0; // convert to amps
        regen_1_vars.vbus_ramp_start = (*comm_vars)->regen_ramp_start;
        regen_1_vars.vbus_ramp_end = (*comm_vars)->regen_ramp_end;
        regen_1_vars.resistance = (*comm_vars)->regen_resistance;
        
        current_controller.Kp = (*comm_vars)->pfc_current_p_gain;
        current_controller.Ki = (*comm_vars)->pfc_current_i_gain;
        current_controller.I_term_limit = (*comm_vars)->pfc_current_i_limit;

        voltage_controller.Kp = (*comm_vars)->pfc_voltage_p_gain;
        voltage_controller.Ki = (*comm_vars)->pfc_voltage_i_gain;
        voltage_controller.I_term_limit = (*comm_vars)->pfc_voltage_i_limit;

        return; // skip update if startup is not complete
    }

    pfc_1_brake_run();
}

void pfc_mode::pfc_1_brake_run(){
    /*
    connections:
    U = mains 1
    V = mains 2 and brake resistor 1
    W = brake resistor 2
    */

    float mains_current = CurrentSense->phase_U_milliamps / 1000.0;
    //float brake_current = CurrentSense->phase_W_milliamps / 1000.0;

    desired_dc_bus_voltage = (*comm_vars)->pfc_target_voltage / 1000.0; // convert to volts

    float U_actual;
    float V_actual;
    float W_actual;
    
    PhasePWM->get_voltage(&U_actual, &V_actual, &W_actual, filtered_dc_bus_voltage);

    pfc_1_vars.pfc_sense_voltage = 0.9*pfc_1_vars.pfc_sense_voltage + 0.1*(U_actual - V_actual); // filter the voltage to remove noise

    //pfc_1_vars.pfc_sense_voltage = -20.0f;

    // handle brake resistor control

    // find desired brake current
    if(filtered_dc_bus_voltage >= regen_1_vars.vbus_ramp_end){
        regen_1_vars.regen_cmd_current = regen_1_vars.max_regen_current;
    }
    else if(filtered_dc_bus_voltage <= regen_1_vars.vbus_ramp_start){
        regen_1_vars.regen_cmd_current = 0.0;
    }
    else{
        float slope = (regen_1_vars.max_regen_current - 0.0) / (regen_1_vars.vbus_ramp_end - regen_1_vars.vbus_ramp_start);
        regen_1_vars.regen_cmd_current = slope * (filtered_dc_bus_voltage - regen_1_vars.vbus_ramp_start);
    }

    // calculate desired voltage
    float regen_power = regen_1_vars.regen_cmd_current * filtered_dc_bus_voltage;
    float resistor_voltage = sqrt(regen_power * regen_1_vars.resistance); // voltage across the resistor

    // TODO: add a check to ensure we never command a voltage that would exceed the max current rating of the resistor/drive


    // handle pfc control

    
    // decay the max/min voltage towards zero to filter out noise but still handle the slow 60hz input
    if(pfc_1_vars.pfc_sense_max_voltage > 0.0){
        pfc_1_vars.pfc_sense_max_voltage -= 0.0001 * pfc_1_vars.pfc_sense_max_voltage;
    }
    if(pfc_1_vars.pfc_sense_min_voltage < 0.0){
        pfc_1_vars.pfc_sense_min_voltage -= 0.0001 * pfc_1_vars.pfc_sense_min_voltage;
    }
    // capture peaks/troughs of the mains voltage
    if(pfc_1_vars.pfc_sense_voltage > pfc_1_vars.pfc_sense_max_voltage){
        pfc_1_vars.pfc_sense_max_voltage = .99*pfc_1_vars.pfc_sense_max_voltage + .01*pfc_1_vars.pfc_sense_voltage;
    }
    if(pfc_1_vars.pfc_sense_voltage < pfc_1_vars.pfc_sense_min_voltage){
        pfc_1_vars.pfc_sense_min_voltage = .99*pfc_1_vars.pfc_sense_min_voltage + .01*pfc_1_vars.pfc_sense_voltage;
    }

    constexpr float sqrt2_inv = 1.0f/1.41421356237f;
    bool is_ac = false;
    // calculate the rms voltage from the peaks
    if((pfc_1_vars.pfc_sense_min_voltage < -1.0f && pfc_1_vars.pfc_sense_max_voltage > 1.0f)){
        // AC voltage is present, calculate the rms voltage
        pfc_1_vars.pfc_sense_rms_voltage = (-pfc_1_vars.pfc_sense_min_voltage + pfc_1_vars.pfc_sense_max_voltage)/2.0f * sqrt2_inv;
    }
    else{
        // DC voltage is present, set the rms voltage to average
        pfc_1_vars.pfc_sense_rms_voltage = fmax(pfc_1_vars.pfc_sense_max_voltage, -pfc_1_vars.pfc_sense_min_voltage);   // using both means both polarities will work
    }

    // calculate the PFC current to regulate the bus voltage
    volatile float pfc_cmd_current = voltage_controller.update(filtered_dc_bus_voltage - desired_dc_bus_voltage);
    // positive current is out of the drive, negative current is into the drive
    pfc_cmd_current = fmax(pfc_cmd_current, -pfc_1_vars.max_rms_current);    // limit input current to max
    pfc_1_vars.cmd_rms_current = fmin(pfc_cmd_current, pfc_1_vars.min_rms_current);    // limit input current to min

    // calculate the actual commanded current
    pfc_1_vars.actual_cmd_current = pfc_1_vars.cmd_rms_current * (pfc_1_vars.pfc_sense_voltage / pfc_1_vars.pfc_sense_rms_voltage);

    // limit current cmd again to ensure we are not exceeding the max current rating of the drive
    pfc_1_vars.actual_cmd_current = fmax(pfc_1_vars.actual_cmd_current, -fabs(pfc_1_vars.cmd_rms_current) * 1.41421356237f);
    pfc_1_vars.actual_cmd_current = fmin(pfc_1_vars.actual_cmd_current, fabs(pfc_1_vars.cmd_rms_current) * 1.41421356237f);

    float pfc_cmd_voltage = current_controller.update(pfc_1_vars.actual_cmd_current - mains_current);
    pfc_cmd_voltage += pfc_1_vars.pfc_sense_voltage; // feed forward the mains voltage

    
    // calculate the commanded voltages
    float U_voltage = pfc_cmd_voltage/2.0f;
    float V_voltage = -pfc_cmd_voltage/2.0f;
    float W_voltage;

    // the brake resistor can only be guaranteed 50% duty cycle
    if(V_voltage > 0.0f){
        W_voltage = V_voltage - resistor_voltage;
    }
    else{
        W_voltage = V_voltage + resistor_voltage;
    }

    PhasePWM->set_voltage(U_voltage, V_voltage, W_voltage, filtered_dc_bus_voltage);

    uint32_t bus_millivolts;
    Adc->get_dc_bus_millivolts(&bus_millivolts);
    filtered_dc_bus_voltage = 0.9*filtered_dc_bus_voltage + (0.1/1000.0)*bus_millivolts;
}
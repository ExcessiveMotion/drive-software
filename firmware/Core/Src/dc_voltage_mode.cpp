#include "dc_voltage_mode.h"
#include <math.h>

void dc_voltage_mode::tim1_up_irq_handler(void){
    
if(safe_start_step == safe_start_steps::DONE){  // PWM enabled
    PhasePWM->set_voltage(U_voltage, V_voltage, W_voltage, filtered_dc_bus_voltage);
}
        
uint32_t bus_millivolts;
Adc->get_dc_bus_millivolts(&bus_millivolts);
filtered_dc_bus_voltage = 0.9*filtered_dc_bus_voltage + (0.1/1000.0)*bus_millivolts;

}
#include "default_mode.h"

/**
    @brief: This mode outputs a set DC voltage to the motor phases, no current control is implemented.
            usefull for running DC motors open loop
**/

class dc_voltage_mode : public Mode{
    public:
        dc_voltage_mode(logging* logs, fans* Fans, current_sense_interface* CurrentSense, phase_pwm* PhasePWM, sto* Sto, user_io* UserIO, adc_interface* Adc, device_struct** comm_vars) : Mode(logs, Fans, CurrentSense, PhasePWM, Sto, UserIO, Adc, comm_vars){}

        void tim1_up_irq_handler(void) override;

        void systick_handler(void) override{
            // do nothing
        }

        //void run(void) override;

   private:
        float filtered_dc_bus_voltage = 0;
        float U_voltage = 0;
        float V_voltage = 0;
        float W_voltage = 0;
        
};
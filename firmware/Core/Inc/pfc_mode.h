#include "default_mode.h"
#include <cmath>
#include <algorithm>
#include <stdint.h>


class pi_controller{
    public:
        pi_controller(){}

        float update(float error){
            float P_Term = Kp * error;
            I_Term += Ki * error;

            if (I_Term > I_term_limit){
                I_Term = I_term_limit;
            }
            else if (I_Term < -I_term_limit){
                I_Term = -I_term_limit;
            }

            return P_Term + I_Term;
        }

        float Kp = 0.0;
        float Ki = 0.0;
        float I_term_limit = 0.0;

        void reset(void){
            I_Term = 0.0;
        }

    private:
        float I_Term = 0.0;
};


/**
    @brief: This mode allows the drive to work in reverse to rectify AC mains to supply the dc bus
**/

class pfc_mode : public Mode{
    public:
        pfc_mode(logging* logs, fans* Fans, current_sense_interface* CurrentSense, phase_pwm* PhasePWM, sto* Sto, user_io* UserIO, adc_interface* Adc, device_struct** comm_vars) : Mode(logs, Fans, CurrentSense, PhasePWM, Sto, UserIO, Adc, comm_vars){}

        void tim1_up_irq_handler(void) override;

        void systick_handler(void) override{
            // do nothing
        }

        uint32_t set_sub_mode(uint32_t sub_mode) override{
            switch(sub_mode){
                case 0:
                    mode = modes::SINGLE_PHASE_PFC_AND_BRAKING;
                    break;
                default:
                    return 1; // invalid sub mode
                    break;
            }
            return 0; // valid sub mode
        }

   private:

        enum class modes{
            SINGLE_PHASE_PFC_AND_BRAKING
        } mode = modes::SINGLE_PHASE_PFC_AND_BRAKING;

        bool error_on_comm_timeout = false;

        float filtered_dc_bus_voltage = 0.0;

        float desired_dc_bus_voltage = 0.0; // desired dc bus voltage that the drive will try to maintain

        struct single_phase_pfc_vars{
            float cmd_rms_current = 0.0; // commanded rms current
            float pfc_sense_voltage = 0.0; // sensed voltage between U and V (before filter inductor)
            float pfc_sense_rms_voltage = 0.0; // sensed rms voltage
            float pfc_sense_max_voltage = 0.0;
            float pfc_sense_min_voltage = 0.0;
            float max_rms_current = 0.0; // max rms current (into the drive)
            float min_rms_current = 0.0; // min rms current (out of the drive)
            
            float actual_cmd_current = 0.0; // actual commanded current (will follow the pfc sense sine wave)
            float actual_fbk_current = 0.0; // actual feedback current (will follow the pfc sense sine wave)
        } pfc_1_vars;

        struct single_regen_brake_vars{
            float resistance = 15.0; // regen resistance
            float max_regen_current = 0.0; // max regen current
            float vbus_ramp_start = 0.0; // vbus ramp start voltage
            float vbus_ramp_end = 0.0; // vbus ramp end voltage

            float regen_cmd_current = 0.0; // commanded regen current
            float regen_fbk_current = 0.0; // feedback regen current
        } regen_1_vars;

        pi_controller current_controller = pi_controller();
        pi_controller voltage_controller = pi_controller();

        void pfc_1_brake_run(void);
};




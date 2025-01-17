#include "default_mode.h"
#include <cmath>
#include <algorithm>
#include <stdint.h>


class current_pi_controller{
    public:
        current_pi_controller(float Kp, float Ki, float I_term_limit){
            this->Kp = Kp;
            this->Ki = Ki;
            this->I_term_limit = I_term_limit;
        }

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

    private:
        float Kp;
        float Ki;
        float I_term_limit;
        float I_Term = 0.0;
};


/**
    @brief: This mode provides current control for 3 phase PMSM motors
**/

class foc_current_mode : public Mode{
    public:
        foc_current_mode(logging* logs, fans* Fans, current_sense_interface* CurrentSense, phase_pwm* PhasePWM, sto* Sto, user_io* UserIO, adc_interface* Adc, device_struct** comm_vars) : Mode(logs, Fans, CurrentSense, PhasePWM, Sto, UserIO, Adc, comm_vars){}

        void tim1_up_irq_handler(void) override;

        void systick_handler(void) override{
            // do nothing
        }

        //void run(void) override;

   private:

        float resistance = 1.4;
        float inductance = 0.014;

        float max_voltage = 20; // V, the maximum voltage to apply to the motor

        float filtered_dc_bus_voltage = 0;

        // current loop gains
        float p_gain = 30.0;
        float i_gain = 0.0;
        float i_limit = 20.0;
        float i_term = 0.0;

        float theta = 0.0;

        current_pi_controller current_controller_d = current_pi_controller(30.0, 0.0, 0.0);
        current_pi_controller current_controller_q = current_pi_controller(30.0, 0.0, 0.0);

        uint32_t clarke_and_park_transform(float theta, float A, float B, float C, float *D, float *Q);
        uint32_t Inverse_Carke_and_Park_Transform(float theta, float D, float Q, float *A, float *B, float *C);
};




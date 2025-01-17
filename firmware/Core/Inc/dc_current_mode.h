#include "default_mode.h"
#include <cmath>
#include <algorithm>



class MotorCurrentObserver
{

private:

    const float ts = 1.0/(PWMCLK*1000*2); // half the PWM period in seconds

    float motor_L = 0.01;       // Motor inductance (Henries)
    float motor_R = 0.1;       // Motor winding resistance (Ohms)
    float observer_k = 0.0;    // Observer correction gain
    float starting_current = 0.0;  // Estimated current at the start of each half-PWM cycle

    float dc_bus_voltage = 0.0; // DC bus voltage
    float bemf = 0.0;           // Motor back-emf

public:

    void setParams(float L, float R, float Kobs){
        motor_L = L;
        motor_R = R;
        observer_k = Kobs;
    }


    // Call this once per half-PWM cycle (e.g. at 50 kHz if PWM is 25 kHz center-aligned).
    //   current_fbk   : measured average current during the previous half-cycle
    //   dc_bus_voltage        : voltage applied during "on" portion
    //   bemf       : motor back emf at current speed
    //   pwm_val          : duty ratio [-1..1] for this half-cycle (0 is 50%)
    //      this assumes 100% applies full voltage to the phase for the entire half-cycle
    //      care must be taken to account for other phase pwm values affecting the true reesulting duty cycle

    void updateObserver(float current_fbk, float dc_bus_voltage, float bemf, float pwm_val){
        this->dc_bus_voltage = dc_bus_voltage;
        this->bemf = bemf;

        // Calculate actual applied voltage including back-emf and pwm duty cycle direction
        float Von = copysign(dc_bus_voltage, pwm_val);
        Von -= bemf;

        float Voff = -bemf; // off portion should only be back-emf

        float duty_cycle = fabs(pwm_val);

        // 1. Predict how the current evolves in the half-cycle, assuming linear ramps.
        
        // (a) On-interval slope approximation: slope_on = (Von - R*iStart_) / L
        // end current after "on" portion:
        float slopeOn = (Von - (motor_R * starting_current)) / motor_L;
        float iOnEnd = starting_current + slopeOn * (duty_cycle * ts);
        
        // (b) Off-interval slope approximation: slope_off = (Voff - R*iOnEnd) / L
        // end current after "off" portion:
        float slopeOff = (Voff - (motor_R * iOnEnd)) / motor_L;
        float iOffEnd  = iOnEnd + slopeOff * ((1.0 - duty_cycle) * ts);
        
        // 2. Compute the predicted average current:
        //    avgOn  = (iStart_ + iOnEnd)   / 2
        //    avgOff = (iOnEnd + iOffEnd)  / 2
        //    weighted by d and (1-d)
        float avgOn  = 0.5 * (starting_current + iOnEnd);
        float avgOff = 0.5 * (iOnEnd  + iOffEnd);
        
        float current_predicted = (duty_cycle * avgOn) + ((1.0 - duty_cycle) * avgOff);
        
        // 3. Compare to measured average and correct
        float error = current_fbk - current_predicted;
        
        // A simple proportional correction to the observer's starting current
        // so that next cycle's starting_current is adjusted.
        starting_current = iOffEnd + (observer_k * error);

    }
    
    // Return the observer's estimate of the current at the start of the half-cycle
    float getEstimatedCurrent() const{
        return starting_current;
    }

    // estimate the current at the end of the next half-cycle
    float estimate_future_current(float pwm_val){
        // Calculate actual applied voltage including back-emf and pwm duty cycle direction
        float Von = copysign(dc_bus_voltage, pwm_val);
        Von -= bemf;

        float Voff = -bemf; // off portion should only be back-emf

        float duty_cycle = fabs(pwm_val);

        // 1. Predict how the current evolves in the half-cycle, assuming linear ramps.
        
        // (a) On-interval slope approximation: slope_on = (Von - R*iStart_) / L
        // end current after "on" portion:
        float slopeOn = (Von - (motor_R * starting_current)) / motor_L;
        float iOnEnd  = starting_current + slopeOn * (duty_cycle * ts);
        
        // (b) Off-interval slope approximation: slope_off = (Voff - R*iOnEnd) / L
        // end current after "off" portion:
        float slopeOff = (Voff - (motor_R * iOnEnd)) / motor_L;
        float iOffEnd  = iOnEnd + slopeOff * ((1.0 - duty_cycle) * ts);

        return iOffEnd;
    }
    
};







/**
    @brief: This mode outputs set DC currents between phases UV and VW.
**/

class dc_current_mode : public Mode{
    public:
        dc_current_mode(logging* logs, fans* Fans, current_sense_interface* CurrentSense, phase_pwm* PhasePWM, sto* Sto, user_io* UserIO, adc_interface* Adc, device_struct** comm_vars) : Mode(logs, Fans, CurrentSense, PhasePWM, Sto, UserIO, Adc, comm_vars){}

        void tim1_up_irq_handler(void) override;

        void systick_handler(void) override{
            // do nothing
        }

        //void run(void) override;

   private:
        struct pair_data{
            float current_cmd = 0;
            float current_fbk_raw = 0;  // avg value from the current sensor
            float current_fbk_observed = 0; // instant value from the observer (at time of update)
            float resistance = 0;
            float inductance = 0;
            float BEMF_voltage = 0; // must be input from the controller (not calculated internally in this mode)
            float integrator_gain = 0;
            float integrator_value = 0;
            float integrator_limit = 0;
        };

        pair_data UV_data;
        pair_data VW_data;

        float max_voltage = 20; // V, the maximum voltage to apply to the motor

        float filtered_dc_bus_voltage = 0;
        float U_percent = 0.0;
        float V_percent = 0.0;
        float W_percent = 0.0;
        float U_percent_prev = 0.0;
        float V_percent_prev = 0.0;
        float W_percent_prev = 0.0;

        MotorCurrentObserver observer = MotorCurrentObserver();

        float p_gain = 30.0;
        float i_gain = 0.0;
        float i_limit = 20.0;
        float i_term = 0.0;


        
};




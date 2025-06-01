#include "default_mode.h"
#include <cmath>
#include <algorithm>
#include <stdint.h>


class current_pi_controller{
    public:
        current_pi_controller(){}

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
 * Discrete Kalman filter for 2-state [angle; angular rate] estimation.
 * - Prediction step runs every control-loop cycle (dt).
 * - Measurement update is called when a new encoder angle arrives.
 *
 * Angles are represented in [0, 2π).  Differences are wrapped to [-π, π).
 *
 * Tuning guidance for quantized encoders:
 * - For N counts per revolution, quantization step Δθ = 2π/N.
 * - A uniform-distribution quantization noise has variance ≈ Δθ²/12.
 *   Thus R_angle ≈ (2π/N)²/12.
 * - Process noise Q_rate should reflect expected acceleration variability.
 */
class KalmanFilterFOC {
    public:
        /**
         * @param dt         Control loop timestep (s), e.g., 1/50000
         * @param q_rate     Process noise variance for rate state (rate random-walk)
         * @param r_angle    Measurement noise variance for angle (encoder jitter)
         */
        KalmanFilterFOC(float dt, float q_rate, float r_angle)
            : dt(dt), Q_rate(q_rate), R_angle(r_angle)
        {
            // Initialize state estimates
            x_angle = 0.0f;  // [0, 2π)
            x_rate  = 0.0f;
            // Initialize covariance matrix P
            P00 = 1.0f; P01 = 0.0f;
            P10 = 0.0f; P11 = 1.0f;
        }

        /**
         * @brief re initialize the filter with new parameters\
         * @param dt         Control loop timestep (s), e.g., 1/50000
         * @param q_rate     Process noise variance for rate state (rate random-walk)
         * @param r_angle    Measurement noise variance for angle (encoder jitter)
         */
        void re_init(float dt, float q_rate, float r_angle){
            this->dt = dt;
            Q_rate = q_rate;
            R_angle = r_angle;

            // Initialize state estimates
            x_angle = 0.0f;  // [0, 2π)
            x_rate  = 0.0f;
            // Initialize covariance matrix P
            P00 = 1.0f; P01 = 0.0f;
            P10 = 0.0f; P11 = 1.0f;
        }
    
        /**
         * Utility: estimate measurement noise variance from encoder resolution.
         * @param counts    Number of discrete steps per full revolution.
         * @return R_angle  ≈ (2π/counts)²/12
         */
        static float estimateRAngleFromResolution(int counts) {
            float delta = 2.0f * M_PI / float(counts);
            return delta * delta / 12.0f;
        }
    
        /**
         * Update tuning: process (Q) and measurement (R) noise covariances.
         */
        void setTuning(float q_rate, float r_angle) {
            Q_rate  = q_rate;
            R_angle = r_angle;
        }
    
        /**
         * Measurement update at encoder rate (e.g., 1 kHz).
         * @param theta_meas  Measured angle in [0, 2π)
         */
        void encoderUpdate(float theta_meas) {
            float y = wrapDiff(theta_meas - x_angle);  // Innovation
            float S = P00 + R_angle;                   // Innovation covariance
            float K0 = P00 / S;
            float K1 = P10 / S;
            // State correction
            x_angle = wrap2pi(x_angle + K0 * y);
            x_rate  += K1 * y;
            // Covariance correction
            float P00_old = P00, P01_old = P01;
            P00 = (1.0f - K0) * P00_old;
            P01 = (1.0f - K0) * P01_old;
            P10 = P10 - K1 * P00_old;
            P11 = P11 - K1 * P01_old;
        }
    
        /**
         * Time update each control-loop tick (e.g., 50 kHz).
         * @return Estimated angle in [0, 2π)
         */
        float update() {
            // State prediction
            x_angle = wrap2pi(x_angle + x_rate * dt);
            // Covariance prediction
            float P00_old = P00, P01_old = P01;
            float P10_old = P10, P11_old = P11;
            P00 = P00_old + dt * (P10_old + P01_old) + P11_old * dt * dt;
            P01 = P01_old + P11_old * dt;
            P10 = P10_old + P11_old * dt;
            P11 = P11_old + Q_rate * dt;
            return x_angle;
        }
    
        /**
         * @return Current estimated angle in [0, 2π)
         */
        float getAngle() const { return x_angle; }
    
        /**
         * @return Current estimated angular rate (rad/s)
         */
        float getRate() const { return x_rate; }
    
    private:
        float dt;         // Control-loop timestep (s)
        float Q_rate;     // Process noise variance for rate
        float R_angle;    // Measurement noise variance for angle
    
        float x_angle;    // State: estimated angle [0,2π)
        float x_rate;     // State: estimated angular rate
    
        float P00, P01;   // Covariance matrix entries
        float P10, P11;
    
        /** Wrap angle into [0, 2π). */
        static float wrap2pi(float a) {
            float m = fmodf(a, 2.0f * M_PI);
            return (m < 0) ? m + 2.0f * M_PI : m;
        }
        /** Wrap difference into [-π, π). */
        static float wrapDiff(float d) {
            if (d >  M_PI) d -= 2.0f * M_PI;
            else if (d < -M_PI) d += 2.0f * M_PI;
            return d;
        }
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

        uint32_t set_sub_mode(uint32_t sub_mode) override{
            switch(sub_mode){
                case 0:
                    calibration_mode = calibration_modes::NONE;
                    break;
                case 1:
                    calibration_mode = calibration_modes::RESISTANCE;
                    break;
                case 2:
                    calibration_mode = calibration_modes::BEMF;
                    break;
                case 3:
                    calibration_mode = calibration_modes::COMMUTATION;
                    break;
                case 4:
                    calibration_mode = calibration_modes::CURRENT_LOOP;
                    break;
                default:
                    return 1; // invalid sub mode
                    break;
            }
            return 0; // valid sub mode
        }

        //void run(void) override;

   private:

        uint16_t debug[32];
        uint8_t debug_index = 0;

        enum class calibration_modes{
            NONE,
            RESISTANCE,
            BEMF,
            COMMUTATION,
            CURRENT_LOOP
        } calibration_mode = calibration_modes::NONE;

        uint64_t last_comm_update_time = 0; // last time the commutation was updated (internally)

        bool error_on_comm_timeout = true;

        float filtered_dc_bus_voltage = 0;

        float prev_theta = 0.0;

        // commanded current
        float current_cmd_q = 0.0;
        float current_cmd_d = 0.0;

        // fbk current
        float current_fbk_q = 0.0;
        float current_fbk_d = 0.0;

        float applied_voltage_q = 0.0;
        float applied_voltage_d = 0.0;

        float previous_applied_voltage_q = 0.0;
        float previous_applied_voltage_d = 0.0;

        current_pi_controller current_controller_d = current_pi_controller();
        current_pi_controller current_controller_q = current_pi_controller();

        KalmanFilterFOC kalman_filter = KalmanFilterFOC(1.0f/(PWMCLK*2000.0f), 100.0f, KalmanFilterFOC::estimateRAngleFromResolution(1024)); // dt = 50kHz, r_angle = according to encoder resolution

        void run_foc(float theta);

        uint32_t clarke_and_park_transform(float theta, float A, float B, float C, float *D, float *Q);
        uint32_t Inverse_Carke_and_Park_Transform(float theta, float D, float Q, float *A, float *B, float *C);


        class resistance_calibration{
            public:

                float calib_current = 0.0;

                enum class states{
                    IDLE,
                    START,
                    WAIT,
                    MEASURE,
                    DONE,
                    FAIL
                } state = states::IDLE;

                resistance_calibration(foc_current_mode* parent){
                    this->parent = parent;
                }

                float cmd_theta = 0.0;

                float measured_resistance = 0.0;

                void run(void){

                    switch(state){
                        case states::IDLE:
                            break;

                        case states::START:

                            reset_current_controllers();

                            parent->current_cmd_q = calib_current;
                            parent->current_cmd_d = 0.0;

                            // very light gains to prevent any overshoot or oscillation
                            parent->current_controller_q.Kp = 0.0;
                            parent->current_controller_q.Ki = 0.0001;
                            parent->current_controller_q.I_term_limit = 50.0;    // relying on all integral for this
                            parent->current_controller_d.Kp = 0.0;
                            parent->current_controller_d.Ki = 0.0001;
                            parent->current_controller_d.I_term_limit = 50.0;    // relying on all integral for this

                            count = 0;
                            cmd_theta = 0.0;
                            state = states::WAIT;
                            break;

                        case states::WAIT:
                            if(cmd_theta > 1 * M_PI){
                                // check if the current is within 5% of the commanded current
                                if (parent->current_fbk_q > 0.95 * calib_current && parent->current_fbk_q < 1.05 * calib_current){
                                    if(cmd_theta > 2.0 * M_PI){
                                        state = states::MEASURE;
                                        measured_resistance = parent->applied_voltage_q / parent->current_fbk_q; // starting point for resistance
                                        count = 0;
                                        break;
                                    }
                                }
                                else{
                                    parent->logs->add((uint32_t)foc_resistance_calib_messages::could_not_reach_current);
                                    state = states::FAIL;
                                    reset_current_controllers();
                                    break;
                                }
                            }
                            cmd_theta += 0.00005;  // slow rotation to ensure we arent balanced on a peak
                            break;

                        case states::MEASURE:
                            measured_resistance = (measured_resistance*.9) + (parent->applied_voltage_q / parent->current_fbk_q)*.1; // filter the resistance readings
                            if(count > 5000){ // wait for the filter to settle
                                measured_resistance *= sqrt(3); // convert to phase resistance
                                state = states::DONE;
                                reset_current_controllers();
                                break;
                            }
                            count++;
                            break;

                        case states::DONE:
                            break;

                        case states::FAIL:
                            break;
                    }
                    
                }

                void reset_current_controllers(void){
                    parent->current_cmd_q = 0.0;
                    parent->current_cmd_d = 0.0;
                    parent->current_controller_q.Kp = 0.0;
                    parent->current_controller_q.Ki = 0.0;
                    parent->current_controller_q.I_term_limit = 0.0;
                    parent->current_controller_d.Kp = 0.0;
                    parent->current_controller_d.Ki = 0.0;
                    parent->current_controller_d.I_term_limit = 0.0;
                    parent->current_controller_q.reset();
                    parent->current_controller_d.reset();
                }

            private:
                uint32_t count = 0;
                foc_current_mode* parent;

        } resistance_cal = resistance_calibration(this);

        class current_loop_calibration{

            public:
                float calib_current = 0.0;

                enum class states{
                    IDLE,
                    START,
                    RUN,
                    DONE,
                    FAIL
                } state = states::IDLE;

                current_loop_calibration(foc_current_mode* parent){
                    this->parent = parent;
                }

                void run(void){

                    switch(state){
                        case states::IDLE:
                            break;

                        case states::START:
                            parent->current_cmd_q = 0.0;
                            parent->current_cmd_d = 0.0;
                            parent->current_controller_q.Kp = 0.1;
                            // parent->current_controller_q.Ki = 0.0;
                            // parent->current_controller_q.I_term_limit = 0.0;
                            parent->current_controller_d.Kp = 0.1;
                            // parent->current_controller_d.Ki = 0.0;
                            // parent->current_controller_d.I_term_limit = 0.0;
                            parent->current_controller_q.reset();
                            parent->current_controller_d.reset();

                            state = states::RUN;
                            break;

                        case states::RUN:
                            // create the step current commands between calib_current and 0
                            if(count > (PWMCLK*2000) / step_frequency){

                                if(q_overshoot > .05 && q_overshoot >= calib_current * .05){ // 5% overshoot, we are done
                                    state = states::DONE;
                                    parent->current_cmd_q = 0.0;
                                    parent->current_cmd_d = 0.0;
                                    break;
                                }

                                if(parent->current_cmd_q == 0.0){
                                    parent->current_cmd_q = calib_current;
                                    parent->current_cmd_d = 0.0;
                                }
                                else{
                                    parent->current_cmd_q = 0.0;
                                    parent->current_cmd_d = 0.0;
                                }
                                parent->current_controller_q.Kp *= 1.05;    // increase the gains by 5%
                                parent->current_controller_d.Kp *= 1.05;
                                q_overshoot = 0.0;
                                count = 0;
                            }

                            count++;
                            

                            if(parent->current_cmd_q == 0.0){
                                q_overshoot = fmax(0.0, -parent->current_fbk_q);
                            }
                            else{
                                q_overshoot = fmax(0.0, parent->current_fbk_q - calib_current);
                            }

                            break;

                        case states::DONE:
                            break;

                        case states::FAIL:
                            break;

                    }
                }

            private:
                uint32_t count = 0;
                foc_current_mode* parent;
                const uint32_t step_frequency = 500; // (hz)
                float q_overshoot = 0.0;

        } current_loop_cal = current_loop_calibration(this);

        class commutation_calibration{

            public:
                enum class states{
                    IDLE,
                    START,
                    RUN_FWD,
                    RUN_REV,
                    DONE,
                    FAIL
                } state = states::IDLE;

                float cmd_theta = 0.0;
                float calib_current = 0.0;

                commutation_calibration(foc_current_mode* parent){
                    this->parent = parent;
                }

                void run(void){

                    uint32_t t = (*(parent->comm_vars))->commutation_command * (*(parent->comm_vars))->commutation_scale;
                    float fbk_theta = float(t) / (65535.0f / float(2.0 * M_PI));

                    int32_t comm_fbk = (*(parent->comm_vars))->commutation_command;
                    comm_fbk *= (*(parent->comm_vars))->commutation_scale;  // number of electrical rotations per commutation cycle
                    comm_fbk %= 65536; // wrap to 0 -> 65535
                    {
                        static int32_t last_comm_fbk = 0;

                        fbk_theta = (float)comm_fbk / (65535.0 / (2.0 * M_PI)); // single turn angle

                        // wrap to 0 -> 2*pi
                        fbk_theta = fmod(fbk_theta, 2.0 * M_PI);
                        
                        if(comm_fbk < last_comm_fbk - 32768){
                            multiturn_fbk_theta += 2.0 * M_PI;
                        }
                        else if(comm_fbk > last_comm_fbk + 32768){
                            multiturn_fbk_theta -= 2.0 * M_PI;
                        }
                        last_comm_fbk = comm_fbk;
                    }
                    fbk_theta += multiturn_fbk_theta; // add the multiturn angle


                    switch(state){
                        case states::IDLE:
                            break;

                        case states::START:
                            parent->current_cmd_q = calib_current;
                            parent->current_cmd_d = 0.0;
                            cmd_theta = 0.0;
                            first_point_reached = false;
                            second_point_reached = false;
                            fwd_offset = 0.0;
                            fwd_offset_starting = 0.0;
                            fwd_offset_ending = 0.0;
                            rev_offset = 0.0;
                            rev_offset_starting = 0.0;
                            rev_offset_ending = 0.0;
                            multiturn_fbk_theta = 0.0;
                            state = states::RUN_FWD;
                            break;

                        case states::RUN_FWD:

                            if(!first_point_reached && cmd_theta > 1.0 * M_PI){
                                fwd_offset = fbk_theta - cmd_theta;
                                fwd_offset_starting = fwd_offset;
                                first_point_reached = true;
                                fbk_theta_points[0] = fbk_theta;
                                comm_cmd_points[0] = comm_fbk;
                            }

                            if(first_point_reached){
                                fwd_offset = fwd_offset * 0.99 + (fbk_theta - cmd_theta) * 0.01;  // filter the offset reading
                            }

                            if(cmd_theta > 5.0 * M_PI){
                                fwd_offset_ending = fbk_theta - cmd_theta;
                                fbk_theta_points[1] = fbk_theta;
                                comm_cmd_points[1] = comm_fbk;
                                state = states::RUN_REV;
                            }
                            cmd_theta += 0.0003;  // slow rotation fwd
                            break;

                        case states::RUN_REV:

                            if(!second_point_reached && cmd_theta < 4.0 * M_PI){
                                rev_offset = fbk_theta - cmd_theta;
                                rev_offset_starting = rev_offset;
                                second_point_reached = true;
                                fbk_theta_points[2] = fbk_theta;
                                comm_cmd_points[2] = comm_fbk;
                            }

                            if(second_point_reached){
                                rev_offset = rev_offset * 0.99 + (fbk_theta - cmd_theta) * 0.01;  // filter the offset reading
                            }

                            if(cmd_theta < 0.0){
                                rev_offset_ending = fbk_theta - cmd_theta;
                                parent->current_cmd_q = 0.0;
                                fbk_theta_points[3] = fbk_theta;
                                comm_cmd_points[3] = comm_fbk;

                                // calculate the commutation offset
                                float offset_tolerance = 0.05 * (2.0 * M_PI); // difference in measured offset over 1 electrical rotation
                                if(fabs(fwd_offset_starting - fwd_offset_ending) > offset_tolerance || fabs(rev_offset_starting - rev_offset_ending) > offset_tolerance){
                                    parent->logs->add((uint32_t)foc_commutation_calib_messages::out_of_tolerance);
                                    state = states::FAIL;
                                    break;
                                }

                                float offset = (fwd_offset + rev_offset) / 2.0;
                                if(offset < 0.0){
                                    offset += 2.0 * M_PI; // make positive
                                }
                                offset = fmod(offset, 2.0 * M_PI); // wrap to 0 -> 2*pi
                                offset *= (65535.0 / (2.0 * M_PI)); // convert to commutation command
                                offset = fmax(0.0, fmin(65535.0, offset)); // limit to 0 -> 65535
                                (*(parent->comm_vars))->commutation_offset = uint16_t(offset);

                                state = states::DONE;
                            }
                            cmd_theta -= 0.0003;  // slow rotation rev
                            break;

                        case states::DONE:
                            break;

                        case states::FAIL:
                            break;

                    }
                }

            private:
                foc_current_mode* parent;
                bool first_point_reached = false;
                bool second_point_reached = false;
                float fwd_offset = 0.0;
                float fwd_offset_starting = 0.0;
                float fwd_offset_ending = 0.0;
                float rev_offset = 0.0;
                float rev_offset_starting = 0.0;
                float rev_offset_ending = 0.0;

                float multiturn_fbk_theta = 0.0;

                float fbk_theta_points[4] = {0.0, 0.0, 0.0, 0.0};
                uint16_t comm_cmd_points[4] = {0, 0, 0, 0};

        } commutation_cal = commutation_calibration(this);
};




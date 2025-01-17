#include "dc_current_mode.h"
#include <math.h>
#include <limits>



// Returns { integral_of_i, final_i }
static std::pair<float, float> integrateExponentialSegment(
    float iStart,   // starting current
    float voltage,  // constant applied voltage (includes sign and backEMF)
    float motorR,
    float motorL,
    float duration  // time length of this segment
)
{
    if (duration <= 1e-12) {
        // No time -> no change
        return { iStart * duration, iStart };
    }

    float alpha   = motorR / motorL;    // [1/s]
    float iInf    = voltage / motorR;   // steady-state current if voltage was held forever
    float diff    = (iInf - iStart);

    // final current after "duration"
    float iFinal  = iInf - diff * std::exp(-alpha * duration);

    // integral of i(t) from t=0..duration
    // i(t) = iInf - diff*exp(-alpha*t)
    // => ∫ i(t) dt = iInf*duration + diff*(1/alpha)*(1 - exp(-alpha*duration))
    float integral = iInf*duration + diff*(1.0/alpha)*(1.0 - std::exp(-alpha*duration));

    return { integral, iFinal };
}

// This struct is just for clarity / debugging
struct MotorParams
{
    float resistance;     // R
    float inductance;     // L
    float dcBusVoltage;   // Vbus
    float backEmf;        // bemf
    float halfCycleTime;  // T_s (e.g. 1/(2*25e3)=20us)
};

class AverageCurrentSolver
{
public:
    AverageCurrentSolver(float currentStart, float desiredAvg, const MotorParams& params)
    : iStart_(currentStart)
    , iDesiredAvg_(desiredAvg)
    , mp_(params)
    {}

    // Given a duty in [-1, +1], compute the actual average current
    float computeAverageCurrent(float duty)
    {
        // 1) Figure out the "on" portion length
        float tOn  = std::fabs(duty) * mp_.halfCycleTime;
        float tOff = mp_.halfCycleTime - tOn;

        // 2) Compute the on-voltage based on the sign of 'duty'
        float onVoltage = 0.0;
        if (duty > 1e-12) {
            onVoltage = ( mp_.dcBusVoltage ) - mp_.backEmf;  // positive drive
        } 
        else if (duty < -1e-12) {
            onVoltage = (-mp_.dcBusVoltage) - mp_.backEmf;   // negative drive
        } 
        else {
            onVoltage = 0.0 - mp_.backEmf;  // effectively no "on" portion
        }

        // 3) "On" segment
        auto seg1 = integrateExponentialSegment(iStart_, onVoltage,
                                                mp_.resistance, mp_.inductance, tOn);
        float intOn   = seg1.first;   // integral of current during on
        float iMid    = seg1.second;  // current at the end of on portion

        // 4) "Off" segment => voltage is (0 - bemf)
        float offVoltage = 0.0 - mp_.backEmf;
        auto seg2 = integrateExponentialSegment(iMid, offVoltage,
                                                mp_.resistance, mp_.inductance, tOff);
        float intOff  = seg2.first;   // integral of current during off
        // float iEnd    = seg2.second; // if you want the final current

        // 5) The total integral of current over [0..T_s], then divide by T_s
        float totalIntegral = intOn + intOff;
        float iAverage = totalIntegral / mp_.halfCycleTime;
        return iAverage;
    }

    // The function we want to drive to zero: f(duty) = iAvg(duty) - iDesiredAvg
    float errorFunction(float duty)
    {
        return computeAverageCurrent(duty) - iDesiredAvg_;
    }

private:
    float iStart_;       // current at start of the half-cycle
    float iDesiredAvg_;  // desired average current
    MotorParams mp_;
};



float computeDutyForDesiredAvgCurrent(
    float currentStart,    // iStart
    float desiredAvgCurrent,
    float motorR,
    float motorL,
    float dcBusVoltage,
    float backEmf,
    float halfCycleTime
)
{
    // 1) Set up our data & solver
    MotorParams mp;
    mp.resistance     = motorR;
    mp.inductance     = motorL;
    mp.dcBusVoltage   = dcBusVoltage;
    mp.backEmf        = backEmf;
    mp.halfCycleTime  = halfCycleTime;

    AverageCurrentSolver solver(currentStart, desiredAvgCurrent, mp);

    // We'll solve f(d) = solver.errorFunction(d) = 0 in the range [-1, +1].
    auto f = [&](float d){ return solver.errorFunction(d); };

    // 2) Evaluate at the boundaries
    float dLow  = -1.0;
    float dHigh = +1.0;
    float fLow  = f(dLow);
    float fHigh = f(dHigh);

    // If fLow and fHigh have the same sign, we might not have a root in [-1,1].
    // We'll still do a "best guess" approach:
    if ( (fLow * fHigh) > 0.0 ) {
        // No guaranteed crossing. Let's just keep them for secant but might converge near an endpoint.
    }

    // 3) Secant iterations
    // We'll do e.g. 5 or 6 steps. That should be plenty for a 20kHz or 50kHz loop if it's not too heavy.
    float dGuessOld = dLow;
    float dGuess    = dHigh;
    float fOld      = fLow;
    float fNew      = fHigh;

    for(int i=0; i<6; i++)
    {
        if(std::fabs(fNew) < 1e-12) {
            break;  // close enough to zero
        }

        // Secant step: dNext = dGuess - fNew * ( (dGuess - dGuessOld)/(fNew - fOld) )
        if(std::fabs(fNew - fOld) < 1e-14) {
            // fallback to bisection
            float dMid = 0.5*(dLow + dHigh);
            float fMid = f(dMid);
            // Decide which half to keep
            if(fMid == 0.0) {
                return dMid;
            } else if( (fLow * fMid) <= 0.0 ) {
                dHigh = dMid; 
                fHigh = fMid;
            } else {
                dLow  = dMid; 
                fLow  = fMid;
            }
            dGuessOld = dLow;
            dGuess    = dHigh;
            fOld      = fLow;
            fNew      = fHigh;
            continue;
        }

        float dNext = dGuess - fNew * ((dGuess - dGuessOld)/(fNew - fOld));

        // clamp to [-1, +1]
        dNext = fmax(-1.0, fmin(+1.0, dNext));

        // Evaluate
        float fNext = f(dNext);

        // Update
        dGuessOld = dGuess; 
        dGuess    = dNext;
        fOld      = fNew; 
        fNew      = fNext;

        // Also do a small bracket check
        if(dGuess < 0.0) {
            dLow = dGuess;  
            fLow = fNew;
        } else {
            dHigh= dGuess;  
            fHigh= fNew;
        }
    }

    // dGuess is our approximate solution
    return fmax(-1.0, fmin(+1.0, dGuess));
}


void dc_current_mode::tim1_up_irq_handler(void){
    if(safe_start_step != safe_start_steps::DONE){
        // TESTING
        UV_data.resistance = 1.4;
        UV_data.inductance = 0.014;

        observer.setParams(UV_data.inductance, UV_data.resistance, 0.5);
        return;
    }

    observer.setParams(UV_data.inductance, UV_data.resistance, 0.5);

    UV_data.current_fbk_raw = CurrentSense->phase_U_milliamps / 1000.0;
    VW_data.current_fbk_raw = -CurrentSense->phase_W_milliamps / 1000.0;

    observer.updateObserver(UV_data.current_fbk_raw, filtered_dc_bus_voltage, UV_data.BEMF_voltage, U_percent_prev/2.0);
    UV_data.current_fbk_observed = observer.getEstimatedCurrent();  // this is the estimate for current at the end of the previous PWM cycle

    //float current_error_UV = UV_data.current_fbk - UV_data.current_cmd;
    //float current_error_VW = VW_data.current_fbk - VW_data.current_cmd;


    /*
    Current fbk raw is from the n-1 1/2 pwm period
    we are currently in the n 1/2 period, so we can only adjust the voltage for the n+1 1/2 period

    */

    // estimate the current at the end of the current PWM cycle, so we can plan for the next cycle
    float estimated_future_current_UV = observer.estimate_future_current(U_percent/2.0);   

    // this mode uses phase V as a center point at 50% duty cycle, so the effective voltage is half of the bus voltage
    //float applied_voltage = filtered_dc_bus_voltage/2 - UV_data.BEMF_voltage; // what the true voltage applied to the motor is

    // float a = (applied_voltage / UV_data.resistance) - UV_data.current_cmd;
    // float b = (applied_voltage / UV_data.resistance) - UV_data.current_fbk_raw;

    // float total_time = -(UV_data.inductance / UV_data.resistance) * log(a/b);   // this is how long the phase output needs to be at the applied voltage to reach the desired current

    // total_time -= U_percent * (1/(PWMCLK*1000*2)); // subtract the time that the phase is on in the current cycle

    // float percent = total_time * PWMCLK*1000; // convert the time to a percentage of the full PWM period



    //float duty_cycle = computeDutyForDesiredAvgCurrent(estimated_future_current_UV, UV_data.current_cmd, UV_data.resistance, UV_data.inductance, filtered_dc_bus_voltage, 0.0, 1.0/(PWMCLK*1000*2));
    //duty_cycle *= 2.0; // double to account for the center point at 50% duty cycle


    float voltage = p_gain * (UV_data.current_cmd - UV_data.current_fbk_raw);
    i_term += i_gain * (UV_data.current_cmd - UV_data.current_fbk_raw);
    i_term = fmax(-i_limit, fmin(+i_limit, i_term));
    voltage += i_term;

    voltage = fmax(-max_voltage, fmin(+max_voltage, voltage));

    float duty_cycle = voltage / (filtered_dc_bus_voltage/2.0);

    duty_cycle = fmax(-PhasePWM->max_percent, fmin(+PhasePWM->max_percent, duty_cycle));

    U_percent_prev = U_percent;
    //V_percent_prev = V_percent;

    U_percent = duty_cycle;
    //V_percent = 0.0;

    uint32_t bus_millivolts;
    Adc->get_dc_bus_millivolts(&bus_millivolts);
    filtered_dc_bus_voltage = 0.9*filtered_dc_bus_voltage + (0.1/1000.0)*bus_millivolts;
    PhasePWM->set_percentange(U_percent, V_percent, W_percent);

}
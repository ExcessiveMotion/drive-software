#include "logging.h"
#include <memory.h>
#include <math.h>

void logging::init(){
    // reset times
    memset(last_active_error_times_micros, 0, sizeof(uint64_t)*identifiers_count);
    memset(last_ok_error_times_micros, 0, sizeof(uint64_t)*identifiers_count);
    memset(persistent_error_trigger_times_micros, 0, sizeof(uint32_t)*identifiers_count);
}

message_severities logging::get_active_severity(void){
    return active_severity;
}

uint64_t logging::get_last_reset_time_micros(void){
    return last_reset_time_micros;
}

uint64_t logging::get_last_active_error_time_micros(uint32_t id){
    return last_active_error_times_micros[uint32_t(id)];
}

void logging::clear_all(void){
    for (uint32_t i = 0; i < identifiers_count; i++){
        last_active_error_times_micros[i] = 0;  // reset message times
        last_ok_error_times_micros[i] = *microseconds;  // update ok times to now
    }

    active_severity = message_severities(0);   // reset active severity
    last_reset_time_micros = *microseconds;    // update last reset time
}

message_severities logging::add(uint32_t  id){
    last_active_error_times_micros[uint32_t(id)] = *microseconds;  // update error time
    message_severities new_severity = message_severities(values[id] >> 28);
    if(new_severity > active_severity){
        active_severity = new_severity;
    }
    
    return active_severity;
}

message_severities logging::log_persistent_active(uint32_t id){
    if(last_ok_error_times_micros[uint32_t(id)] + persistent_error_trigger_times_micros[uint32_t(id)] >= *microseconds){
        // the id has been active for long enough
        add(id);
    }
    return active_severity;
}

message_severities logging::log_persistent_inactive(uint32_t id){
    last_ok_error_times_micros[uint32_t(id)] = *microseconds;  // update ok time
    return active_severity;
}

void logging::set_persistent_error_trigger_time(uint32_t id, uint32_t time){
    persistent_error_trigger_times_micros[uint32_t(id)] = time;
}


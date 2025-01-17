/**
  ******************************************************************************
  * @file           : logging.h
  * @brief          : Header for logging.cpp file.
  *                   log files contain the requred functions to setup and use error handling
  ******************************************************************************
**/


#pragma once

#include "device_descriptor.h"
#include "stdint.h"


class logging {
public:

    void init();

    const uint64_t* microseconds = nullptr;   // pointer to the global microseconds variable
    

private:
    static const uint32_t identifiers_count = MESSAGE_COUNT;
    const uint32_t* values = message_values;

    // times are in global microseconds
    uint64_t last_active_error_times_micros[identifiers_count]; // last time the error was active   (TODO: possibly make this a 2d array to hold multiple times per id)
    uint64_t last_ok_error_times_micros[identifiers_count]; // last time the error was inactive
    uint32_t persistent_error_trigger_times_micros[identifiers_count]; // how long an id must be logged as persistent active before it's actually triggered

    message_severities active_severity = message_severities::none;   // the current active severity of the system
    uint64_t last_reset_time_micros = 0;    // the last time all logs were reset

public:
    message_severities get_active_severity(void); // get the current active severity of the system
    uint64_t get_last_reset_time_micros(void);  // get the last time all logs were reset
    uint64_t get_last_active_error_time_micros(uint32_t  id);  // get the last time the id was active
    void clear_all(void);   // resets all active logs
    message_severities add(uint32_t id);    // log the identifier as active
    message_severities log_persistent_active(uint32_t  id);  // signal that a persistent id is active
    message_severities log_persistent_inactive(uint32_t  id);    // signal that a persistent id is inactive
    void set_persistent_error_trigger_time(uint32_t  id, uint32_t time);    // set the time required for an id to be logged as persistent active before it's actually triggered
};

/**
  ******************************************************************************
  * @file           : sto.h
  * @brief          : Header for sto.cpp file.
  *                   sto files contain the requred functions to setup and use STO inputs/outputs
  ******************************************************************************
**/

#pragma once

#include "stm32f413xx.h"
#include <assert.h>
#include "logging.h"

// Class for managing STO hardware
class sto{
    private:
        logging* log;        

    public:
        sto(logging* log);

        void init(void);

        message_severities output_allowed(bool* result);

        message_severities enable(void);
        void disable(void);

        message_severities check_fault(void);

};
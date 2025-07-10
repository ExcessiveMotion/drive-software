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

        bool output_allowed(void);

        bool enable(void);
        void disable(void);

        bool check_fault(void);

};
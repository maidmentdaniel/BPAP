#ifndef LOG_FSM_H
#define LOG_FSM_H

#include "Arduino.h"
#include "pinout.h"
#include "interface.h"

#include <LiquidCrystal.h>

//ENUMERATIONS
enum log_enum
                {
                LOG_START,
                LOG_READOUT,
                LOG_STOP
                };

//PROTOTYPES
bool log_FSM(LiquidCrystal * lcdPtr);

#endif

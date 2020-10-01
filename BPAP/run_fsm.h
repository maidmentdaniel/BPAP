#ifndef RUN_FSM
#define RUN_FSM

#include "Arduino.h"
#include "pinout.h"
#include "interface.h"
#include "motor_ctrl.h"
#include <LiquidCrystal.h>
#include "log_fsm.h"

//ENUMERATIONS
enum RUN_enum { RUN_SETUP,
                WAIT_INHALE,
                SWEEP_IN,
                SWEEP_OUT,
                RUN_TO_SWITCH,
                DONE
            };

//PROTOTYPES
bool run_FSM( LiquidCrystal * lcdPtr);

#endif

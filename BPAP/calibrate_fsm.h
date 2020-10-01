#ifndef CALIBRATE_FSM_H
#define CALIBRATE_FSM_H

#include "Arduino.h"
#include "pinout.h"
#include "interface.h"
#include "motor_ctrl.h"
#include "log_fsm.h"

#include <LiquidCrystal.h>

//ENUMERATIONS
enum calib_enum { BEGIN_CALIBRATE,
                  TO_SWITCH,
                  SET_LIMIT_POSITION,
                  TO_BAG,
                  HOME_SETUP,
                  HOME,
                  HOME_IN,
                  HOME_OUT,
                  FINISH};

//PROTOTYPES
bool calibrate_FSM( LiquidCrystal * lcdPtr);

#endif

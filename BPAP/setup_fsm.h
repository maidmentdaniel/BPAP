#ifndef SETUP_FSM_H
#define SETUP_FSM_H

#include "Arduino.h"
#include "pinout.h"
#include "interface.h"
#include <LiquidCrystal.h>
#include "log_fsm.h"

//ENUMERATIONS
enum SETUP_enum {SET_DISPLAY, DISPLAY_VAL};

//PROTOTYPES
bool setup_FSM( LiquidCrystal * lcdPtr);
void readKnobs();

#endif

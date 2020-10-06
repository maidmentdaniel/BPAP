#ifndef LOG_FSM_H
#define LOG_FSM_H

#include "Arduino.h"
#include "pinout.h"
#include "interface.h"
#include "EEPROM.h"
#include "motor_ctrl.h"

#include <LiquidCrystal.h>

//ENUMERATIONS
enum log_enum
                {
                LOG_START,
                LOG_READOUT,
                LOG_STOP
                };// ignore

//PROTOTYPES
bool log_FSM(LiquidCrystal * lcdPtr);// ignore
bool writeEEPROM(int x = 0);// ignore

void writestate(int x = 0);
void writesubstate(int x = 0);
void writetoserial();

#endif

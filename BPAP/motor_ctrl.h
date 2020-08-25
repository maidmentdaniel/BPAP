#ifndef MOTOR_CTRL_H
#define MOTOR_CTRL_H

#include "pinout.h"
#include "arduino.h"
#include "interface.h"

void confMotor(float bpm = 20.0, float ie = 3.0);
void setMotor(float bpm = 20.0, float ie = 3.0);
void runMotor(float rotate);
void stopMotor();
bool checkMotorRunning();
int getSpeed();
float getMotorBPM();

const int getAngle();

// Pull down on 6
// PIN 6 == OC4A
// ==> TIMER 4, channel A

// TCCR4A – Timer/Counter 4 Control Register A
//  7       |6      |5      |4      |3      |2      |1      |0
//  COM4A1  |COM4A0 |COM4B1 |COM4B0 |COM4C1 |COM4C0 |WGM41  |WMW40

// Compare Output Mode, Fast PWM Table 17-4, page 155
// Clear OC4A on compare match, set OC4A at BOTTOM (non-inverting mode)

// TCCR4B – Timer/Counter 4 Control Register B
//  7       |6      |5      |4      |3      |2    |1    |0
//  ICNC4   |ICES4  |RESV   |WGM43  |WGM42  |CS42 |CS41 |CS40
// Fast PWM mode // Fast PWM | ICR4 | BOTTOM  | TOP

#endif

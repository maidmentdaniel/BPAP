#ifndef MOTOR_CTRL_H
#define MOTOR_CTRL_H

#include "pinout.h"
#include "arduino.h"
#include "interface.h"

void confMotor(float fStep);
void setMotor(float fStep);
void runMotor(float rotate);
void stopMotor();
bool checkMotorRunning();
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
//CS4 2:0: Clock Select
//CS4 2:0: 000      -->No clock source. (Timer/Counter stopped)
//CS4 2:0: 011      -->clk/64

//WGM4 3:0: Waveform Generation Mode
//WGM4 3:0: 1110    --> Timer/CounterMode of Operation:Fast PWM
//                  --> TOP:ICRnBOTTOM
//                  --> Update of OCRnx at:BOTTOM
//                  --> TOVn Flag Set on at:TOP


//OCIE4A: Timer/Countern, Output Compare A Match Interrupt Enable
//When this bit is written to one, and the I-flag in the Status Register is
//set (interrupts globally enabled), the Timer/Countern Output Compare A
// Match interrupt is enabled. The corresponding Interrupt Vector
//(see “Inter-rupts” on page 101) is executed when the OCFnA Flag,
//located in TIFRn, is set.
#endif

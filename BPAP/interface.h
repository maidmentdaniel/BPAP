#ifndef INTERFACE_H
#define INTERFACE_H

#include "Arduino.h"
#include "pinout.h"

long int getClockSpeed();
int getClockDiv();
float getpressureThreshold();
float getStepSize();

float getBPM();
float getVOL();
float getIE();
int getASIST();
int getALARM();
float getSwitchToBag();
float getBagToCentre();

void setBPM(float bpm);
void setVOL(float vol);
void setIE(float ie);
void setASIST(int assist);
void setALARM(int alarm);
void setSwitchToBag(float switch_to_bag);
void setBagToCentre(float bag_to_centre);
void addToSwitchToBag(float delta);

float calcStepRate(bool inhale = true, float sweep = ROM, bool debug = false);

int getSpeed();
float getMotorBPM();
float getMotorIE();
void motorDebug();
const int getSweep();

void debugInterface1();
void debugInterface2();

#endif

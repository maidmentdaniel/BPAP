#ifndef INTERFACE_H
#define INTERFACE_H

long int getClockSpeed();
int getClockDiv();
int getROM();
float getpressureThreshold();
float getStepSize();

int getBPM();
int getVOL();
int getIE();
int getASIST();
int getALARM();
float getSwitchToBag();

void setBPM(int bpm);
void setVOL(int vol);
void setIE(int ie);
void setASIST(int assist);
void setALARM(int alarm);
void setSwitchToBag(float switch_to_bag);
void addToSwitchToBag(float delta);

#endif

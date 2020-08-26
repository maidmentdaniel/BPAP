#ifndef INTERFACE_H
#define INTERFACE_H

long int getClockSpeed();
int getClockDiv();
int getROM();
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

#endif

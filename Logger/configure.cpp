#include "configure.h"
void configure_pins()
{
  //buttons
  pinMode(PIN_AlarmSilence, OUTPUT);   //Alarm button
  pinMode(PIN_SetButton, OUTPUT);      //Set button
  pinMode(PIN_HomeIn, OUTPUT);         // Homing towards centre input
  pinMode(PIN_HomeOut, OUTPUT);        // Homing towards outside input

  // //analog
  // pinMode(ReadBPM_Val, OUTPUT);       // init BPM
  // pinMode(ReadTV_Val, OUTPUT);        // init Tidal Volume
  // pinMode(ReadIE_Val, OUTPUT);        //Init I/E val
  // pinMode(ReadPEEP_alarm, OUTPUT);    //Init PEEP alarm
  // pinMode(ReadSensitivity, OUTPUT);   //Init Assist sensitivity
  // pinMode(PressureSensorPIN, OUTPUT); // Pressure sensor input
  // pinMode(PressureSensorPIN)
}

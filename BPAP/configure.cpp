#include "configure.h"
void configure_pins()
{
  pinMode(LED_Alarm, OUTPUT);      // Alarm LED
  pinMode(Buzzer_PIN, OUTPUT);     //buzzer
  analogWrite(Buzzer_PIN, 0);

  //switches
  pinMode(ToggleSwitch,INPUT);    // Toggle switch
  pinMode(LimitSwitchPIN,INPUT);  // Limit switch input
  //buttons
  pinMode(Home_In,INPUT);         // Homing towards centre input
  pinMode(Home_Out,INPUT);        // Homing towards outside input
  pinMode(SetButton, INPUT_PULLUP);      //Set button
  pinMode(Alarm_switch, INPUT);   //Alarm button

  //analog
  pinMode(ReadBPM_Val,INPUT);       // init BPM
  pinMode(ReadTV_Val,INPUT);        // init Tidal Volume
  pinMode(ReadIE_Val,INPUT);        //Init I/E val
  pinMode(ReadPEEP_alarm,INPUT);    //Init PEEP alarm
  pinMode(ReadSensitivity,INPUT);   //Init Assist sensitivity
  pinMode(PressureSensorPIN,INPUT); // Pressure sensor input

  pinMode(LimitSwitchPIN, INPUT);         //Alternative limit switch placement
}

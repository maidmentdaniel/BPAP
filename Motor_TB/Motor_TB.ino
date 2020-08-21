#include "pinout.h"
#include <AccelStepper.h>

AccelStepper stepper(1, 6, 7);
int spd = 1000;
int toggle = 1;
int pinRead = 0;
void setup()
{   
  Serial.begin(9600);
  pinMode(LED_Alarm, OUTPUT);      // Alarm LED
  pinMode(Alarm,INPUT);
  digitalWrite(Alarm, LOW);
  
  //switches
  pinMode(ToggleSwitch,INPUT);    // Toggle switch
  pinMode(LimitSwitchPIN,INPUT_PULLUP);     // Limit switch input
//  digitalWrite(LimitSwitchPIN, LOW);
  //buttons
  pinMode(SetButton, INPUT);  //Set button
  pinMode(Alarm_switch, INPUT);   //Alarm button
  pinMode(Home_In,INPUT);         // Homing towards centre input
  pinMode(Home_Out,INPUT);        // Homing towards outside input
  //analog
  pinMode(ReadBPM_Val,INPUT);     // init BPM
  pinMode(ReadTV_Val,INPUT);      // init Tidal Volume
  pinMode(ReadIE_Val,INPUT);      //Init I/E val
  pinMode(ReadPEEP_alarm,INPUT);  //Init PEEP alarm
  pinMode(ReadSensitivity,INPUT); //Init Assist sensitivity
  
  pinMode(PressureSensorPIN,INPUT);         // Pressure sensor input
  pinMode(laserPIN, INPUT);
  digitalWrite(LED_Alarm, HIGH);
  stepper.setMaxSpeed(1000);
  stepper.setSpeed(spd);
}
void loop()
{  
  stepper.runSpeed();
  pinRead = digitalRead(laserPIN);
  Serial.println(pinRead);
  if(!pinRead)
  {
    if(toggle==1){digitalWrite(LED_Alarm, HIGH);}
    else{digitalWrite(LED_Alarm, LOW);}
    toggle =  -1*toggle;
    spd = -1*spd;
    stepper.setMaxSpeed(spd);
    stepper.setSpeed(spd);
    delay(500);
  }
}

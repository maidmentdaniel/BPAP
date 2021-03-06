#ifndef PINOUT_H
#define PINOUT_H

//Buttons and switches - External 10kOhm Pulls low --> Switch High
#define Home_In 25		//  
#define Home_Out 27		//
#define ToggleSwitch 3	//
#define SetButton 5	//

  //Alarm
#define Alarm 23  //Alarm digital pin
#define LED_Alarm 29  //LED lights on alarm
#define Alarm_switch 4  //Pulls low --> Switch High

#define LimitSwitchPIN 2		// normally high switch to low. I think

//Pressure Sensor
#define PressureSensorPIN A4		//pressure analog read

//Dials - BPM 10K pot - (0, 5)V
#define ReadBPM_Val A0	// pin BPM
#define ReadTV_Val A1		// pin tidal volume
#define ReadIE_Val A2		// pin IE ratio
#define ReadPEEP_alarm A3	// pin PEEP Alarm sensitivity level
#define ReadSensitivity A5	// pin Asist control sensitivity level

//Laser Pin P10, 10k pullup --> Used for limit switch as limit switch appears to be broken
#define LimitSwitchPIN 31

//Stepper Motor PWM on pin 6: timer 4, channel a
#define drivePIN 6
// Stepper Motor direction control pin. cw is LOW, ccw is HIGH
#define dirPIN 7

#endif

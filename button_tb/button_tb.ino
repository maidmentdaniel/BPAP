#include <LiquidCrystal.h>

//Buttons and switches - External 10kOhm Pulls low --> Switch High
#define Home_In 25    //  
#define Home_Out 27   //
#define ToggleSwitch 3  //
#define SetButton 5 //

  //Alarm
#define Alarm 23  //Alarm digital pin
#define LED_Alarm 29  //LED lights on alarm
#define Alarm_switch 4  //Pulls low --> Switch High

#define LimitSwitchPIN 2    // normally high switch to low. I think

//Pressure Sensor
#define PressureSensorPIN A4    //pressure analog read

//Dials - BPM 10K pot - (0, 5)V
#define ReadBPM_Val A0  // pin BPM
#define ReadTV_Val A1   // pin tidal volume
#define ReadIE_Val A2   // pin IE ratio
#define ReadPEEP_alarm A3 // pin PEEP Alarm sensitivity level
#define ReadSensitivity A5  // pin Asist control sensitivity level

uint8_t rs = 13, en = 12, d4 = 11, d5 = 10, d6 = 9, d7 = 8;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int val1 = 0;
int val2 = 0;
int val3 = 0;
int val4 = 0;
int val5 = 0;
int val6 = 0;
int i = 0;

void setup()
{
  pinMode(LED_Alarm, OUTPUT);      // Alarm LED
  pinMode(Alarm,INPUT);
  digitalWrite(Alarm, LOW);
  
  //switches
  pinMode(ToggleSwitch,INPUT);    // Toggle switch
  pinMode(LimitSwitchPIN,INPUT);     // Limit switch input
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

  lcd.begin(20, 4);
  lcd.clear();
}

void loop()
{
  //row `
  if(digitalRead(SetButton))
  {
    digitalWrite(LED_Alarm, HIGH);
  }
  else
  {
    digitalWrite(LED_Alarm, LOW);
  }
  lcd.setCursor(0,0);
  lcd.print("SET");
  lcd.setCursor(6,0);
  lcd.print(digitalRead(SetButton));
  
  lcd.setCursor(9,0);
  lcd.print("ALARM");
  lcd.setCursor(15,0);
  lcd.print(digitalRead(Alarm_switch));
  //row 2
  lcd.setCursor(0,1);
  lcd.print("OUT");
  lcd.setCursor(6,1);
  lcd.print(digitalRead(Home_Out));
  
  lcd.setCursor(9,1);
  lcd.print("IN");
  lcd.setCursor(15,1);
  lcd.print(digitalRead(Home_In));
  //row 3
  lcd.setCursor(0,2);
  lcd.print("TOG");
  lcd.setCursor(6,2);
  lcd.print(digitalRead(ToggleSwitch));
  
  lcd.setCursor(9,2);
  lcd.print("LIMIT");
  lcd.setCursor(15,2);
  lcd.print(digitalRead(LimitSwitchPIN));
  delay(100);
  lcd.clear();
}

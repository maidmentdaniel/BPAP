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
  pinMode(PressureSensorPIN,INPUT);         // Pressure sensor input
  pinMode(ToggleSwitch,INPUT);    // Toggle switch
  pinMode(LimitSwitchPIN,INPUT);     // Limit switch input
  pinMode(Home_In,INPUT);         // Homing towards centre input
  pinMode(Home_Out,INPUT);        // Homing towards outside input
    
  pinMode(ReadBPM_Val,INPUT);     // init BPM
  pinMode(ReadTV_Val,INPUT);      // init Tidal Volume
  pinMode(ReadIE_Val,INPUT);      //Init I/E val
  pinMode(ReadPEEP_alarm,INPUT);  //Init PEEP alarm
  pinMode(ReadSensitivity,INPUT); //Init Assist sensitivity 
  
  pinMode(SetButton, INPUT);  //Set button
  pinMode(Alarm_switch, INPUT);   //Alarm button

  lcd.begin(20, 4);
  lcd.clear();
}

void loop()
{
  val1 = map(analogRead(ReadBPM_Val), 0, 1023, 0, 5);
  val2 = map(analogRead(ReadTV_Val), 0, 1023, 0, 5);
  val3 = map(analogRead(ReadIE_Val), 0, 1023, 0, 5);
  val4 = map(analogRead(ReadPEEP_alarm), 0, 1023, 0, 5);
  val5 = map(analogRead(ReadSensitivity), 0, 1023, 0, 5);
  val6 = map(analogRead(PressureSensorPIN), 0, 1023, 0, 5);

  
  lcd.setCursor(0,0);
  lcd.print("Pressure");
  lcd.setCursor(10,0);
  lcd.print(val6);
  
  lcd.setCursor(0,1);
  lcd.print("BPM");
  lcd.setCursor(6,1);
  lcd.print(val1);

  lcd.setCursor(0,2);
  lcd.print("IE");
  lcd.setCursor(4,2);
  lcd.print(val3);

  lcd.setCursor(8,2);
  lcd.print("TV");
  lcd.setCursor(13,2);
  lcd.print(val2);

  lcd.setCursor(0,3);
  lcd.print("Assist");
  lcd.setCursor(7,3);
  lcd.print(val5);

  lcd.setCursor(11,3);
  lcd.print("PEEP");
  lcd.setCursor(18,3);
  lcd.print(val4);

  delay(100);
  lcd.clear();
}

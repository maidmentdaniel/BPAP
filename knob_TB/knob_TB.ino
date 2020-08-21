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

enum SETUP_enum {SET_DISPLAY, DISPLAY_VAL};

SETUP_enum SETUP_STATE = SET_DISPLAY;

int bpm = 20;
int vol = 100;
int ie = 5;
int assist = -5;
int alarm = 20;

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
  SETUP_STATE = SET_DISPLAY;
}

void loop()
{
  bpm = map(analogRead(ReadBPM_Val), 0, 1023, 40, -1);
  vol = map(analogRead(ReadTV_Val), 0, 1023, 99, -1);
  ie = map(analogRead(ReadIE_Val), 0, 1023, 5, 0);
  assist = map(analogRead(ReadSensitivity), 0, 1023, 20, -1);
  alarm = map(analogRead(ReadPEEP_alarm), 0, 1023, 40, -1);
  switch(SETUP_STATE)
  {
    case SET_DISPLAY:
    {
      //line 1
      lcd.print("BPM   |VOL   |iE 1: ");
      lcd.setCursor(4,0);
      lcd.print(bpm);
      lcd.setCursor(11,0);
      lcd.print(vol);
      lcd.setCursor(19,0);
      lcd.print(ie);
      //line 2
      lcd.setCursor(0,1);
      lcd.print("ASSIST   cmH2O      ");
      lcd.setCursor(7,1);
      lcd.print(assist);
      //line3
      lcd.setCursor(0,2);
      lcd.print("ALARM   cmH2O       ");
      lcd.setCursor(6,2);
      lcd.print(alarm);
      //line 3
      lcd.setCursor(0,3);
      lcd.print("  SET to continue  ");
      SETUP_STATE = DISPLAY_VAL;
      break;
    }
    case DISPLAY_VAL:
    {
      lcd.leftToRight();
      //line 1
      lcd.setCursor(4,0);
      lcd.print("  ");
      lcd.setCursor(4,0);
      lcd.print(bpm);
      lcd.setCursor(11,0);
      lcd.print("  ");
      lcd.setCursor(11,0);
      lcd.print(vol);
      lcd.setCursor(19,0);
      lcd.print(" ");
      lcd.setCursor(19,0);
      lcd.print(ie);
      //line 2
      lcd.setCursor(7,1);
      lcd.print("  ");
      lcd.setCursor(7,1);
      lcd.print(assist);
      //line3
      lcd.setCursor(6,2);
      lcd.print("  ");
      lcd.setCursor(6,2);
      lcd.print(alarm);
      if(digitalRead(SetButton))
      {
        SETUP_STATE = SET_DISPLAY;
        digitalWrite(LED_Alarm, HIGH);
      }
      else
      {
        digitalWrite(LED_Alarm, LOW);
        SETUP_STATE = DISPLAY_VAL;
      }
      break;
    }
  }
}

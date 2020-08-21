#include <LiquidCrystal.h>
#include "pinout.h"
#include "interface.h"
#include "configure.h"
#include "motor_ctrl.h"

//ENUMERATIONS
enum main_enum {START, POS1, WAIT1, POS2, WAIT2, STOP};

uint8_t rs = 13, en = 12, d4 = 11, d5 = 10, d6 = 9, d7 = 8;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//VARIABLES
main_enum MAIN_STATE = START;

void setup()
{
  configure_pins();
  lcd.begin(20, 4);
  lcd.clear();
}

void loop()
{
  switch(MAIN_STATE)
  {
    case START:
    {
        lcd.clear();
        lcd.print("Press SET when the");
        lcd.setCursor(0, 1);
        lcd.print("gear is between");
        lcd.setCursor(0, 2);
        lcd.print("POS1 and POS2.");
        delay(500);
        confMotor(20, 1);
        runMotor(540);
        MAIN_STATE = WAIT1;
        break;
    }
    case WAIT1:
    {
        MAIN_STATE = WAIT1;
        if(digitalRead(SetButton))
        {
            MAIN_STATE = STOP;
        }
        else if(!checkMotorRunning())
        {
            MAIN_STATE = STOP;
        }
        break;
    }
    case STOP:
    {
        stopMotor();
        lcd.clear();
        lcd.print("Finished:");
        lcd.setCursor(0, 1);
        lcd.print("Flash the BPAP or");
        lcd.setCursor(0, 2);
        lcd.print("Press SET to retry.");
        delay(1000);
        if(digitalRead(SetButton))
        {
            MAIN_STATE = START;
            lcd.clear();
        }
        break;
    }
    default:
    {
        lcd.print("Main: DEFAULT");
    }
    }
}

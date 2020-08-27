#include <LiquidCrystal.h>
#include "pinout.h"
#include "interface.h"
#include "configure.h"
#include "motor_ctrl.h"

//ENUMERATIONS
enum main_enum {START, POS1, WAIT1, POS2, WAIT2, HOME,
                HOME_SETUP, HOME_IN, HOME_OUT, STOP};
float homeInc = 0.09;

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
            confMotor(40, 3);
            runMotor(540);
            MAIN_STATE = WAIT1;
            break;
        }
        case WAIT1:
        {
            MAIN_STATE = WAIT1;
            if(digitalRead(SetButton))
            {
                MAIN_STATE = HOME_SETUP;
                stopMotor();
                delay(300);
            }
            else if(!checkMotorRunning())
            {
                MAIN_STATE = HOME_SETUP;
                stopMotor();
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
                delay(500);
            }
            break;
        }
        case HOME_SETUP:
        {
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("                    ");
            lcd.setCursor(0,0);
            lcd.print("Home IN or OUT");
            lcd.setCursor(0,1);
            lcd.print("SET to retry");
            lcd.setCursor(0,2);
            lcd.print("Homing             ");
            MAIN_STATE = HOME;
            confMotor(20, 2, true);
            break;
        }
        case HOME:
        {
            lcd.setCursor(7,2);
            if(digitalRead(Home_In))
            {
                lcd.print("in. ");
                runMotor(-1*homeInc);//rotate gear ccw
                MAIN_STATE = HOME_IN;
            }
            else if(digitalRead(Home_Out))
            {
                lcd.print("out.");
                runMotor(homeInc);//rotate gear cw
                MAIN_STATE = HOME_OUT;
            }
            else if(digitalRead(SetButton))
            {
                MAIN_STATE = START;
            }
            break;
        }
        case HOME_IN:
        {
            if(checkMotorRunning())
            {
                MAIN_STATE = HOME_IN;
            }
            else
            {
                MAIN_STATE = HOME;
            }
            break;
        }
        case HOME_OUT:
        {
            if(checkMotorRunning())
            {
                MAIN_STATE = HOME_OUT;
            }
            else
            {
                MAIN_STATE = HOME;
            }
            break;
        }
        default:
        {
            lcd.print("Main: DEFAULT");
        }
    }
}

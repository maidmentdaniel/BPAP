#include <LiquidCrystal.h>
#include "pinout.h"
#include "interface.h"
#include "configure.h"
#include "motor_ctrl.h"
#include "calibrate_fsm.h"
#include "setup_fsm.h"
#include "run_fsm.h"

//ENUMERATIONS
enum main_enum {START, SETUP, CALIBRATE, WAIT, RUN, STOP};

uint8_t rs = 13, en = 12, d4 = 11, d5 = 10, d6 = 9, d7 = 8;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//VARIABLES
main_enum MAIN_STATE = START;
main_enum MAIN_PREV_STATE = MAIN_STATE;
bool state_change = false;

void setup()
{
    configure_pins();
    lcd.begin(20, 4);
    lcd.clear();
    Serial.begin(9600);
}

void loop()
{
    switch(MAIN_STATE)
    {
        case START:
        {
            lcd.clear();
            lcd.print("START state");
            delay(1000);
            lcd.clear();
            MAIN_STATE = CALIBRATE;
            MAIN_PREV_STATE = START;
            break;
        }
        case CALIBRATE:
        {
            state_change = calibrate_FSM(&lcd);
            if(state_change)
            {
                MAIN_STATE = SETUP;
            }
            else
            {
                MAIN_STATE = CALIBRATE;
            }
            MAIN_PREV_STATE = CALIBRATE;
            break;
        }
        case SETUP:
        {
            if(MAIN_PREV_STATE==STOP)
            {
                confMotor(1000);
                runMotor(getSwitchToBag());
            }
            state_change = setup_FSM(&lcd);
            if(state_change)
            {
                MAIN_STATE = RUN;
            }
            MAIN_PREV_STATE = SETUP;
            break;
        }
        case RUN:
        {
            state_change = run_FSM(&lcd);
            if(state_change)
            {
                lcd.clear();
                MAIN_STATE = STOP;
            }
            MAIN_PREV_STATE = RUN;
            break;
        }
        case STOP:
        {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Main: STOP");
            lcd.setCursor(0, 1);
            lcd.print("SET to change settings");
            lcd.setCursor(0, 2);
            lcd.print("and HOME IN to");
            lcd.setCursor(0, 3);
            lcd.print("recalibrate");
            if(digitalRead(SetButton))
            {
                MAIN_STATE = SETUP;
                delay(250);
            }
            else if(digitalRead(Home_In))
            {
                MAIN_STATE = CALIBRATE;
                delay(250);
            }
            MAIN_PREV_STATE = STOP;
            break;
        }
        default:
        {
            lcd.print("Main: DEFAULT");
            break;
        }
    }
}

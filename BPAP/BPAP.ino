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
            break;
        }
        case SETUP:
        {
            MAIN_STATE = SETUP;
            state_change = setup_FSM(&lcd);
            if(state_change)
            {
                MAIN_STATE = RUN;
                delay(500);
            }
            break;
        }
        case RUN:
        {
            MAIN_STATE = RUN;
            state_change = run_FSM(&lcd);
            if(state_change)
            {
                lcd.clear();
                MAIN_STATE = STOP;
            }
            break;
        }
        case STOP:
        {
            lcd.setCursor(0, 0);
            lcd.print("Main: STOP");
            lcd.setCursor(0, 1);
            lcd.print("SET to restart and recalibrate");
            if(digitalRead(SetButton))
            {
                MAIN_STATE = START;
            }
            break;
        }
        default:
        {
            lcd.print("Main: DEFAULT");
            break;
        }
    }
}

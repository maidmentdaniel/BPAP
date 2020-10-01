#include <LiquidCrystal.h>
#include "pinout.h"
#include "interface.h"
#include "configure.h"
#include "motor_ctrl.h"
#include "calibrate_fsm.h"
#include "setup_fsm.h"
#include "run_fsm.h"
#include "log_fsm.h"
#include "EEPROM.h"

//ENUMERATIONS
enum main_enum {START, CALIBRATE,  SETUP, RUN, STOP, LOG};

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
    writestate(MAIN_STATE);
    writetoserial();
    // Line 3:
    lcd.setCursor(17, 3);
    lcd.print(MAIN_STATE);
    lcd.setCursor(16, 3);
    lcd.print(digitalRead(ToggleSwitch));

    switch(MAIN_STATE)
    {
        case START:
        {
            lcd.setCursor(1,1);
            lcd.print(F("Press SET to begin"));
            lcd.setCursor(4,2);
            lcd.print(F("calibration."));
            lcd.setCursor(18, 3);
            lcd.print(F(" "));
            if(digitalRead(SetButton))
            {
                MAIN_STATE = CALIBRATE;
                delay(delay_const);
                lcd.clear();
            }
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
            lcd.setCursor(0, 0);
            lcd.print(F("Press SET to change"));
            lcd.setCursor(0, 1);
            lcd.print(F("settings."));
            lcd.setCursor(0, 2);
            lcd.print(F("Press HOME IN to"));
            lcd.setCursor(0, 3);
            lcd.print(F("recalibrate."));
            lcd.setCursor(18, 3);
            lcd.print(F(" "));
            if(digitalRead(SetButton))
            {
                MAIN_STATE = SETUP;
                confMotor(1000);
                runMotor(getSwitchToBag());
                delay(delay_const);
            }
            else if(digitalRead(Home_In))
            {
                MAIN_STATE = CALIBRATE;
                delay(delay_const);
            }
            else if (digitalRead(Home_Out))
            {
                MAIN_STATE = LOG;
                lcd.clear();
            }
            MAIN_PREV_STATE = STOP;
            break;
        case LOG:
        {
            state_change = log_FSM(&lcd);
            if(state_change)
            {
                lcd.clear();
                MAIN_STATE = STOP;
            }
            MAIN_PREV_STATE = LOG;
            break;
        }
        }
        default:
        {
            lcd.print(F("Main: DEFAULT"));
            break;
        }
    }
}

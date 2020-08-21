#include "calibrate_fsm.h"

//VARIABLES
calib_enum CALIBRATE_STATE = BEGIN_CALIBRATE;

float pressurePrev = 0;
float pressureCur = 0;
float homeInc = 0.09;

bool calibrate_FSM(LiquidCrystal * lcdPtr)
{
    pressureCur = map(analogRead(PressureSensorPIN), 0, 1023, -50.986, 50.986);
    switch(CALIBRATE_STATE)
    {
        case BEGIN_CALIBRATE:
        {
            lcdPtr->clear();
            lcdPtr->print("Calibrating:");
            CALIBRATE_STATE = TO_SWITCH;
            lcdPtr->setCursor(0,1);
            lcdPtr->print("TO_SWITCH");
            // Run ccw
            confMotor(40, 2);
            runMotor(getROM());// rotate through range assuming starting at bag.
            break;
        }
        case TO_SWITCH:
        {
            CALIBRATE_STATE = TO_SWITCH;
            if(!digitalRead(laserPIN) || !checkMotorRunning())
            {
                stopMotor();
                CALIBRATE_STATE = SET_LIMIT_POSITION;
            }
            break;
        }
        case SET_LIMIT_POSITION:// maybe redundent
        {
            lcdPtr->setCursor(0,1);
            lcdPtr->print("SET_LIMIT_POSITION");
            sei();
            delay(1000);
            pressurePrev = 0.2;
            lcdPtr->setCursor(15,3);
            lcdPtr->print(pressurePrev);

            lcdPtr->setCursor(0,1);
            lcdPtr->print("                    ");
            lcdPtr->setCursor(0,1);
            lcdPtr->print("TO_BAG");

            CALIBRATE_STATE = TO_BAG;
            confMotor(40, 2);
            runMotor(-72);//rotate gear cw
            break;
        }
        case TO_BAG:
        {
            CALIBRATE_STATE = TO_BAG;
            if(pressureCur >= pressurePrev || !checkMotorRunning())
            {
                stopMotor();
                setSwitchToBag(getAngle());
                CALIBRATE_STATE = HOME_SETUP;
            }
            lcdPtr->setCursor(0,3);
            lcdPtr->print("     ");
            lcdPtr->setCursor(0,3);
            lcdPtr->print(pressureCur);
            break;
        }
        case HOME_SETUP:
        {
            lcdPtr->setCursor(0,1);
            lcdPtr->print("                    ");
            lcdPtr->setCursor(0,1);
            lcdPtr->print("Home IN or OUT");
            lcdPtr->setCursor(0,2);
            lcdPtr->print("SET to continue");
            lcdPtr->setCursor(0,3);
            lcdPtr->print("Homing             ");
            CALIBRATE_STATE = HOME;
            confMotor(20, 2);
            break;
        }
        case HOME:
        {
            lcdPtr->setCursor(7,3);
            CALIBRATE_STATE = HOME;
            if(digitalRead(Home_In))
            {
                lcdPtr->print("in. ");
                runMotor(-1*homeInc);//rotate gear ccw
                CALIBRATE_STATE = HOME_IN;
            }
            else if(digitalRead(Home_Out))
            {
                lcdPtr->print("out.");
                runMotor(homeInc);//rotate gear cw
                CALIBRATE_STATE = HOME_OUT;
            }
            else if(digitalRead(SetButton))
            {
            CALIBRATE_STATE = FINISH;
            }
            break;
        }
        case HOME_IN:
        {
            if(checkMotorRunning())
            {
                CALIBRATE_STATE = HOME_IN;
            }
            else
            {
                addToSwitchToBag(-1*homeInc);
                CALIBRATE_STATE = HOME;
            }
            break;
        }
        case HOME_OUT:
        {
            if(checkMotorRunning())
            {
                CALIBRATE_STATE = HOME_OUT;
            }
            else
            {
                addToSwitchToBag(homeInc);
                CALIBRATE_STATE = HOME;
            }
            break;
        }
        case FINISH:
        {
            lcdPtr->clear();
            lcdPtr->setCursor(0,1);
            lcdPtr->print("Finished Calibrating");
            delay(500);
            lcdPtr->clear();
            CALIBRATE_STATE = BEGIN_CALIBRATE;
            return true;
            break;
        }
    }
    return false;
}

#include "calibrate_fsm.h"

//VARIABLES
calib_enum CUR_CALIBRATE_STATE = BEGIN_CALIBRATE;
calib_enum PREV_CALIBRATE_STATE = CUR_CALIBRATE_STATE;

float pressurePrev = 0;
float pressureCur = 0;
float homeInc = 0.09;

bool calibrate_FSM(LiquidCrystal * lcdPtr)
{
    PREV_CALIBRATE_STATE = CUR_CALIBRATE_STATE;
    pressureCur = map(analogRead(PressureSensorPIN), 0, 1023, -50.986, 50.986);
    switch(CUR_CALIBRATE_STATE)
    {
        case BEGIN_CALIBRATE:
        {
            lcdPtr->clear();
            lcdPtr->print("Calibrating:");
            CUR_CALIBRATE_STATE = TO_SWITCH;
            lcdPtr->setCursor(0,1);
            lcdPtr->print("TO_SWITCH");
            // Run ccw
            confMotor(40, 2, true);
            runMotor(getROM());// rotate through range assuming starting at bag.
            break;
        }
        case TO_SWITCH:
        {
            CUR_CALIBRATE_STATE = TO_SWITCH;
            if( !digitalRead(laserPIN)
                || !checkMotorRunning()
                ||  digitalRead(SetButton))
            {
                stopMotor();
                CUR_CALIBRATE_STATE = SET_LIMIT_POSITION;
            }
            break;
        }
        case SET_LIMIT_POSITION:// maybe redundent
        {
            lcdPtr->setCursor(0,1);
            lcdPtr->print("SET_LIMIT_POSITION");
            delay(1000);
            pressurePrev = 0.2;
            lcdPtr->setCursor(15,3);
            lcdPtr->print(pressurePrev);

            lcdPtr->setCursor(0,1);
            lcdPtr->print("                    ");
            lcdPtr->setCursor(0,1);
            lcdPtr->print("TO_BAG");

            CUR_CALIBRATE_STATE = TO_BAG;
            confMotor(40, 2, true);
            runMotor(-1*getROM());//rotate gear cw
            break;
        }
        case TO_BAG:
        {
            CUR_CALIBRATE_STATE = TO_BAG;
            if(pressureCur >= pressurePrev || !checkMotorRunning())
            {
                stopMotor();
                setSwitchToBag(getAngle());
                CUR_CALIBRATE_STATE = HOME_SETUP;
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
            CUR_CALIBRATE_STATE = HOME;
            confMotor(20, 2, true);
            break;
        }
        case HOME:
        {
            lcdPtr->setCursor(7,3);
            CUR_CALIBRATE_STATE = HOME;
            if(digitalRead(Home_In))
            {
                lcdPtr->print("in. ");
                runMotor(-1*homeInc);//rotate gear ccw
                CUR_CALIBRATE_STATE = HOME_IN;
            }
            else if(digitalRead(Home_Out))
            {
                lcdPtr->print("out.");
                runMotor(homeInc);//rotate gear cw
                CUR_CALIBRATE_STATE = HOME_OUT;
            }
            else if(digitalRead(SetButton))
            {
            CUR_CALIBRATE_STATE = FINISH;
            }
            break;
        }
        case HOME_IN:
        {
            if(checkMotorRunning())
            {
                CUR_CALIBRATE_STATE = HOME_IN;
            }
            else
            {
                addToSwitchToBag(-1*homeInc);
                CUR_CALIBRATE_STATE = HOME;
            }
            break;
        }
        case HOME_OUT:
        {
            if(checkMotorRunning())
            {
                CUR_CALIBRATE_STATE = HOME_OUT;
            }
            else
            {
                addToSwitchToBag(homeInc);
                CUR_CALIBRATE_STATE = HOME;
            }
            break;
        }
        case FINISH:
        {
            lcdPtr->clear();
            lcdPtr->setCursor(0,1);
            lcdPtr->print("Finished Calibrating");
            delay(2000);
            lcdPtr->clear();
            CUR_CALIBRATE_STATE = BEGIN_CALIBRATE;
            return true;
            break;
        }
    }
    return false;
}

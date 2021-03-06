#include "calibrate_fsm.h"

//VARIABLES
calib_enum CUR_CALIBRATE_STATE = BEGIN_CALIBRATE;
calib_enum PREV_CALIBRATE_STATE = CUR_CALIBRATE_STATE;

static float pressurePrev = 0;
static float pressureCur = 0;
static float homeInc = 0.09;
static float calib_fstep = 1000;
static float calib_threshold = 0.5;

bool calibrate_FSM(LiquidCrystal * lcdPtr)
{
    PREV_CALIBRATE_STATE = CUR_CALIBRATE_STATE;
    pressureCur = map(analogRead(PressureSensorPIN), 0, 1023, -50.986, 50.986);

    // Line 3:
    lcdPtr->setCursor(18, 3);
    lcdPtr->print(CUR_CALIBRATE_STATE);

    switch(CUR_CALIBRATE_STATE)
    {
        case BEGIN_CALIBRATE:
        {
            lcdPtr->clear();
            lcdPtr->print(F("Calibrating:"));
            lcdPtr->setCursor(0,1);
            lcdPtr->print(F("TO_SWITCH           "));
            delay(delay_const);
            CUR_CALIBRATE_STATE = TO_SWITCH;
            confMotor(calib_fstep);
            runMotor(ROM);
            break;
        }
        case TO_SWITCH:
        {
            CUR_CALIBRATE_STATE = TO_SWITCH;
            if( !digitalRead(LimitSwitchPIN)
                || !checkMotorRunning()
                || digitalRead(SetButton))
            {
                stopMotor();
                CUR_CALIBRATE_STATE = SET_LIMIT_POSITION;
            }
            break;
        }
        case SET_LIMIT_POSITION:// maybe redundent
        {
            lcdPtr->setCursor(0,1);
            lcdPtr->print(F("SET_LIMIT_POSITION  "));
            delay(delay_const);
            CUR_CALIBRATE_STATE = TO_BAG;
            confMotor(calib_fstep);
            runMotor(-0.75*ROM);//rotate gear cw
            break;
        }
        case TO_BAG:
        {
            CUR_CALIBRATE_STATE = TO_BAG;
            if  (  (pressureCur >= pressurePrev + calib_threshold)
                || (!checkMotorRunning())
                || (digitalRead(SetButton))
                )
            {
                stopMotor();
                setSwitchToBag(getAngle());
                CUR_CALIBRATE_STATE = HOME_SETUP;
            }
            break;
        }
        case HOME_SETUP:
        {
            lcdPtr->setCursor(0,1);
            lcdPtr->print(F("                    "));
            lcdPtr->setCursor(0,1);
            lcdPtr->print(F("Home IN or OUT"));
            lcdPtr->setCursor(0,2);
            lcdPtr->print(F("SET to continue"));
            lcdPtr->setCursor(0,3);
            lcdPtr->print(F("Homing             "));
            CUR_CALIBRATE_STATE = HOME;
            confMotor(calib_fstep);
            break;
        }
        case HOME:
        {
            lcdPtr->setCursor(7,3);
            CUR_CALIBRATE_STATE = HOME;
            if(digitalRead(Home_In) && abs(getSwitchToBag())<ROM)
            {
                lcdPtr->print(F("in. "));
                runMotor(-1*homeInc);//rotate gear ccw
                CUR_CALIBRATE_STATE = HOME_IN;
            }
            else if(digitalRead(Home_Out) && digitalRead(LimitSwitchPIN))
            {
                lcdPtr->print(F("out."));
                runMotor(homeInc);//rotate gear cw
                CUR_CALIBRATE_STATE = HOME_OUT;
            }
            else if(digitalRead(SetButton))
            {
                delay(delay_const);
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
            lcdPtr->print(F("Finished Calibrating"));
            lcdPtr->clear();
            CUR_CALIBRATE_STATE = BEGIN_CALIBRATE;
            return true;
            break;
        }
    }
    return false;
}

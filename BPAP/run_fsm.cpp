#include "run_fsm.h"

//PROTOTYPES
RUN_enum RUN_STATE = RUN_SETUP;
RUN_enum PREV_STATE = RUN_STATE;

static float _pressCur = 0.00;
static float _pressThresh = -2.00;
static float _posCur = 0.00;
static float _run_speed = 1000;

bool run_FSM( LiquidCrystal * lcdPtr)
{
    PREV_STATE = RUN_STATE;
    if(digitalRead(SetButton))
    {
        _posCur = getAngle();
        stopMotor();
        confMotor(_run_speed);
        runMotor(ROM);
        RUN_STATE = RUN_TO_SWITCH;
    }
    _pressCur = map(analogRead(PressureSensorPIN), 0, 1023, -50.986, 50.986);
    _posCur = getAngle();
    switch(RUN_STATE)
    {
        case RUN_SETUP:
        {
            lcdPtr->clear();
            lcdPtr->print(F("Running RUN SETUP"));
            confMotor(calcStepRate(true, getBagToCentre(), false));
            _pressThresh = -1*getASSIST();
            RUN_STATE = WAIT_INHALE;
            break;
        }
        case WAIT_INHALE:
        {
            lcdPtr->setCursor(8,0);
            lcdPtr->print(F("AWAIT INHALE"));
            if(_pressCur < _pressThresh)
            {
                runMotor(getVOL()*getBagToCentre());
                RUN_STATE = SWEEP_IN;
            }
            break;
        }
        case SWEEP_IN:
        {
            lcdPtr->setCursor(8,0);
            lcdPtr->print(F("SWEEP IN    "));
            if(!checkMotorRunning())
            {
                confMotor(calcStepRate(false, getBagToCentre(), false));
                runMotor(-1*getVOL()*getBagToCentre());
                RUN_STATE = SWEEP_OUT;
            }
            break;
        }
        case SWEEP_OUT:
        {
            lcdPtr->setCursor(8,0);
            lcdPtr->print(F("SWEEP OUT   "));
            if(!checkMotorRunning())
            {
                RUN_STATE = RUN_SETUP;
            }
            break;
        }
        case RUN_TO_SWITCH:
        {
            if(!digitalRead(laserPIN) || !checkMotorRunning())
            {
                stopMotor();
                RUN_STATE = DONE;
            }
            lcdPtr->setCursor(8,0);
            lcdPtr->print(F("TO SWITCH   "));
            break;
        }
        case DONE:
        {
            lcdPtr->setCursor(8,0);
            lcdPtr->print(F("DONE       "));
            delay(1000);
            RUN_STATE = RUN_SETUP;
            return true;
            break;
        }
    }
    return false;
}

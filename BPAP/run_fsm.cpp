#include "run_fsm.h"

//PROTOTYPES
RUN_enum RUN_STATE = RUN_SETUP;
RUN_enum RUN_NEXT_STATE = RUN_SETUP;
RUN_enum RUN_PREV_STATE = RUN_SETUP;

static float _pressCur = 0.00;
static float _pressThresh = -2.00;
static float _posCur = 0.00;
static float _run_speed = 1000;
static float _set_point_pressure = 0;

bool run_FSM( LiquidCrystal * lcdPtr)
{
    if(digitalRead(SetButton))
    {
        _posCur = getAngle();
        stopMotor();
        confMotor(_run_speed);
        runMotor(ROM);
        RUN_NEXT_STATE = RUN_TO_SWITCH;
    }
    _pressCur = map(analogRead(PressureSensorPIN), 0, 1023, -50.986, 50.986);
    
    RUN_STATE = RUN_NEXT_STATE;
    switch(RUN_STATE)
    {
        case RUN_SETUP:
        {
            lcdPtr->clear();
            lcdPtr->print(F("Running RUN SETUP"));
            lcdPtr->setCursor(0,1);
            lcdPtr->print(F("Pressure     cmH2O  "));
            lcdPtr->setCursor(0,2);
            lcdPtr->print(F("Threshold    cmH2O  "));
            lcdPtr->setCursor(0,3);
            lcdPtr->print(F("SET to escape"));
            confMotor(calcStepRate(true, getBagToCentre(), false));
            _pressThresh = -1*getASSIST();
            _set_point_pressure = _pressCur;
            RUN_NEXT_STATE = WAIT_INHALE;
            break;
        }
        case WAIT_INHALE:
        {
            lcdPtr->setCursor(8,0);
            lcdPtr->print(F("AWAIT INHALE"));
            if(_pressCur <= _pressThresh)
            {
                runMotor(getVOL()*getBagToCentre());
                RUN_NEXT_STATE = SWEEP_IN;
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
                RUN_NEXT_STATE = SWEEP_OUT;
            }
            break;
        }
        case SWEEP_OUT:
        {
            lcdPtr->setCursor(8,0);
            lcdPtr->print(F("SWEEP OUT   "));
            if(!checkMotorRunning())
            {
                RUN_NEXT_STATE = RUN_SETUP;
            }
            break;
        }
        case RUN_TO_SWITCH:
        {
            if(!digitalRead(LimitSwitchPIN) || !checkMotorRunning())
            {
                stopMotor();
                RUN_NEXT_STATE = DONE;
            }
            lcdPtr->setCursor(8,0);
            lcdPtr->print(F("TO SWITCH   "));
            break;
        }
        case DONE:
        {
            lcdPtr->setCursor(8,0);
            lcdPtr->print(F("DONE       "));
            delay(delay_const );
            RUN_NEXT_STATE = RUN_SETUP;
            return true;
            break;
        }
    }
    lcdPtr->setCursor(10,1);
    lcdPtr->print(F("   "));
    lcdPtr->setCursor(10,1);
    lcdPtr->print(round(_pressCur));
    lcdPtr->setCursor(10,2);
    lcdPtr->print(F("   "));
    lcdPtr->setCursor(10,2);
    lcdPtr->print(round(_pressThresh));
    RUN_PREV_STATE = RUN_STATE;
    return false;
}

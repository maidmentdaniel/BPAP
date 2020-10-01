#include "run_fsm.h"

//PROTOTYPES
RUN_enum RUN_STATE = RUN_SETUP;
RUN_enum RUN_NEXT_STATE = RUN_SETUP;
RUN_enum RUN_PREV_STATE = RUN_SETUP;

static float _pressCur = 0.00;
static float _pressThresh = -2.00;
static float _posCur = 0.00;
static float _run_speed = 1000;
static bool _alarm_state = false;
static volatile long _start_time = 0;

bool run_FSM( LiquidCrystal * lcdPtr)
{
    writesubstate(RUN_STATE);
    if(digitalRead(SetButton))
    {
        stopAlarm();
        _posCur = getAngle();
        stopMotor();
        confMotor(_run_speed);
        runMotor(ROM);
        RUN_NEXT_STATE = RUN_TO_SWITCH;
    }
    _pressCur = map(analogRead(PressureSensorPIN), 0, 1023, -50.986, 50.986);

    if((_pressCur >= PIP) && _alarm_state)
    {
        startALARM();
        lcdPtr->setCursor(19, 3);
        lcdPtr->print(RUN_STATE);
        lcdPtr->print(2);// Why?
    }

    if(digitalRead(Alarm_switch))
    {
        stopAlarm();
        _alarm_state = !_alarm_state;
        _start_time = millis();
        lcdPtr->setCursor(19, 3);
        lcdPtr->print(RUN_STATE);
        lcdPtr->print(" ");
        delay(delay_const);
    }

    _pressThresh = -1*getASSIST();

    setBPM(map(analogRead(ReadBPM_Val), 0, 1023, 40, -1));
    setVOL(analogRead(ReadTV_Val));
    setIE( map(analogRead(ReadIE_Val), 0, 1023, 5, 0));
    setASSIST(map(analogRead(ReadSensitivity), 0, 1023, 20, -1));
    setALARM(map(analogRead(ReadPEEP_alarm), 0, 1023, 40, -1));

    // Line 0:
    lcdPtr->setCursor(4,0);
    lcdPtr->print(F("  "));
    lcdPtr->setCursor(4,0);
    lcdPtr->print(round(getBPM()));
    lcdPtr->setCursor(11,0);
    lcdPtr->print(F("  "));
    lcdPtr->setCursor(11,0);
    lcdPtr->print((round((getVOL()*100))));
    lcdPtr->setCursor(19,0);
    lcdPtr->print(F(" "));
    lcdPtr->setCursor(19,0);
    lcdPtr->print(round(getIE()));
    // Line 1:
    lcdPtr->setCursor(10,1);
    lcdPtr->print(F("   "));
    lcdPtr->setCursor(10,1);
    lcdPtr->print(round(_pressCur));
    // Line 2:
    lcdPtr->setCursor(10,2);
    lcdPtr->print(F("   "));
    lcdPtr->setCursor(10,2);
    lcdPtr->print(round(_pressThresh));

    RUN_STATE = RUN_NEXT_STATE;
    // Line 3:
    lcdPtr->setCursor(18, 3);
    lcdPtr->print(RUN_STATE);
    lcdPtr->print(_alarm_state);

    switch(RUN_STATE)
    {
        case RUN_SETUP:
        {
            lcdPtr->clear();
            lcdPtr->print(F("BPM   |VOL   |iE 1: "));   // Line 0:
            lcdPtr->setCursor(0,1);
            lcdPtr->print(F("Pressure     cmH2O  "));   // Line 1:
            lcdPtr->setCursor(0,2);
            lcdPtr->print(F("Threshold    cmH2O  "));   // Line 2:
            lcdPtr->setCursor(0,3);
            lcdPtr->print(F("SET to escape"));          // Line 3:

            confMotor(calcStepRate(true, getBagToCentre(), false));
            _alarm_state = true;
            _start_time = millis();
            RUN_NEXT_STATE = WAIT_INHALE;
            break;
        }
        case WAIT_INHALE:
        {
            if((millis() - _start_time >= 30000) && _alarm_state)
            {
                startALARM();
                lcdPtr->setCursor(19, 3);
                lcdPtr->print(RUN_STATE);
                lcdPtr->print(1);
            }
            if(_pressCur <= _pressThresh || digitalRead(ToggleSwitch))
            {
                runMotor(getVOL()*getBagToCentre());
                RUN_NEXT_STATE = SWEEP_IN;
            }
            break;
        }
        case SWEEP_IN:
        {
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
            if(!checkMotorRunning())
            {
                _start_time = millis();
                RUN_NEXT_STATE = WAIT_INHALE;
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
            break;
        }
        case DONE:
        {
            RUN_NEXT_STATE = RUN_SETUP;
            if(digitalRead(!ToggleSwitch))
            {
                return true;
            }
            else
            {
                return false;
            }
            break;
        }
    }

    RUN_PREV_STATE = RUN_STATE;
    return false;
}

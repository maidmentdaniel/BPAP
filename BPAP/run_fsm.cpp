#include "run_fsm.h"

//PROTOTYPES
RUN_enum RUN_STATE = RUN_SETUP;
RUN_enum PREV_STATE = RUN_STATE;

float pressCur = 0.00;
float pressThresh = -2.00;
float posCur = 0.00;

bool run_FSM( LiquidCrystal * lcdPtr)
{
    PREV_STATE = RUN_STATE;
    if(digitalRead(SetButton))
    {
        posCur = getAngle();
        stopMotor();
        confMotor(10, 2, true);
        RUN_STATE = RUN_TO_SWITCH;
    }
    pressCur = map(analogRead(PressureSensorPIN), 0, 1023, -50.986, 50.986);
    posCur = getAngle();
    // Serial.print("Speed: ");
    // Serial.print(getSpeed());
    // Serial.print("\t| Angle: ");
    // Serial.print(getAngle());
    // Serial.print("\t| pressure ");
    // Serial.print(pressCur);
    // Serial.print("\t| MotorIE 1:");
    // Serial.print(getMotorIE());
    // Serial.print("\t| Motor BPM ");
    // Serial.println(getMotorBPM());
    switch(RUN_STATE)
    {
        case RUN_SETUP:
        {
            lcdPtr->clear();
            lcdPtr->print("Running RUN SETUP");
            confMotor(getBPM(), getIE(), true);
            RUN_STATE = WAIT_INHALE;
            break;
        }
        case WAIT_INHALE:
        {
            lcdPtr->setCursor(8,0);
            lcdPtr->print("AWAIT INHALE");
            if(pressCur < pressThresh)
            {
                runMotor(-1*getBagToCentre());
                RUN_STATE = SWEEP_IN;
            }
            break;
        }
        case SWEEP_IN:
        {
            lcdPtr->setCursor(8,0);
            lcdPtr->print("SWEEP IN    ");
            if(!checkMotorRunning())
            {
                confMotor(getBPM(), getIE(), true);
                runMotor(getBagToCentre());
                RUN_STATE = SWEEP_OUT;
            }
            break;
        }
        case SWEEP_OUT:
        {
            lcdPtr->setCursor(8,0);
            lcdPtr->print("SWEEP OUT   ");
            if(!checkMotorRunning())
            {
                RUN_STATE = RUN_SETUP;
            }
            break;
        }
        case RUN_TO_SWITCH:
        {
            confMotor(10, 2, true);
            lcdPtr->setCursor(8,0);
            lcdPtr->print("TO SWITCH   ");
            float theta = 0;
            if(PREV_STATE == SWEEP_OUT)
            {
                theta = getROM()-posCur;
            }
            else if(PREV_STATE == SWEEP_IN)
            {
                theta = (-1*getSwitchToBag())+posCur;
            }
            else if(PREV_STATE == WAIT_INHALE)
            {
                theta = -1*getSwitchToBag();
            }
            else if(PREV_STATE == RUN_SETUP)
            {
                theta = -1*getSwitchToBag();
            }
            Serial.println(PREV_STATE);
            Serial.println(theta);
            runMotor(theta);
            RUN_STATE = DONE;
            delay(5000);
            break;
        }
        case DONE:
        {
            lcdPtr->setCursor(8,0);
            lcdPtr->print("DONE       ");
            delay(2000);
            if(!checkMotorRunning())
            {
                return true;
                RUN_STATE = RUN_SETUP;
                break;
            }
            break;
        }
    }
    return false;
}

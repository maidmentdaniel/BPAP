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
        confMotor(10, 2);
        RUN_STATE = RUN_TO_SWITCH;
    }
    pressCur = map(analogRead(PressureSensorPIN), 0, 1023, -50.986, 50.986);
    posCur = getAngle();
    Serial.print("Speed: ");
    Serial.print(getSpeed());
    Serial.print("\t| Angle: ");
    Serial.print(getAngle());
    Serial.print("\t| pressure ");
    Serial.print(pressCur);
    Serial.print("\t| MotorIE 1:");
    Serial.print(getMotorIE());
    Serial.print("\t| Motor BPM ");
    Serial.println(getMotorBPM());
    switch(RUN_STATE)
    {
        case RUN_SETUP:
        {
            lcdPtr->clear();
            lcdPtr->print("Running RUN SETUP");
            confMotor(getBPM(), getIE());
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
            confMotor(10, 2);
            lcdPtr->setCursor(8,0);
            lcdPtr->print("TO SWITCH   ");
            if(PREV_STATE == SWEEP_OUT)
            {
                Serial.println("SWEEP_OUT");
                Serial.println(getROM()-posCur);
                runMotor(getROM()-posCur);
            }
            else if(PREV_STATE == SWEEP_IN)
            {
                Serial.println("SWEEP_IN");
                Serial.println(-1*getSwitchToBag()+posCur);
                runMotor((-1*getSwitchToBag())+posCur);
            }
            else if(PREV_STATE == WAIT_INHALE)
            {
                Serial.println("WAIT_INHALE");
                Serial.println(-1*getSwitchToBag());
                runMotor(-1*getSwitchToBag());
            }
            else if(PREV_STATE == RUN_SETUP)
            {
                Serial.println("RUN_SETUP");
                Serial.println(-1*getSwitchToBag());
                runMotor(-1*getSwitchToBag());
            }
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

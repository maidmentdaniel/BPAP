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
        confMotor(20, 4);
        RUN_STATE = RUN_TO_SWITCH;
    }
    pressCur = map(analogRead(PressureSensorPIN), 0, 1023, -50.986, 50.986);
    posCur = getAngle();
    //line 2
    Serial.print("Cur ");
    Serial.print(posCur);
    Serial.print("\t| BPM ");
    Serial.print(getBPM());
    Serial.print("\t| bag ");
    Serial.print(getSwitchToBag());
    Serial.print("\t| I:E 1:");
    Serial.print(getIE());
    Serial.print("\t| Pressure ");
    Serial.print(pressCur);
    Serial.print('\r');
    // Serial.print();
    switch(RUN_STATE)
    {
        case RUN_SETUP:
        {
            lcdPtr->clear();
            lcdPtr->print("Running RUN SETUP");
            confMotor(getBPM(), getIE());
            RUN_STATE = RUN_SETUP;
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
                RUN_STATE = WAIT_INHALE;
            }
            break;
        }
        case RUN_TO_SWITCH:
        {
            digitalWrite(LED_Alarm, LOW);
            confMotor(20, 3);
            lcdPtr->setCursor(8,0);
            lcdPtr->print("TO SWITCH   ");
            if(PREV_STATE == SWEEP_OUT)
            {
                runMotor(getROM()-posCur);
            }
            else if(PREV_STATE == SWEEP_IN)
            {
                runMotor((-1*getSwitchToBag())+posCur);
            }
            else if(PREV_STATE == WAIT_INHALE)
            {
                runMotor(-1*getSwitchToBag());
            }
            else if(PREV_STATE == RUN_SETUP)
            {
                digitalWrite(LED_Alarm, HIGH);
                delay(1000);
                runMotor(-1*getSwitchToBag());
            }
            RUN_STATE = DONE;
            break;
        }
        case DONE:
        {
            lcdPtr->setCursor(8,0);
            lcdPtr->print("DONE       ");
            if(!checkMotorRunning())
            {
                return true;
                break;
            }
            break;
        }
    }
    return false;
}

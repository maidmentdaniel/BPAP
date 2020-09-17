#include "log_fsm.h"

log_enum CUR_LOG_STATE = LOG_START;
// log_enum PREV_LOG_STATE = CUR_LOG_STATE;
int addr = 0;

bool log_FSM(LiquidCrystal * lcdPtr)
{
    switch (CUR_LOG_STATE)
    {
        case LOG_START:
        {
            lcdPtr->clear();
            lcdPtr->print(F("LOG Mode:"));
            lcdPtr->setCursor(0,1);
            lcdPtr->print(F("Press SET to begin"));
            lcdPtr->setCursor(0,2);
            lcdPtr->print(F("reading LOG data to "));
            lcdPtr->setCursor(0,3);
            lcdPtr->print(F("USB port."));
            Serial.begin(9600);
            if(digitalRead(SetButton))
            {
                CUR_LOG_STATE = LOG_READOUT;
            }
            CUR_LOG_STATE = LOG_START;
            break;
        }
        case LOG_READOUT:
        {
            byte val;
            for(int i = 0; i < EEPROM.length(); i++)
            {
                val = EEPROM.read(i);
                Serial.println(val);
            }
            CUR_LOG_STATE = LOG_STOP;
            break;
        }
        case LOG_STOP:
        {
            lcdPtr->clear();
            lcdPtr->print(F("Clearing Memory."));
            for(int i = 0; i < EEPROM.length(); i++)
            {
                EEPROM.write(i, 0);
            }
            return true;
            break;
        }
    }
    return false;
}

bool writeEEPROM(int x)
{
    if(addr < EEPROM.length())
    {
        EEPROM.write(addr, uint8_t(x));
        addr +=1;
        return false;
    }
    else
    {
        return true;
    }
}

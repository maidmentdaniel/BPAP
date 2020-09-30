#include "log_fsm.h"

log_enum CUR_LOG_STATE = LOG_START;
// log_enum PREV_LOG_STATE = CUR_LOG_STATE;
static volatile int _addr = 0;
static byte _val = 0;

static char _logstring[2] = {0};
static uint8_t idx = 0;

bool log_FSM(LiquidCrystal * lcdPtr)
{
    lcdPtr->setCursor(0,0);
    lcdPtr->print(F("LOG Mode:"));
    switch (CUR_LOG_STATE)
    {
        case LOG_START:
        {
            lcdPtr->setCursor(0,1);
            lcdPtr->print(F("Press SET to begin"));
            lcdPtr->setCursor(0,2);
            lcdPtr->print(F("reading LOG data to "));
            lcdPtr->setCursor(0,3);
            lcdPtr->print(F("USB port."));
            if(digitalRead(SetButton))
            {
                CUR_LOG_STATE = LOG_READOUT;
                lcdPtr->clear();
                delay(delay_const);
            }
            break;
        }
        case LOG_READOUT:
        {
            lcdPtr->setCursor(0,1);
            lcdPtr->print(F("Reading out.        "));
            for(int i = 0; i < EEPROM.length(); i++)
            {
                _val = EEPROM.read(i);
                Serial.println(_val);
            }
            lcdPtr->clear();
            CUR_LOG_STATE = LOG_STOP;
            break;
        }
        case LOG_STOP:
        {
            lcdPtr->setCursor(0,0);
            lcdPtr->print(F("LOG Mode:"));
            lcdPtr->setCursor(0,1);
            lcdPtr->print(F("Clearing memory.    "));
            for(int i = 0; i < EEPROM.length(); i++)
            {
                EEPROM.write(i, 0);
            }
            CUR_LOG_STATE = LOG_START;
            return true;
            break;
        }
    }
    return false;
}

bool writeEEPROM(int x)
{
    if(_val < EEPROM.length())
    {
        EEPROM.write(_addr, uint8_t(x));
        _addr +=1;
        return false;
    }
    else
    {
        return true;
    }
}

void writemainstate(int x)
{
    _logstring[0] = char(x);
}
void writesubstate(int x)
{
    _logstring[1] = char(x);
}
bool writetoserial(char x)
{
    Serial.print(_logstring[0]);
    Serial.print(',');
    Serial.print(_logstring[1]);
    Serial.print(',');
    Serial.print(round(getBPM()));
    Serial.print(',');
    Serial.print((round((getVOL()*100))));
    Serial.print(',');
    Serial.print(round(getIE()));
    Serial.print(',');
    Serial.print(round(_pressCur));
    Serial.print(',');
    Serial.print(round(_pressThresh));
    Serial.print(',');
    Serial.println(getAngle());
}

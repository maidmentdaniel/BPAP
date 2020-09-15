#include 'log_fsm.h'

log_enum CUR_LOG_STATE = LOG_START;
log_enum PREV_LOG_STATE = CUR_LOG_STATE;

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
            break;
        }
        case LOG_READOUT:
        {
            break;
        }
        case LOG_STOP:
        {
            return true;
            break;
        }
        // case :
        // {
        //     break;
        // }
    }
    return false;
}

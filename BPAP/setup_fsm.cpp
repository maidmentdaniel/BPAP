#include "setup_fsm.h"

SETUP_enum SETUP_STATE = SET_DISPLAY;

bool setup_FSM( LiquidCrystal * lcdPtr)
{
  setBPM(map(analogRead(ReadBPM_Val), 0, 1023, 40, -1));
  setVOL(analogRead(ReadTV_Val));
  Serial.print(F("Vol: "));
  Serial.println(getVOL());
  setIE( map(analogRead(ReadIE_Val), 0, 1023, 5, 0));
  setASSIST(map(analogRead(ReadSensitivity), 0, 1023, 20, -1));
  setALARM(map(analogRead(ReadPEEP_alarm), 0, 1023, 40, -1));
  switch(SETUP_STATE)
  {
    case SET_DISPLAY:
    {
      lcdPtr->clear();
      lcdPtr->print(F("BPM   |VOL   |iE 1: "));
      //line 2
      lcdPtr->setCursor(0,1);
      lcdPtr->print(F("ASSIST   cmH2O      "));
      //line3
      lcdPtr->setCursor(0,2);
      lcdPtr->print(F("ALARM   cmH2O       "));
      //line 3
      lcdPtr->setCursor(0,3);
      lcdPtr->print(F("  SET to continue  "));
      SETUP_STATE = DISPLAY_VAL;
      break;
    }
    case DISPLAY_VAL:
    {
      SETUP_STATE = DISPLAY_VAL;
      //line 1
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
      //line 2
      lcdPtr->setCursor(7,1);
      lcdPtr->print(F("  "));
      lcdPtr->setCursor(7,1);
      lcdPtr->print(getASSIST());
      //line3
      lcdPtr->setCursor(6,2);
      lcdPtr->print(F("  "));
      lcdPtr->setCursor(6,2);
      lcdPtr->print(getALARM());
      if(digitalRead(SetButton))
      {
        lcdPtr->clear();
        delay(250);
        SETUP_STATE = SET_DISPLAY;
        return true;
      }
      break;
    }
  }
  return false;
}

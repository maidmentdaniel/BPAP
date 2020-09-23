#include "pinout.h"
#include "configure.h"

//ENUMERATIONS
enum main_enum {START, CALIBRATE,  SETUP, RUN, STOP, LOG};

//VARIABLES
main_enum MAIN_STATE = START;
main_enum MAIN_PREV_STATE = MAIN_STATE;
// bool state_change = false;
char incomingByte = 0; // for incoming serial data

void setup()
{
    configure_pins();
    Serial.begin(9600);
}

void loop()
{
  // send data only when you receive data:
  if (Serial.available() > 0)
  {
        // read the incoming byte:
        incomingByte = Serial.read();
        // say what you got:
        Serial.println(incomingByte);
  }
    // switch(MAIN_STATE)
    // {
    //     case START:
    //     {
    //         break;
    //     }
    //     case CALIBRATE:
    //     {
    //         break;
    //     }
    //     case SETUP:
    //     {
    //         break;
    //     }
    //     case RUN:
    //     {
    //         break;
    //     }
    //     case STOP:
    //     {
    //         break;
    //     }
    //     case LOG:
    //     {
    //         break;
    //     }
    //     default:
    //     {
    //         break;
    //     }
    // }
}

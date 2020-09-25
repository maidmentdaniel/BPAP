#include "pinout.h"
#include "configure.h"
#define SIZE 10

//ENUMERATIONS
enum main_enum {START, RECEIVE, TRANSMIT,  SETUP, RUN, STOP, LOG};

//VARIABLES
main_enum MAIN_STATE = RECEIVE;

// int size = 10;
char incomingBuffer[SIZE]; // for incoming serial data
uint8_t idx = 0;

void setup()
{
    configure_pins();
    Serial.begin(9600);
}

void loop()
{
    switch(MAIN_STATE)
    {
        case RECEIVE:
        {
            if(Serial.available() > 0)
            {
                incomingBuffer[idx] = Serial.read();
                idx += 1;
                if(Serial.available() <= 0 || idx > SIZE-1)
                {
                    MAIN_STATE = TRANSMIT;
                    idx = 0;
                    delay(100);
                }
            }
            break;
        }
        case TRANSMIT:
        {
            for(int i=0; i<SIZE; i++)
            {
                Serial.print(incomingBuffer[i]);
            }
            MAIN_STATE = RECEIVE;
            break;
        }
    }
  // send data only when you receive data:

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

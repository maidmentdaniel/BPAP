#include "pinout.h"
#include "configure.h"
#define SIZE 10
#define time_delay   1000

//ENUMERATIONS
enum main_enum {START, RECEIVE, TRANSMIT,  SETUP, RUN, STOP, LOG};

//VARIABLES
main_enum MAIN_STATE = RECEIVE;

// int size = 10;
char incomingBuffer[SIZE] = {0}; // for incoming serial data
uint8_t idx = 0;
int num_bytes = 0;

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
            num_bytes = Serial.available();
            if(num_bytes > 0)
            {
                for(int i = 0; i<num_bytes; i++)
                {
                    incomingBuffer[i] = Serial.read();
                }
                // Serial.readBytes(incomingBuffer, num_bytes);
                delay(time_delay);
                MAIN_STATE = TRANSMIT;
            }
            break;
        }
        case TRANSMIT:
        {
            // Serial.write("No. recieved: ");
            // delay(time_delay);
            // Serial.write(num_bytes);
            // delay(time_delay);
            // Serial.write('\n');
            // delay(time_delay);
            Serial.write("Buffer: ");
            delay(time_delay);
            Serial.write(incomingBuffer, DEC);
            delay(time_delay);
            incomingBuffer[SIZE] = {0};
            // Serial.write('\n');
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

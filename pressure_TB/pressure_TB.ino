#include "Arduino.h"
#include "pinout.h"
#include "configure.h"

float pressurePrev = 0;
float pressureCur = 0;

//ENUMERATIONS
enum main_enum {START, STOP};
main_enum STATE = START;

void setup()
{
  Serial.begin(9600);
  digitalWrite(LED_Alarm, LOW);
  configure_pins();
}

void loop()
{
  pressureCur = map(analogRead(PressureSensorPIN), 0, 1023, -50.986, 50.986);
  Serial.println(pressureCur);
  delay(100);
//  switch(STATE)
//  {
//    case START:
//    {
//      if(pressureCur<pressurePrev)
//      {
//        digitalWrite(LED_Alarm, LOW);
//        stepper.runSpeed();
//        STATE = START;
//      }
//      else if(pressureCur>=pressurePrev)
//      {
//        STATE = STOP;
//        digitalWrite(LED_Alarm, HIGH);
//      }
//      break;
//    }
//    case STOP:
//    {
//      stepper.stop();
//      STATE = STOP;
//      break;
//    }
//  }
}

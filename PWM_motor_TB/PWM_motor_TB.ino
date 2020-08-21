#include "Arduino.h"
#include "pinout.h"
#include "interface.h"
#include "motor_ctrl.h"

int angle = 45;
void setup()
{
  Serial.begin(9600);
  confMotor(20, 3);
  runMotor(angle);
}

void loop()
{
    Serial.println(getAngle());
    if(!checkMotorRunning())
    {
        angle *= -1;
        delay(1000);
        // confMotor(20, 3);
        runMotor(angle);
    }
}

#include "pinout.h"
#include "configure.h"
void setup()
{
  configure_pins();
  Serial.begin(9600);
}

void loop()
{
  Serial.println(digitalRead(LimitSwitchPIN));
  delay(100);
}

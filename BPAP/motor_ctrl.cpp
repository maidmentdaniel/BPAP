#include "motor_ctrl.h"

volatile int counter0 = 0;
volatile int counter1 = 0;
volatile int direction = 1;

void confMotor(float fStep)
{
    TIMSK4 = 0;                 // disable  Timer/Counter 4 Interrupt Mask Register
    TIFR4 = 0;                  //clear timer 4 counter interrupt flag register
    TCNT4 = 0;                  // clear timer counter 4, pg 154
    TCCR4B = 0;                 // clear timer 4 control register B, pg 154

    // Configure timer 4 control register A,
    // Clear OC4A on compare match, set OC4A at BOTTOM.
    // PWM in fast mode
    TCCR4A = B10000010;

    pinMode(drivePIN, OUTPUT);  //PWM PIN
    pinMode(dirPIN, OUTPUT);    //Direction PIN

    setMotor(fStep);
}

void setMotor(float fStep)
{
    ICR4_var = round((fck)/(N*fStep));
    OCR4A_var = round(0.5*ICR4_var);
    //Set frequency for an I:E ratio of 1:3 at 20BPM
    ICR4 = ICR4_var;
    //Duty cycle is 50%, hence OCR4B = 0.5*ICR4
    OCR4A = OCR4A_var;

}

 void runMotor(float sweep)
{
    counter0 = 0;
    counter1 = round(abs(sweep)/step_size);

    if(sweep<0)
    {
        digitalWrite(dirPIN, HIGH);
        direction = -1;
    }
    else
    {
        digitalWrite(dirPIN, LOW);
        direction = 1;
    }
    TIMSK4 |= (1 << OCIE4A);
    TCCR4B = B00011011;
    motorDebug();
}

void stopMotor()
{
    TIMSK4 |= (0 << OCIE4A);
    TCCR4B = 0;
}

bool checkMotorRunning()
{
    if(counter0 < counter1)
    {
        return true;
    }
    else
    {
        return false;
    }
}

ISR(TIMER4_COMPA_vect)
{
  counter0 += 1;
  if(counter0 >= counter1)
  {
    stopMotor();
  }
}

const int getAngle()
{
    return direction*counter0*step_size;
}

const int getSweep()
{
    return sweep;
}

void motorDebug()
{
    Serial.println("DEBUG Motor:");
    Serial.print("\t| ROM: ");
    Serial.print(ROM);
    Serial.print("\t| fStep: ");
    Serial.print(fStep);
    Serial.print("\t| ICR4: ");
    Serial.println(ICR4);
}

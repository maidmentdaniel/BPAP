#include "motor_ctrl.h"

static int _counter0;
static int _counter1;
static int _direction;
static float _fstep;
static float _sweep;

void confMotor(float fstep)
{
    _fstep = fstep;
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

    setMotor(_fstep);
}

void setMotor(float fstep)
{
    _fstep = fstep;
    // ICR4_var = round((fck)/(N*fStep));
    // OCR4A_var = round(0.5*ICR4_var);
    //Set frequency for an I:E ratio of 1:3 at 20BPM
    ICR4 = round((fck)/(N*_fstep));
    //Duty cycle is 50%, hence OCR4B = 0.5*ICR4
    OCR4A = round(0.5*ICR4);

}

 void runMotor(float sweep)
{
    _sweep = sweep;
    _counter0 = 0;
    _counter1 = round(abs(_sweep)/step_size);

    if(_sweep<0)
    {
        digitalWrite(dirPIN, HIGH);
        _direction = -1;
    }
    else
    {
        digitalWrite(dirPIN, LOW);
        _direction = 1;
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
    if(_counter0 < _counter1)
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
  _counter0 += 1;
  if(_counter0 >= _counter1)
  {
    stopMotor();
  }
}

const int getAngle()
{
    return _direction*_counter0*step_size;
}

void motorDebug()
{
    Serial.println("DEBUG Motor:");
    Serial.print("\t| ROM: ");
    Serial.print(ROM);
    Serial.print("\t| fStep: ");
    Serial.print(_fstep);
    Serial.print("\t| ICR4: ");
    Serial.println(ICR4);
}

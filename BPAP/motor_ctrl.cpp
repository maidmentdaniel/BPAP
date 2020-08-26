#include "motor_ctrl.h"

int counter0 = 0;
int counter1 = 0;

long int fck = 0;
float step_size = 0;
int ROM = 72;
int N = 64;
int direction = 1;

float bpm = 20;                     //beats per minute
float ie = 3;                       //ie
float bps = bpm/60;                 // beats per second
float T = 1/bps;                    // period per beat
float in_T = T/(ie+1);              // inhilation period
float ex_T = (T*ie)/(ie+1);         // exhilation period
float fStep = (ROM/in_T)/step_size; // step rate

int ICR4_var = round((fck)/(N*fStep));
int OCR4A_var = round(0.5*ICR4_var);

void confMotor(float bpm, float ie)
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

    setMotor(bpm, ie);
}

void setMotor(float bpm, float ie)
{
    fck = getClockSpeed();
    step_size = getStepSize();
    ROM = getBagToCentre();

    bps = bpm/60;                 // beats per second
    T = 1/bps;                    // period per beat
    in_T = T/(ie+1);              // inhilation period
    ex_T = (T*ie)/(ie+1);         // exhilation period
    fStep = (ROM/in_T)/step_size; // step rate
    ICR4_var = round((fck)/(N*fStep));
    OCR4A_var = round(0.5*ICR4_var);

    //Set frequency for an I:E ratio of 1:3 at 20BPM
    ICR4 = ICR4_var;
    //Duty cycle is 50%, hence OCR4B = 0.5*ICR4
    OCR4A = OCR4A_var;

}

 void runMotor(float rotate)
{
    counter0 = 0;
    counter1 = round(abs(rotate)/step_size);

    //OCIE4A: Timer/Countern, Output Compare A Match Interrupt Enable
    //When this bit is written to one, and the I-flag in the Status Register is
    //set (interrupts globally enabled), the Timer/Countern Output Compare A
    // Match interrupt is enabled. The corresponding Interrupt Vector
    //(see “Inter-rupts” on page 101) is executed when the OCFnA Flag,
    //located in TIFRn, is set.
    if(rotate<0)
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

int getSpeed()
{
    return fStep*step_size;
}

float getMotorBPM()
{
    return bpm;
}

float getMotorIE()
{
    return ie;
}

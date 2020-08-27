#include "motor_ctrl.h"

volatile int counter0 = 0;
volatile int counter1 = 0;

volatile long int fck = 0;
volatile float step_size = 0;
volatile int ROM = 72;
volatile int N = 64;
volatile int direction = 1;

volatile float BPM = 20;                         //beats per minute
volatile float IE = 3;                       //ie
volatile float bps = BPM/60;                 // beats per second
volatile float T = 1/bps;                    // period per beat
volatile float in_T = T/(IE+1);              // inhilation period
volatile float ex_T = (T*IE)/(IE+1);         // exhilation period
volatile float fStep = (ROM/in_T)/step_size; // step rate

volatile int ICR4_var = round((fck)/(N*fStep));
volatile int OCR4A_var = round(0.5*ICR4_var);

void confMotor(float bpm, float ie, bool debug)
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

    setMotor(bpm, ie, debug);
}

void setMotor(float bpm, float ie, bool debug)
{
    fck = getClockSpeed();
    step_size = getStepSize();
    ROM = getBagToCentre();
    BPM = bpm;
    IE = ie;
    bps = BPM/60;                 // beats per second
    T = 1/bps;                    // period per beat
    in_T = T/(IE+1);              // inhilation period
    ex_T = (T*IE)/(IE+1);         // exhilation period
    fStep = (ROM/in_T)/step_size; // step rate
    ICR4_var = round((fck)/(N*fStep));
    OCR4A_var = round(0.5*ICR4_var);

    //Set frequency for an I:E ratio of 1:3 at 20BPM
    ICR4 = ICR4_var;
    //Duty cycle is 50%, hence OCR4B = 0.5*ICR4
    OCR4A = OCR4A_var;

    if(debug)
    {
        Serial.println("DEBUG Motor:");
        Serial.print("\t| ROM: ");
        Serial.print(ROM);
        Serial.print("\t| BPM: ");
        Serial.print(BPM);
        Serial.print("\t| IE: ");
        Serial.print(IE);
        Serial.print("\t| bps: ");
        Serial.print(bps);
        Serial.print("\t| T: ");
        Serial.print(T);
        Serial.print("\t| in_T: ");
        Serial.print(in_T);
        Serial.print("\t| fStep: ");
        Serial.print(fStep);
        Serial.print("\t| ICR4_var: ");
        Serial.print(ICR4_var);
        Serial.print("\t| OCR4A_var: ");
        Serial.println(OCR4A_var);
    }

}

 void runMotor(float rotate)
{
    counter0 = 0;
    counter1 = round(abs(rotate)/step_size);


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
    return BPM;
}

float getMotorIE()
{
    return IE;
}

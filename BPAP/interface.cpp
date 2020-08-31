#include "interface.h"

//global variables
const float pressureThreshold = 0.5;

static float   _bpm = 20;
static float   _vol = 1.0;
static float   _ie = 2;
static int     _assist = -5;
static int     _alarm = 20;
static float   _switch_to_bag = 45.0;
static float   _bag_to_centre = ROM - _switch_to_bag;
static float   _sweep = ROM;
static float   _fstep = 0.0;

static float _bps = _bpm/60;                      // beats per second [Hz]
static float _T = 1/_bps;                          // period per beat [s]
static float _in_T = _T/(_ie+1);                   // inhilation period [s]
static float _ex_T = (_T*_ie)/(_ie+1);   // exhilation period [s]

float getpressureThreshold(){return pressureThreshold;}
float getStepSize(){return step_size;}
float   getBPM(){return _bpm;}
float   getVOL(){return _vol;}
float   getIE(){return _ie;}
int     getASIST(){return _assist;}
int     getALARM(){return _alarm;}
float   getSwitchToBag(){return _switch_to_bag;}
float   getBagToCentre()
{
    _bag_to_centre = ROM - _switch_to_bag;
    return _bag_to_centre;
}

void setBPM(float bpm){_bpm = bpm;}
void setVOL(float vol){_vol = vol;}
void setIE(float ie){_ie = ie;}
void setASIST(int assist){_assist = assist;}
void setALARM(int alarm){_alarm = alarm;}
void setSwitchToBag(float switch_to_bag){_switch_to_bag = switch_to_bag;}
void setBagToCentre(float bag_to_centre){_bag_to_centre = bag_to_centre;}
void addToSwitchToBag(float delta){_switch_to_bag += delta;}

float calcStepRate(bool inhale, float sweep)
{
    _bps = _bpm/60;                      // beats per second [Hz]
    _T = 1/_bps;                          // period per beat [s]
    _in_T = _T/(_ie+1);                   // inhilation period [s]
    _ex_T = (_T*_ie)/(_ie+1);   // exhilation period [s]

    if(inhale)
    {
        _fstep = (sweep/_in_T)/step_size;       // step rate
    }
    else
    {
        _fstep = (sweep/_ex_T)/step_size;       // step rate
    }

    return _fstep;
}

int getSpeed()
{
    return _fstep*step_size;
}

float getMotorBPM()
{
    return _bpm;
}

float getMotorIE()
{
    return _ie;
}

const int getSweep()
{
    return _sweep;
}

//global variables
const float pressureThreshold = 0.5;

float getpressureThreshold(){return pressureThreshold;}
float getStepSize(){return step_size;}

float   _bpm = 20;
float   _vol = 1.0;
float   _ie = 2;
int     _assist = -5;
int     _alarm = 20;
float   _switch_to_bag = 45.0;
float   _bag_to_centre = ROM - _switch_to_bag;
float   _sweep = ROM;
float   _fStep = 0.0;

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
    bps = _bpm/60;                       // beats per second [Hz]
    T = 1/bps;                          // period per beat [s]
    in_T = T/(_ie+1);                    // inhilation period [s]
    ex_T = (T*_ie)/(_ie+1);               // exhilation period [s]

    if(inhale):
    {
        fStep = (sweep/in_T)/step_size;       // step rate
    }
    else
    {
        fStep = (sweep/ex_T)/step_size;       // step rate
    }

    return fStep;
}

int getSpeed()
{
    return fStep*step_size;
}

float getMotorBPM()
{
    return _bpm;
}

float getMotorIE()
{
    return _ie;
}

//global variables
const long int fck = 16000000;  //clock frequency
const int N = 64;               //clock divider
const float step_size = 0.09;   //deg/step
const float pressureThreshold = 0.5;

long int getClockSpeed(){return fck;}
int getClockDiv(){return N;}
float getpressureThreshold(){return pressureThreshold;}
float getStepSize(){return step_size;}

int _bpm = 20;
int _vol = 100;
int _ie = 5;
int _assist = -5;
int _alarm = 20;
float _switch_to_bag = 0.00;

int getBPM(){return _bpm;}
int getVOL(){return _vol;}
int getIE(){return _ie;}
int getASIST(){return _assist;}
int getALARM(){return _alarm;}
float getSwitchToBag(){return _switch_to_bag;}

void setBPM(int bpm){_bpm = bpm;}
void setVOL(int vol){_vol = vol;}
void setIE(int ie){_ie = ie;}
void setASIST(int assist){_assist = assist;}
void setALARM(int alarm){_alarm = alarm;}
void setSwitchToBag(float switch_to_bag){_switch_to_bag = switch_to_bag;}
void addToSwitchToBag(float delta){_switch_to_bag += delta;}

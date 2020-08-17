#include "delayEffect.h"

void delayEffect::initParams(uint16_t index){
  lfo->begin(WAVEFORM_SINE);
  lfo->frequency(0.1);
  
  lowCtrl->begin(WAVEFORM_SAWTOOTH);
  lowCtrl->frequency(8);
  lowCtrl->amplitude(0);
  lowCtrl->offset(0.5);
  
  highCtrl->begin(WAVEFORM_SAWTOOTH_REVERSE);
  highCtrl->frequency(8);
  highCtrl->amplitude(0);
  highCtrl->offset(0.5);
  
  feedback->gain(0,0);
  
  modCtrl->gain(0,1);
  modCtrl->gain(2,0);
  modCtrl->gain(3,0);
  
  xdly->setbuf(LEN, delaybuf);
  
  ind=index;
}

void delayEffect::toggleShift(){
  toggle=(toggle+1)%2;
  if(!toggle){
    lowCtrl->amplitude(0);
    highCtrl->amplitude(0);
    modCtrl->gain(0,1);
    Serial.print("Pitch-shifting for delay ");
    Serial.print(ind);
    Serial.println(": off");
  }
  else{
    lowCtrl->amplitude(0.5);
    highCtrl->amplitude(0.5);
    modCtrl->gain(0,0);
    Serial.print("Pitch-shifting for delay ");
    Serial.print(ind);
    Serial.println(": on");        
  }
}

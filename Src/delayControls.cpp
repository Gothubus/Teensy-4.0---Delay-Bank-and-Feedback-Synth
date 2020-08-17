#include "delayControls.h"

void delayControls::updateParameters(delayEffect& delayEffect){
  dryWet=dryWetPot->updatePot();
  delayTime=delayTimePot->updatePot();
  fBack=fBackPot->updatePot();
  modAmp=modAmpPot->updatePot();
  modRate=modRatePot->updatePot();
  cutoff=filterPot->updatePot();

  //modulation amplitude scaling
  if(modAmp>0.15)modOn=1;  
  else modOn=0;
  modAmp=pow(modAmp,7);
  modAmp*=0.5*log(delayTime*1.8+1);

  //modulation frequency scaling
  modRate=pow(modRate,7);

  //filter cutoff scaling
  cutoff=pow(2,cutoff);

  delayEffect.finalMix->gain(0,1-dryWet);
  delayEffect.finalMix->gain(1,dryWet);
  delayEffect.dlyCtrl->amplitude(delayTime);  
  delayEffect.feedback->gain(0,fBack);
  delayEffect.modCtrl->gain(1,modOn);
  delayEffect.lfo->amplitude(modAmp);
  delayEffect.lfo->frequency(modRate);
  delayEffect.filter->frequency(cutoff);

  //pitch shifting waveforms scaling
  delayTime-=0.5;
  float lowShift=(delayTime<0)?abs(delayTime):0;
  float highShift=(delayTime>0)?delayTime:0;
  delayEffect.modCtrl->gain(2,lowShift);
  delayEffect.modCtrl->gain(3,highShift);
}

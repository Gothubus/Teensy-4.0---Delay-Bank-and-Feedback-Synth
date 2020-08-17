#ifndef delayControls_h
#define delayControls_h

#include <Arduino.h>
#include "potControl.h"
#include "delayEffect.h"

class delayControls{
  public:
    delayControls(potControl& pot1,
                potControl& pot2,
                potControl& pot3,
                potControl& pot4,
                potControl& pot5,
                potControl& pot6){
      dryWetPot=&pot1;
      delayTimePot=&pot2;
      fBackPot=&pot3;
      modAmpPot=&pot4;
      modRatePot=&pot5;
      filterPot=&pot6;
    }
    void updateParameters(delayEffect& delayEffect);
  private:
    potControl* dryWetPot;
    potControl* delayTimePot;
    potControl* fBackPot;
    potControl* modAmpPot;
    potControl* modRatePot;
    potControl* filterPot;
    float dryWet,delayTime,fBack,modAmp,modRate,cutoff;
    bool modOn;
};

#endif

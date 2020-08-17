#include "potControl.h"

potControl::potControl(){
  this->setParams(A1,1000,50);
}

void potControl::setParams(int16_t pin,float range,int16_t smooth){
  potHistory=analogRead(pin);
  readPin=pin;
  potRange=range;
  potSmooth=smooth;
}

float potControl::updatePot(){
  int pot=analogRead(readPin);
  if(pot>potHistory+potSmooth||pot<potHistory-potSmooth)potHistory=pot;
  return potHistory/potRange;
}

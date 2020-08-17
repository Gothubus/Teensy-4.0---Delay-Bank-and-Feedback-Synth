#ifndef potControl_h
#define potControl_h

#include <Arduino.h>

class potControl{
  public:
    potControl();
    void setParams(int16_t pin,float range,int16_t smooth);
    float updatePot();
  private:
    int16_t readPin,potSmooth;
    float potRange,potHistory;
};

#endif

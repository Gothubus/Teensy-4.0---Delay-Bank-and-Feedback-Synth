#ifndef delayEffect_h
#define delayEffect_h

#include <Arduino.h>
#include <Audio.h>

//approximately 1/3 of internal ram
static const uint32_t LEN=0x22AA;

class delayEffect{
  public:
    delayEffect(AudioSynthWaveform&    lfoIn,
                AudioSynthWaveform&    lowCtrlIn,
                AudioSynthWaveform&    highCtrlIn,
                AudioSynthWaveformDc&  dlyCtrlIn,
                AudioMixer4&           modCtrlIn,
                AudioMixer4&           feedbackIn,
                AudioMixer4&           finalMixIn,
                AudioFilterStateVariable&   filterIn,
                AudioEffectModDelay&   xdlyIn,uint8_t index){
      lfo=&lfoIn;
      lowCtrl=&lowCtrlIn;
      highCtrl=&highCtrlIn;
      dlyCtrl=&dlyCtrlIn;
      modCtrl=&modCtrlIn;
      feedback=&feedbackIn;
      finalMix=&finalMixIn;
      filter=&filterIn;
      xdly=&xdlyIn;
      toggle=0;
      ind=index;
    }
    void initParams(uint16_t index);
    void toggleShift();
    AudioSynthWaveform*    lfo;
    AudioSynthWaveform*    lowCtrl;
    AudioSynthWaveform*    highCtrl;
    AudioSynthWaveformDc*  dlyCtrl;
    AudioMixer4*           modCtrl;
    AudioMixer4*           feedback;
    AudioMixer4*           finalMix;
    AudioFilterStateVariable* filter;
    AudioEffectModDelay*   xdly;
  private:
    int16_t delaybuf[LEN];
    bool toggle;
    uint8_t ind;
};

#endif

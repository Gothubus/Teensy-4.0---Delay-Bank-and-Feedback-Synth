#include <Audio.h>
#include <Wire.h>
#include <SD.h>
#include <SPI.h>
#include <SerialFlash.h>
#include <Bounce.h>
#include "mod-delay.h" //mod delay by Byron Jacquot
#include "delayEffect.h"
#include "delayControls.h"
#include "potControl.h"

//audio library objects===================================================================

//effect 1
AudioSynthWaveform    lfo;
AudioSynthWaveform    lowCtrl;
AudioSynthWaveform    highCtrl;
AudioSynthWaveformDc  dlyCtrl;
AudioMixer4           modCtrl;
AudioMixer4           feedback;
AudioMixer4           finalMix;
AudioFilterStateVariable filter;
AudioEffectModDelay   xdly;
delayEffect delay1(lfo,lowCtrl,highCtrl,dlyCtrl,modCtrl,feedback,finalMix,filter,xdly,1);

//effect 2
AudioSynthWaveform    lfo2;
AudioSynthWaveform    lowCtrl2;
AudioSynthWaveform    highCtrl2;
AudioSynthWaveformDc  dlyCtrl2;
AudioMixer4           modCtrl2;
AudioMixer4           feedback2;
AudioMixer4           finalMix2;
AudioFilterStateVariable filter2;
AudioEffectModDelay   xdly2;
delayEffect delay2(lfo2,lowCtrl2,highCtrl2,dlyCtrl2,modCtrl2,feedback2,finalMix2,filter2,xdly2,2);

//effect 3
AudioSynthWaveform    lfo3;
AudioSynthWaveform    lowCtrl3;
AudioSynthWaveform    highCtrl3;
AudioSynthWaveformDc  dlyCtrl3;
AudioMixer4           modCtrl3;
AudioMixer4           feedback3;
AudioMixer4           finalMix3;
AudioFilterStateVariable filter3;
AudioEffectModDelay   xdly3;
delayEffect delay3(lfo3,lowCtrl3,highCtrl3,dlyCtrl3,modCtrl3,feedback3,finalMix3,filter3,xdly3,3);
     
AudioInputI2S         i2s1;
AudioOutputI2S        i2s2;
AudioPlaySdWav        playWav;
AudioMixer4           inputMix;

//audio library connections=======================================================================

AudioConnection       patchCord0(i2s1,0,inputMix,0);
AudioConnection       patchCord01(playWav,0,inputMix,1);
AudioConnection       patchCord02(i2s1,1,inputMix,2);

//delay 1
AudioConnection       patchCord1(inputMix,0,finalMix,0);
AudioConnection       patchCord2(inputMix,0,filter,0);
AudioConnection       patchCord3(filter,0,feedback,1);
AudioConnection       patchCord4(xdly,0,finalMix,1);
AudioConnection       patchCord5(feedback,0,xdly,0);
AudioConnection       patchCord6(xdly,0,feedback,0);
AudioConnection       patchCord7(dlyCtrl,0,modCtrl,0);
AudioConnection       patchCord8(lfo,0,modCtrl,1);
AudioConnection       patchCord9(lowCtrl,0,modCtrl,2);
AudioConnection       patchCord10(highCtrl,0,modCtrl,3);
AudioConnection       patchcord11(modCtrl,0,xdly,1);

//delay 2
AudioConnection       patchCord12(finalMix,0,finalMix2,0);
AudioConnection       patchCord13(finalMix,0,filter2,0);
AudioConnection       patchCord14(filter2,0,feedback2,1);
AudioConnection       patchCord15(xdly2,0,finalMix2,1);
AudioConnection       patchCord16(feedback2,0,xdly2,0);
AudioConnection       patchCord17(xdly2,0,feedback2,0);
AudioConnection       patchCord18(dlyCtrl2,0,modCtrl2,0);
AudioConnection       patchCord19(lfo2,0,modCtrl2,1);
AudioConnection       patchCord20(lowCtrl2,0,modCtrl2,2);
AudioConnection       patchCord21(highCtrl2,0,modCtrl2,3);
AudioConnection       patchcord22(modCtrl2,0,xdly2,1);

//delay 2
AudioConnection       patchCord23(finalMix2,0,finalMix3,0);
AudioConnection       patchCord24(finalMix2,0,filter3,0);
AudioConnection       patchCord25(filter3,0,feedback3,1);
AudioConnection       patchCord26(xdly3,0,finalMix3,1);
AudioConnection       patchCord27(feedback3,0,xdly3,0);
AudioConnection       patchCord28(xdly3,0,feedback3,0);
AudioConnection       patchCord29(dlyCtrl3,0,modCtrl3,0);
AudioConnection       patchCord30(lfo3,0,modCtrl3,1);
AudioConnection       patchCord31(lowCtrl3,0,modCtrl3,2);
AudioConnection       patchCord32(highCtrl3,0,modCtrl3,3);
AudioConnection       patchcord33(modCtrl3,0,xdly3,1);
AudioConnection       patchCord34(finalMix3,0,i2s2,0);
AudioConnection       patchCord35(finalMix3,0,i2s2,1);

AudioControlSGTL5000  sgtl5000_1;

#define SDCARD_CS_PIN    10
#define SDCARD_MOSI_PIN  7
#define SDCARD_SCK_PIN   14

const uint8_t buttonPin=2;
uint8_t state=0;
uint8_t state2=0;
bool stateWaiting=false;
bool stateWaiting2=false;
bool stateWaiting3=false;

const int myInput=AUDIO_INPUT_LINEIN;

potControl dryWetPot,delayTimePot,fBackPot,modAmpPot,modRatePot,filterPot;

delayControls parameters(dryWetPot,delayTimePot,fBackPot,modAmpPot,modRatePot,filterPot);

void setup() {
  Serial.begin(9600);

  //assign memory for audio
  AudioMemory(20);

  //initialise to line in
  inputMix.gain(0,1);
  inputMix.gain(1,0);
  inputMix.gain(2,0);

  //read sd card, returns error if sd card is unable to be read
  SPI.setMOSI(SDCARD_MOSI_PIN);
  SPI.setSCK(SDCARD_SCK_PIN);
  if(!(SD.begin(SDCARD_CS_PIN))){
    while (1) {
      Serial.println("Unable to access the SD card");
      delay(500);
    }
  }

  //set ranges and analog pins for potentiometers
  dryWetPot.setParams(A1,1000,50);
  delayTimePot.setParams(A2,1000,50);
  fBackPot.setParams(A3,1000,50);
  modAmpPot.setParams(A0,1000,50);
  modRatePot.setParams(A5,700,50);
  filterPot.setParams(A8,75,50);

  //initialise parameters
  delay1.initParams(0);
  delay2.initParams(1);
  delay3.initParams(2);

  //turn audio chip on
  sgtl5000_1.enable();
  sgtl5000_1.inputSelect(myInput);
  sgtl5000_1.volume(0.8);
}

void changeInput(){
  //line in
  if(state2==0){
    inputMix.gain(0,1);
    inputMix.gain(1,0);
    inputMix.gain(2,0);
  }

  //sd card
  else if(state2==1){
    inputMix.gain(0,0);
    inputMix.gain(1,1);
    inputMix.gain(2,0);
  }

  //no-input mixing
  else{
    inputMix.gain(0,0);
    inputMix.gain(1,0);
    inputMix.gain(2,1);
  }
}

void toggleShift(){
  if(!state)delay1.toggleShift();
  else if(state==1)delay2.toggleShift();
  else delay3.toggleShift();
}

void loop() {
  //only plays file is sd card is chosen source
  if(state2==1)playWav.play("SDTEST4.WAV");
  delay(5); //delay to allow file to be read

  //update parameters
  while(state2!=1||playWav.isPlaying()){
    if(!state)parameters.updateParameters(delay1);
    else if(state==1)parameters.updateParameters(delay2);
    else parameters.updateParameters(delay3);

    //button for changing which delay is being changed
    if(digitalRead(buttonPin))stateWaiting=true;
    else if(stateWaiting&&!digitalRead(buttonPin)){
      state=(state+1)%3;
      stateWaiting=false;
      Serial.println(state);
    }

    //button for changing input source
    if(digitalRead(3))stateWaiting2=true;
    else if(stateWaiting2&&!digitalRead(3)){
      state2=(state2+1)%2;
      stateWaiting2=false;
      changeInput();
      Serial.print("Input source: ");
      Serial.println(state2);
    }

    //button for toggle pitch-shifting on or off
    if(digitalRead(4))stateWaiting3=true;
    else if(stateWaiting3&&!digitalRead(4)){
      stateWaiting3=false;
      toggleShift();
    }
  }
}

#ifndef MY_LEDS_H
#define MY_LEDS_H

#include <Arduino.h>

class Led {
  
  private:
    uint16_t greenPin;
    uint16_t yellowPin;
    uint16_t redPin;
    int brightnessGreen = 0;
    int brightnessYellow = 0;
    int brightnessRed = 0;
    int maxBrightness = 230;  // this is a value between 0 and 255, with 255 being maximum power
    int fadeAmount = 7; // each fading iteration, this value will be incremented up/down until the desired brightness is reached;  the higher the number, the shorter the fade duration
    int delayAmount = 10; // this is the delay in milliseconds between fading iterations; the lower the number, the shorter the fade duration
    int fadeOff(uint16_t pin, int currentBrightness);
    int fadeOn(uint16_t pin, int currentBrightness);
  
    
  public:
    Led(uint16_t greenPin, uint16_t yellowPin, uint16_t redPin, int maxBrightness = 230, int fadeAmount =7);  // set default max of 230 and fade speed of 7
    void init();
    void green();
    void yellow();
    void red();
    void off();

};

#endif

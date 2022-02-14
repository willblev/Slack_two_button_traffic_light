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
    int maxBrightness = 180;
    int fadeAmount = 5;
    int delayAmount = 10;
    
    
  public:
    Led(uint16_t greenPin, uint16_t yellowPin, uint16_t redPin, int maxBrightness, int fadeAmount);
    void init();
    void green();
    void yellow();
    void red();
    void off();
};

#endif

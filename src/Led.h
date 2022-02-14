#ifndef MY_LEDS_H
#define MY_LEDS_H

#include <Arduino.h>

class Led {
  
  private:
    uint16_t greenPin;
    uint16_t yellowPin;
    uint16_t redPin;
    
  public:
    Led(uint16_t greenPin, uint16_t yellowPin, uint16_t redPin);
    void init();
    void green();
    void yellow();
    void red();
    void off();
};

#endif

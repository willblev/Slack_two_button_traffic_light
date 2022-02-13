#ifndef MY_LEDS_H
#define MY_LEDS_H
#include <Arduino.h>


class Led {
  
  private:
    byte pin;
    
  public:
    Led(byte pin);
    void init();
    void on();
    void off();
};


#endif

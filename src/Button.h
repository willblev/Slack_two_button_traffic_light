#ifndef MY_BUTTON_H
#define MY_BUTTON_H

#include <Arduino.h>

class Button {
  
  private:
    uint16_t pin;
    uint16_t currentState;
    
    
  public:
    Button(uint16_t pin);
    void init();
    bool isPressed();
};

#endif

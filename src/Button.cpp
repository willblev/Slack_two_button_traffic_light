#include "Button.h"

/* code simplified & adapted from https://roboticsbackend.com/arduino-object-oriented-programming-oop/  
*/

Button::Button(uint16_t pin) {
  this->pin = pin;
  init();
}

void Button::init() {
  pinMode(pin, INPUT_PULLUP);
  currentState = 0;
}

bool Button::isPressed() {  // debouncing without millis from https://www.e-tinkers.com/2021/05/the-simplest-button-debounce-solution/
  currentState = (currentState<<1) | digitalRead(pin) | 0xfe00;
      return (currentState == 0xff00);
}




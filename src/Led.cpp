#include "Led.h"

Led::Led(uint16_t greenPin, uint16_t yellowPin, uint16_t redPin, int maxBrightness, int fadeAmount) {
  // Use 'this->' to make the difference between the
  // 'pin' attribute of the class and the 
  // local variable 'pin' created from the parameter.
  this->greenPin = greenPin;  
  this->yellowPin = yellowPin;  
  this->redPin = redPin;
  init();
}

void Led::init() {
  pinMode(greenPin, OUTPUT);
  pinMode(yellowPin, OUTPUT);
  pinMode(redPin, OUTPUT);
  // Always try to avoid duplicate code.
  // Instead of writing digitalWrite(pin, LOW) here,
  // call the function off() which already does that
  off();
}


// Fade off the LED by progressively decreasing the brightness of pins individually:
int Led::fadeOff(uint16_t pin, int currentBrightness) {
  if(currentBrightness > 0) {
    currentBrightness -=fadeAmount;
  
  } else if(currentBrightness < 0 ) {
      currentBrightness = 0;}
  
  analogWrite(pin, currentBrightness);
  return(currentBrightness); 
}

// Fade on the LED by progressively increasing the brightness of pins individually:
int Led::fadeOn(uint16 pin, int currentBrightness) {
  currentBrightness +=fadeAmount;
  if(currentBrightness > maxBrightness){
    currentBrightness = maxBrightness;
    }
  
  analogWrite(pin, currentBrightness);
  return(currentBrightness); 
}

void Led::off() {
  while (brightnessGreen || brightnessYellow || brightnessRed){
    brightnessGreen = fadeOff(greenPin,brightnessGreen);
    brightnessYellow = fadeOff(yellowPin,brightnessYellow);
    brightnessRed = fadeOff(redPin,brightnessRed);  
    delay(delayAmount);
  } 
}

void Led::green() {
  while (brightnessGreen < maxBrightness){
    brightnessGreen = fadeOn(greenPin,brightnessGreen);
    brightnessYellow = fadeOff(yellowPin,brightnessYellow);
    brightnessRed = fadeOff(redPin,brightnessRed);
    delay(delayAmount);
  }
}

void Led::yellow() {
  while (brightnessYellow < maxBrightness){
    brightnessGreen = fadeOff(greenPin,brightnessGreen);
    brightnessYellow = fadeOn(yellowPin,brightnessYellow);
    brightnessRed = fadeOff(redPin,brightnessRed);
    delay(delayAmount);
  }
}

void Led::red() {
  while (brightnessRed < maxBrightness){
    brightnessGreen = fadeOff(greenPin,brightnessGreen);
    brightnessYellow = fadeOff(yellowPin,brightnessYellow);
    brightnessRed = fadeOn(redPin,brightnessRed);
    delay(delayAmount);
  }
}




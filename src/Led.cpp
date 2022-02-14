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

/*
void Led::green() {
  digitalWrite(greenPin, HIGH);
  digitalWrite(yellowPin, LOW);
  digitalWrite(redPin, LOW);
}

void Led::yellow() {
  digitalWrite(greenPin, LOW);
  digitalWrite(yellowPin, HIGH);
  digitalWrite(redPin, LOW);
}

void Led::red() {
  digitalWrite(greenPin, LOW);
  digitalWrite(yellowPin, LOW);
  digitalWrite(redPin, HIGH);
}

void Led::off() {
  digitalWrite(greenPin, LOW);
  digitalWrite(yellowPin, LOW);
  digitalWrite(redPin, LOW);
}
*/

// set the brightness of pins individually:
void Led::green() {
  while (brightnessGreen < maxBrightness){
    brightnessGreen += fadeAmount;
    if(brightnessGreen > maxBrightness){brightnessGreen = maxBrightness;}
    if(brightnessYellow > 0 ){brightnessYellow -=fadeAmount;} else if(brightnessYellow < 0 ) {brightnessYellow = 0;}
    if(brightnessRed > 0){brightnessRed -=fadeAmount;} else if(brightnessRed < 0 ) {brightnessRed = 0;}
    analogWrite(greenPin, brightnessGreen); 
    analogWrite(yellowPin, brightnessYellow); 
    analogWrite(redPin, brightnessRed);
    delay(delayAmount);
  }
}

void Led::yellow() {
  while (brightnessYellow < maxBrightness){
    brightnessYellow += fadeAmount;
    if(brightnessYellow > maxBrightness){brightnessYellow = maxBrightness;}
    if(brightnessGreen){brightnessGreen -=fadeAmount;} else if(brightnessGreen < 0 ) {brightnessGreen = 0;}
    if(brightnessRed){brightnessRed -=fadeAmount;} else if(brightnessRed < 0 ) {brightnessRed = 0;}
    analogWrite(greenPin, brightnessGreen); 
    analogWrite(yellowPin, brightnessYellow); 
    analogWrite(redPin, brightnessRed);
    delay(delayAmount);
  }
}

void Led::red() {
  while (brightnessRed < maxBrightness){
    brightnessRed += fadeAmount;
    if(brightnessRed > maxBrightness){brightnessRed = maxBrightness;}
    if(brightnessYellow > 0 ){brightnessYellow -=fadeAmount;} else if(brightnessYellow < 0 ) {brightnessYellow = 0;}
    if(brightnessGreen){brightnessGreen -=fadeAmount;} else if(brightnessGreen < 0 ) {brightnessGreen = 0;}
    analogWrite(greenPin, brightnessGreen); 
    analogWrite(yellowPin, brightnessYellow); 
    analogWrite(redPin, brightnessRed);
    delay(delayAmount);
  }
}
void Led::off() {
  while (brightnessGreen || brightnessYellow || brightnessRed){
    if(brightnessGreen){brightnessGreen -=fadeAmount;} else if(brightnessGreen < 0 ) {brightnessGreen = 0;}
    if(brightnessYellow > 0 ){brightnessYellow -=fadeAmount;} else if(brightnessYellow < 0 ) {brightnessYellow = 0;}
    if(brightnessRed){brightnessRed -=fadeAmount;} else if(brightnessRed < 0 ) {brightnessRed = 0;}
    analogWrite(greenPin, brightnessGreen); 
    analogWrite(yellowPin, brightnessYellow); 
    analogWrite(redPin, brightnessRed);
    delay(delayAmount);
  }
}
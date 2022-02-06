#ifndef button_h
#define button_h

class Button
{
  private:
    uint8_t BUTTON_PIN;
    uint16_t currentState;
  public:
    void init(uint8_t button) {
      BUTTON_PIN = button;
      currentState = 0;
      pinMode(BUTTON_PIN, INPUT_PULLUP);
    }
    bool pressDetected() {
      currentState = (currentState<<1) | digitalRead(BUTTON_PIN) | 0xfe00;
      return (currentState == 0xff00);
    }
};

#endif

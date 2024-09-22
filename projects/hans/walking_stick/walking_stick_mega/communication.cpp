#ifndef COMMUNICATION_CPP
#define COMMUNICATION_CPP

#include "Arduino.h"

#include <Wire.h>

class Messenger
{
  public:
    Messenger() {
      Wire.begin();
    }

    Messenger(int pin, void (*receiveCallback)(int)) {
      Wire.begin(pin);
      Wire.onReceive(receiveCallback);
    }

    uint8_t getData() {
      if (Wire.available())
        return Wire.read();
      return 0;
    }

    void send(uint8_t data, int pin = 8) {
      Wire.beginTransmission(pin);
      Wire.write(data);
      Wire.endTransmission();
    }
};

#endif // COMMUNICATION_CPP

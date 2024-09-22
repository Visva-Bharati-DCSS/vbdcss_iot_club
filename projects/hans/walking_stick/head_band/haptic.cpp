#ifndef HAPTIC_CPP
#define HAPTIC_CPP

#include "Arduino.h"

#define HAPTIC_COUNT 5

#define MIN_STATE 400
#define MAX_STATE 4000

#define MAX_ANGLE 180
#define MIN_DISTANCE 2
#define MAX_DISTANCE 200

class HapticArray {
private:
  int pins[HAPTIC_COUNT] = {0};
  int duration = 500;

public:
  HapticArray();

  HapticArray(const int *pins) {
    setPinArray(pins);
  }

  void setPin(int index, int value) {
    if (index >= HAPTIC_COUNT) return;
    pinMode(value, OUTPUT);
    pins[index] = value;
  }

  void setPinArray(const int *array) {
    for (int i = 0; i < HAPTIC_COUNT; ++i)
      setPin(i, array[i]);
  }

  int hapticTimeout = 0;

  void vibrate(uint8_t data) {
    for (int i = 0; i < HAPTIC_COUNT; ++i)
      digitalWrite(pins[i], (data >> i) & 1);
    hapticTimeout = millis() + 500;
  }

  void stopVibration() {
    if (hapticTimeout == 0 || millis() > hapticTimeout)
      return;
    for (int i = 0; i < HAPTIC_COUNT; ++i)
      digitalWrite(pins[i], LOW);
  }
};

#endif // HAPTIC_CPP

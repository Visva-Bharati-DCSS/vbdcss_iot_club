#include "WiFi.h"
#include "esp_now.h"

#include "haptic.cpp"

#define LED_BUILTIN 2
#define STOP_VIBRATION 200

/***********************************
 * HAPTIC
 ***********************************/

const int hapticPins[] = { 27, 26, 25, 33, 32 };
HapticArray haptic(hapticPins);

/***********************************
 * ESP NOW
 ***********************************/

const uint8_t masterMac[] = { 0x24, 0xDC, 0xC3, 0x46, 0x2D, 0x04 };

bool isSameMac(const uint8_t *mac1, const uint8_t *mac2)
{
  for (int i = 0; i < 6; ++i)
    if (mac1[i] != mac2[i]) return false;
  return true;
}

void onDataRecv(const uint8_t *macAddress, const uint8_t *data, int size)
{
  if (!isSameMac(macAddress, masterMac)) return;
  handleHapticArray(*data);
}

esp_err_t initSlave()
{
  WiFi.mode(WIFI_MODE_STA);
  esp_err_t error = esp_now_init();
  esp_now_register_recv_cb(onDataRecv);
  return error;
}

void setupSlave()
{
  // initialization
  if (initSlave() != ESP_OK) {
    Serial.println("ESP Now initialization failed.");
    return;
  }
  Serial.println("Setup successful.");
}

/***********************************
 * SETUP
 ***********************************/

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  while (!Serial) delay(100);
  Serial.println("\n\n---------- New session ----------\n\n");
  setupSlave();
}

/***********************************
 * CONTROL
 ***********************************/

void handleHapticArray(uint8_t data)
{
  haptic.vibrate(data);
}

/***********************************
 * LOOP
 ***********************************/

void loop()
{
  if (millis() % STOP_VIBRATION == 0)
    haptic.stopVibration();
  delay(1);
}

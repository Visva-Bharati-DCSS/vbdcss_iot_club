#include "communication.cpp"

#include "WiFi.h"
#include "esp_now.h"

#define LED_BUILTIN 2
#define ESPNOW_CHANNEL 1

#define ROTATION_INTERVAL 10
#define DATA_HANDLING_INTERVAL 50

Messenger messenger;

/***********************************
 * ESP NOW
 ***********************************/

esp_now_peer_info_t slave;

const uint8_t slaveMac[] = { 0x08, 0xB6, 0x1F, 0xB9, 0x50, 0x10 };

// const uint8_t masterMac[] = { 0x24, 0xD7, 0xEB, 0xCD, 0x27, 0x0C };

bool isSameMac(const uint8_t *mac1, const uint8_t *mac2)
{
  for (int i = 0; i < 6; ++i)
    if (mac1[i] != mac2[i]) return false;
  return true;
}

void onDataSent(const uint8_t *macAddress, esp_now_send_status_t status)
{
  // if (isSameMac(macAddress, slaveMac))
  //   digitalWrite(LED_BUILTIN, status == ESP_NOW_SEND_SUCCESS);
}

esp_err_t initMaster()
{
  WiFi.mode(WIFI_MODE_STA);
  esp_err_t error = esp_now_init();
  esp_now_register_send_cb(onDataSent);
  return error;
}

esp_err_t pairSlave()
{
  slave.encrypt = false;
  slave.channel = ESPNOW_CHANNEL;
  memcpy(slave.peer_addr, slaveMac, sizeof(slaveMac));
  return esp_now_add_peer(&slave);
}

bool isPaired()
{
  return esp_now_is_peer_exist(slave.peer_addr);
}

esp_err_t sendHeadbandData(uint8_t data)
{
  return esp_now_send(slave.peer_addr, &data, sizeof(uint8_t));
}

void setupMaster()
{
  // initialization
  if (initMaster() != ESP_OK) {
    Serial.println("ESP Now initialization failed.");
    return;
  }
  // pairing
  if (!isPaired() && pairSlave() != ESP_OK) {
    Serial.println("Pairing with slave failed.");
    return;
  }
  Serial.println("Setup successful.");
}

/***********************************
 * SETUP
 ***********************************/

void setup()
{
  Serial.begin(115200);
  Serial2.begin(115200);
  while (!Serial) delay(10);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.println("\n\n---------- New session ----------\n\n");
  setupMaster();
}

/***********************************
 * CONTROL
 ***********************************/

void onWireReceive(int value)
{
  uint8_t data = messenger.getData();
  Serial.println(data, BIN);
  sendHeadbandData(data);
}

/***********************************
 * LOOP
 ***********************************/

void loop()
{

}

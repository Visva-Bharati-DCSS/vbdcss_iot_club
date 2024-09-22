#include "lidar180.cpp"
#include "communication.cpp"

#define SERVO_PIN_1 8
#define SERVO_PIN_2 9
#define SERVO_PIN_3 10

#define BUZZER_PIN 7

unsigned long buzzerTimeout = 0;

#define ROTATION_INTERVAL 10
#define DATA_HANDLING_INTERVAL 1000

#define HEIGHT 85
#define MAX_ANGLE 180
#define HAPTIC_COUNT 5
#define MIN_DISTANCE 2
#define MAX_DISTANCE 200
#define SECTOR_ANGLE (MAX_ANGLE/HAPTIC_COUNT)

Lidar180 lidar1, lidar2, lidar3;

LidarData data1, data2, data3;

int hapticData[HAPTIC_COUNT] = {0};

Messenger messenger;

/***********************************
 * SETUP
 ***********************************/

void setup()
{
  Serial.begin(115200);
  while (!Serial) delay(10);
  pinMode(BUZZER_PIN, OUTPUT);
  Serial.println("\n\n---------- New session ----------\n\n");
  lidar1.begin(SERVO_PIN_1, Serial1);
  lidar2.begin(SERVO_PIN_2, Serial2);
  lidar3.begin(SERVO_PIN_3, Serial3);
}

/***********************************
 * CONTROL
 ***********************************/

void rotateLidars()
{
  lidar1.rotate();
  lidar2.rotate();
  lidar3.rotate();
}

void readLidars()
{
  lidar1.dumpData(data1);
  lidar2.dumpData(data2);
  lidar3.dumpData(data3);
  set_vertical_result(data1);
  set_horizontal_result(data2);
  set_horizontal_result(data3);
}

#define GET_MAX_DISTANCE(angle) (tan(angle)*HEIGHT)

void set_vertical_result(const LidarData &data)
{
  int minAngle = atan((double)MAX_DISTANCE/HEIGHT);
  int maxAngle = 135;
  hapticData[HAPTIC_COUNT/2] = data.distance > MIN_DISTANCE && data.distance < GET_MAX_DISTANCE(data.angle) && data.angle > minAngle && data.angle < maxAngle;
}

void set_horizontal_result(const LidarData &data)
{
  int angle = data.angle - data.angle / MAX_ANGLE;            // ensuring angle is less than HAPTIC_COUNT even if data.angle >= MAX_ANGLE
  hapticData[angle/SECTOR_ANGLE] = data.distance > MIN_DISTANCE && data.distance < MAX_DISTANCE;
}

/**
 * @note
 * This function analyzes the data from 3 lidars and computes
 * the behaviour of the 5 haptic modules of the headband.
 *
 * Then the result is sent via serial communication to a ESP32
 * which then sends the data via ESPNow to the ESP32 in the
 * headband.
 *
 * The result is a 8 bit number: 001ABCDE
 * 1 ensures that the result does not start with 0.
 * A, B, C, D, E are either 0 or 1.
 * They are the signals for the 5 haptic modules respectively.
 */
void handleData()
{
  uint8_t result = 1 << 5;
  for (int i = 0; i < HAPTIC_COUNT; ++i)
    result |= hapticData[i] << i;
  Serial.println(result, BIN);
  messenger.send(result);
  Serial.println(data1.toString());
  Serial.println(data2.toString());
  Serial.println(data3.toString());
  Serial.println();
  if (result > 0) {
    digitalWrite(BUZZER_PIN, HIGH);
    buzzerTimeout = millis() + 500;
  }
}

/***********************************
 * LOOP
 ***********************************/

void loop()
{
  readLidars();
  if (millis() % DATA_HANDLING_INTERVAL == 0)
    handleData();
  if (millis() % ROTATION_INTERVAL == 0)
    rotateLidars();
  if (buzzerTimeout != 0 && millis() > buzzerTimeout) {
    digitalWrite(BUZZER_PIN, LOW);
    buzzerTimeout = 0;
  }
  delay(1);
}

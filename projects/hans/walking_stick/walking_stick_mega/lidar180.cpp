#ifndef LIDAR_180_CPP
#define LIDAR_180_CPP

#include <Servo.h>
#include <TFminiS.h>
#include "Arduino.h"
#include "HardwareSerial.h"

#define MAP_MIN_DISTANCE 2
#define MAP_MAX_DISTANCE 800
#define MAP_UNIT 50
#define MAP_UNIT_DISTANCE (MAP_MAX_DISTANCE/MAP_UNIT)

class LidarData
{
  public:
    int angle = 0;
    int distance = 0;

    LidarData() {}

    LidarData(const LidarData *data)
      : angle(data->angle), distance(data->distance) {}
    
    String toString() {
      return String(distance) + '\t' + angle;
    }

    static LidarData parseString(const String &datastr) {
      LidarData data;
      int split = datastr.indexOf('\t');
      data.distance = datastr.substring(0, split).toInt();
      data.angle = datastr.substring(split+1).toInt();
      return data;
    }
};

class Lidar180
{
  private:
    Servo servo;
    TFminiS lidar;
    int angle = 0;
    int maxAngle = 180;
    int angleDelta = 2;
    int mapMatrix[MAP_UNIT_DISTANCE][MAP_UNIT_DISTANCE] = {0};

  public:
    Lidar180() {}
    
    void setMaxAngle(int value) {
      maxAngle = value;
    }

    void setAngleDelta(int value) {
      angleDelta = value;
    }

    void begin(int servoPin, HardwareSerial &serial) {
      servo.attach(servoPin);
      serial.begin(115200);
      lidar.setStream(serial);
    }

    int getAngle() {
      return angle;
    }

    int getDistance() {
      lidar.readSensor();
      return lidar.getDistance();
    }

    void dumpData(LidarData &data) {
      data.angle = getAngle();
      data.distance = getDistance();
      int x = data.distance * cos(data.angle);
      int y = data.distance * sin(data.angle);
      mapMatrix[x%MAP_UNIT_DISTANCE][y%MAP_UNIT_DISTANCE];
    }

    void rotate() {
      if (angle+angleDelta < 0 || angle+angleDelta > maxAngle)
        angleDelta = -angleDelta;
      angle += angleDelta;
      servo.write(angle);
    }
};

#endif // LIDAR_180_CP

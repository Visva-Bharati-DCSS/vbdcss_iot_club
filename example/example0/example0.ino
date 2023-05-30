#include <DHT11.h>

#define DHT11_PIN 2

void setup()
{
  Serial.begin(115200);
}

void loop()
{
  byte data[5] = {0, 0, 0, 0, 0};

  pinMode(DHT11_PIN, OUTPUT);
  digitalWrite(DHT11_PIN, LOW);
  delay(18);
  digitalWrite(DHT11_PIN, HIGH);
  delayMicroseconds(40);
  pinMode(DHT11_PIN, INPUT);

  if (digitalRead(DHT11_PIN) == LOW)
  {
    delayMicroseconds(80);
    if (digitalRead(DHT11_PIN) == HIGH)
    {
      delayMicroseconds(80);

      for (int i = 0; i < 5; i++)
      {
        data[i] = readByte();
      }

      if (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF))
      {
        float humidity = data[0];
        float temperature = data[2];

        Serial.print("Humidity: ");
        Serial.print(humidity);
        Serial.print(" %\t");
        Serial.print("Temperature: ");
        Serial.print(temperature);
        Serial.println(" C");
      }
    }
  }
  delay(2000);
}

byte readByte()
{
  byte value = 0;

  for (int i = 0; i < 8; i++)
  {
    while (digitalRead(DHT11_PIN) == LOW)
      ;
    delayMicroseconds(30);
    if (digitalRead(DHT11_PIN) == HIGH)
    {
      value |= (1 << (7 - i));
    }
    while (digitalRead(DHT11_PIN) == HIGH)
      ;
  }
  return value;
}

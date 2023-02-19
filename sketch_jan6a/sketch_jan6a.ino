#include <EduIntro.h>

DHT11 dht11(A0);

void setup() {
  Serial.begin(9600);
}

void loop() {
  dht11.update();

  int c = dht11.readCelsius();
  int f = dht11.readFahrenheit();
  int h = dht11.readHumidity();

  Serial.println("Temperature in Celcius = " + c);
  Serial.println("Temperature in Fahrenheit = " + f);
  Serial.println("Humidity in percentage = " + h);

  delay(1000);
}

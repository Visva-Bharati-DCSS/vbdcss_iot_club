const int r1 = 0;

const int s1 = 4;
const int s2 = 16;
const int s3 = 17;

void setup()
{
  Serial.begin(9600);

  pinMode(s1, INPUT_PULLUP);
  pinMode(s2, INPUT_PULLUP);
  pinMode(s3, INPUT_PULLUP);

  pinMode(r1, OUTPUT);
}

void loop()
{
  int wl1 = LOW;
  int wl2 = LOW;
  int wl3 = LOW;

  wl1 = digitalRead(s1);
  wl2 = digitalRead(s2);
  wl3 = digitalRead(s3);

  Serial.println("Debug");
  Serial.println("Water Sensor Test");

  if (wl1 == HIGH)
    Serial.println("WL1 = HIGH");
  else
    Serial.println("WL1 = LOW");

  if (wl2 == HIGH)
    Serial.println("WL2 = HIGH");
  else
    Serial.println("WL2 = LOW");

  if (wl3 == HIGH)
    Serial.println("WL3 = HIGH");
  else
    Serial.println("WL3 = LOW");

  Serial.println("Relay Test");
  delay(5000);
  digitalWrite(r1, HIGH);
  Serial.println("Relay ON");
  delay(15000);
  digitalWrite(r1, LOW);
  Serial.println("Relay OFF");

  delay(1000);
}
const int r1 = 0;

void setup()
{
    pinMode(r1, OUTPUT);
}

void loop()
{
  digitalWrite(r1, HIGH);
  delay(5000);
  digitalWrite(r1, LOW);
  delay(10000);
}
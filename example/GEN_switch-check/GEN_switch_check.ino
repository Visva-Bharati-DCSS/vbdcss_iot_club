const int s1 = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(s1, INPUT_PULLUP);
}

void loop()
{
  int r1 = LOW;

  r1 = digitalRead(s1);

  if (r1 == HIGH)
    Serial.println("ON");
  else
    Serial.println("OFF");
}
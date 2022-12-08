const int r1 = 0;
const int fs1 = 4;
const int fs2 = 16;
const int fs3 = 17;

void setup() {
  Serial.begin(9600);
  pinMode(fs1, INPUT_PULLUP);
  pinMode(fs2, INPUT_PULLUP);
  pinMode(fs3, INPUT_PULLUP);
  pinMode(r1, OUTPUT);
}

void loop() {
  Serial.println("Debug Test");
  int wl1 = digitalRead(fs1);
  int wl2 = digitalRead(fs2);
  int wl3 = digitalRead(fs3);
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

  digitalWrite(r1, HIGH);
  Serial.println("ON");
  delay(1000);
  digitalWrite(r1, LOW);
  Serial.println("OFF");
  delay(1000);
}
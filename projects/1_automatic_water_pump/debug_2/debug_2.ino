const int re1 = 0;
const int io1 = 4;
const int io2 = 16;
const int io3 = 17;

void setup() {
  Serial.begin(9600);
  pinMode(io1, INPUT_PULLUP);
  pinMode(io2, INPUT_PULLUP);
  pinMode(io3, INPUT_PULLUP);
  pinMode(re1, OUTPUT);
}

void loop() {
  Serial.println("***********************");
  Serial.println("UPPER");
  int op1 = digitalRead(io1);
  if (op1 == HIGH) {
    Serial.println("ON");
  } else {
    Serial.println("OFF");
  }
  delay(1000);
  Serial.println("LOWER");
  int op2 = digitalRead(io2);
  if (op2 == HIGH) {
    Serial.println("ON");
  } else {
    Serial.println("OFF");
  }
  delay(1000);
  Serial.println("RESER");
  int op3 = digitalRead(io3);
  if (op3 == HIGH) {
    Serial.println("ON");
  } else {
    Serial.println("OFF");
  }
  delay(1000);
  Serial.println("SERVO");
  digitalWrite(re1, HIGH);
  Serial.println("ON");
  delay(1000);
  digitalWrite(re1, LOW);
  Serial.println("OFF");
  delay(1000);
}
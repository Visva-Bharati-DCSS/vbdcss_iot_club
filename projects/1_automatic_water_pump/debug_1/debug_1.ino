const int s1 = 0;

int st1 = LOW;


void setup() {
  Serial.begin(9600);
  pinMode(s1, INPUT_PULLUP);
}

void loop() {
  st1 = digitalRead(s1);

  if (st3 == LOW) {
    Serial.println("Reservoir Water Level Low");
    digitalWrite(r1, LOW);
    goto END;
  }

  }
END:
  delay(1000);
  Serial.println("STANDBY");
}
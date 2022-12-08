const int r1 = 0;//relay
const int s1 = 4;//tank top
const int s2 = 16;//tank bottom
const int s3 = 17;//sump

int st1 = LOW;
int st2 = LOW;
int st3 = LOW;
int st4 = LOW;

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
  st1 = digitalRead(s1); 
  st2 = digitalRead(s2);  
  st3 = digitalRead(s3);  
  st4 = digitalRead(r1);  

  if (st3 == LOW)
  {
    Serial.println("Reservoir Water Level Low");
    digitalWrite(r1, LOW);
    goto END;
  }

  if (st2 == HIGH)
  {
    Serial.println("Pump turned ON");
    digitalWrite(r1, HIGH);
    goto END;
  }
  if (st1 == LOW)
  {
    Serial.println("Pump turned OFF");
    digitalWrite(r1, LOW);
  }
END:
  delay(1000);
  Serial.println("STANDBY");
}
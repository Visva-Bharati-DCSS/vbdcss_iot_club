// Project: Automatic Pump Control
// Version: 1.0
// Copyright (C) 2022  Dhruba Saha
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// Initialize the switch pin
const int r1 = 0;  // relay
const int s1 = 4;  // tank top
const int s2 = 16; // tank bottom
const int s3 = 17; // sump

// Initialize the variables to store the switch status
int st1 = LOW;
int st2 = LOW;
int st3 = LOW;
int st4 = LOW;

// Setup function
void setup()
{
  // Initialize the serial monitor
  Serial.begin(9600);
  // initialize the pins as input/output
  pinMode(s1, INPUT_PULLUP);
  pinMode(s2, INPUT_PULLUP);
  pinMode(s3, INPUT_PULLUP);
  pinMode(r1, OUTPUT);
}

void loop()
{
  // Read the switch status
  st1 = digitalRead(s1);
  st2 = digitalRead(s2);
  st3 = digitalRead(s3);
  st4 = digitalRead(r1);

  // Check the combination of sensor status 
  if (st3 == LOW)
  {
    Serial.println("Reservoir Water Level Low");
    digitalWrite(r1, LOW); // turn off the pump
    goto END;
  }

  if (st2 == HIGH)
  {
    Serial.println("Pump turned ON");
    digitalWrite(r1, HIGH); // turn on the pump
    goto END;
  }
  if (st1 == LOW)
  {
    Serial.println("Pump turned OFF");  // turn off the pump
    digitalWrite(r1, LOW);
  }
END:
  delay(1000);
  Serial.println("STANDBY");
}
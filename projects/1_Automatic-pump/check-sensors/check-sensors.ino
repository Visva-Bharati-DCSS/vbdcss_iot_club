// Check Sensors Data for Autmatic Pump
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
const int r1 = 0;

const int s1 = 4;
const int s2 = 16;
const int s3 = 17;

void setup() // Setup function
{
  Serial.begin(115200); // Initialize the serial monitor

  // initialize the pins as inpu/output
  pinMode(s1, INPUT_PULLUP);
  pinMode(s2, INPUT_PULLUP);
  pinMode(s3, INPUT_PULLUP);

  pinMode(r1, OUTPUT);
}

void loop() // Main loop
{
  // initialize the variables to store the switch status
  int wl1 = LOW;
  int wl2 = LOW;
  int wl3 = LOW;

  // Read the switch status
  wl1 = digitalRead(s1);
  wl2 = digitalRead(s2);
  wl3 = digitalRead(s3);

  Serial.println("Debug");
  Serial.println("Water Sensor Test");
  // Check the switch status
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
  digitalWrite(r1, HIGH); // Turn ON the relay
  Serial.println("Relay ON");
  delay(15000);
  digitalWrite(r1, LOW); // Turn OFF the relay
  Serial.println("Relay OFF");

  delay(1000); // Wait for 1 second
}
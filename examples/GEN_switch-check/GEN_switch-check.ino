// Check the status of a switch and print it on the serial monitor
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
const int s1 = 0;

void setup()
{
  Serial.begin(115200);        // Initialize the serial monitor
  pinMode(s1, INPUT_PULLUP);   // Set the switch pin as input
}

void loop()
{
  int r1 = LOW; // Initialize the variable to store the switch status

  r1 = digitalRead(s1); // Read the switch status

  if (r1 == HIGH) // Check the switch status
    Serial.println("ON");
  else
    Serial.println("OFF");
}
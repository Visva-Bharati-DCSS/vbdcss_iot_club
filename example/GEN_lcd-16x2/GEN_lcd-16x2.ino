// 16x2 LCD Display Example
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

#include <LiquidCrystal.h> // Include the LiquidCrystal library

// Initialize the LCD pins
const int rs = 21;
const int en = 19;
const int d4 = 18;
const int d5 = 5;
const int d6 = 17;
const int d7 = 16;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7); // Create a LiquidCrystal object

void setup()
{
  lcd.begin(16, 2);           // Initialize the LCD
  lcd.print("hello, world!"); // Print a message to the LCD
}

void loop()
{
  lcd.setCursor(0, 1);        // Set the cursor to column 0, line 1
  lcd.print(millis() / 1000); // Print the number of seconds since the Arduino board began running the current program
}
// Project: Automatic Pump Control
// Version: 2.0
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

// include required libraries
#include <LiquidCrystal.h>

// initialize the LCD pins
const int rs = 21;
const int en = 19;
const int d4 = 18;
const int d5 = 5;
const int d6 = 17;
const int d7 = 16;

// create a LiquidCrystal object
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// define custom characters
byte pump_full[8] = {B01110, B11111, B11111, B11111, B11111, B11111, B11111, B11111};
byte pump_half[8] = {B01110, B10001, B10001, B10001, B11111, B11111, B11111, B11111};
byte pump_empty[8] = {B01110, B10001, B10001, B10001, B10001, B10001, B10001, B11111};
byte status_tick[8] = {B00000, B00000, B00001, B00011, B10110, B11100, B01000, B00000};
byte status_cross[8] = {B00000, B00000, B10001, B11011, B01110, B11011, B10001, B00000};
byte status_blink[8] = {B11111, B11111, B11111, B11111, B11111, B11111, B11111, B11111};
byte status_blank[8] = {B00000, B00000, B00000, B00000, B00000, B00000, B00000, B00000};
byte status_heart[8] = {B00000, B01010, B11111, B11111, B01110, B00100, B00000, B00000};

// define sensor pins
const int r1 = 0;  // relay
const int s1 = 15; // tank top
const int s2 = 2;  // tank bottom
const int s3 = 4;  // reservoir

void setup()
{

  // initialize the serial port
  Serial.begin(9600);
  // initialize the LCD
  lcd.begin(16, 2);

  // initialize the sensor pins as input/output
  pinMode(r1, OUTPUT);
  pinMode(s1, INPUT_PULLUP);
  pinMode(s2, INPUT_PULLUP);
  pinMode(s3, INPUT_PULLUP);

  // create custom characters
  lcd.createChar(0, pump_full);
  lcd.createChar(1, pump_half);
  lcd.createChar(2, pump_empty);
  lcd.createChar(3, status_tick);
  lcd.createChar(4, status_cross);
  lcd.createChar(5, status_blink);
  lcd.createChar(6, status_heart);
  lcd.createChar(7, status_blank);

  // display the splash screen
  Serial.println("DCSS IoT Club");
  lcd.setCursor(0, 0);
  lcd.print(" VBDCSS IoT Club");
  delay(250);
  Serial.println("Automatic Pump");
  lcd.setCursor(0, 1);
  lcd.print(" Automatic Pump");
  delay(500);
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Tank");
  lcd.setCursor(5, 0);
  lcd.write(byte(5));

  lcd.setCursor(10, 0);
  lcd.print("Pump");
  lcd.setCursor(15, 0);
  lcd.write(byte(5));

  lcd.setCursor(0, 1);
  lcd.print("Reservoir");
  lcd.setCursor(10, 1);
  lcd.write(byte(5));

  lcd.setCursor(15, 1);
  lcd.write(byte(5));

  delay(5000);
}

void loop()
{
  // intialize the status variables
  int st1 = LOW;
  int st2 = LOW;
  int st3 = LOW;
  int st4 = LOW;

  // read the sensor status
  st1 = digitalRead(s1);
  st2 = digitalRead(s2);
  st3 = digitalRead(s3);
  st4 = digitalRead(r1);

  // condition check
  if (st3 == LOW)
  {
    Serial.println("Reservoir Water Level Low");
    digitalWrite(r1, LOW);
    lcd.setCursor(10, 1);
    lcd.write(byte(5));
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
    lcd.setCursor(15, 1);
    lcd.write(byte(5));
    digitalWrite(r1, LOW);
  }

END:

  delay(1000);
}
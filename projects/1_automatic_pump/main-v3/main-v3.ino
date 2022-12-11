#include <LiquidCrystal.h>

const int rs = 21;
const int en = 19;
const int d4 = 18;
const int d5 = 5;
const int d6 = 17;
const int d7 = 16;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

byte pump_full[8] = {B01110, B11111, B11111, B11111, B11111, B11111, B11111, B11111};
byte pump_half[8] = {B01110, B10001, B10001, B10001, B11111, B11111, B11111, B11111};
byte pump_empty[8] = {B01110, B10001, B10001, B10001, B10001, B10001, B10001, B11111};
byte status_tick[8] = {B00000, B00000, B00001, B00011, B10110, B11100, B01000, B00000};
byte status_cross[8] = {B00000, B00000, B10001, B11011, B01110, B11011, B10001, B00000};
byte status_blink[8] = {B11111, B11111, B11111, B11111, B11111, B11111, B11111, B11111};
byte status_blank[8] = {B00000, B00000, B00000, B00000, B00000, B00000, B00000, B00000};
byte status_heart[8] = {B00000, B01010, B11111, B11111, B01110, B00100, B00000, B00000};

const int r1 = 0;  // relay
const int s1 = 15; // tank top
const int s2 = 2;  // tank bottom
const int s3 = 4;  // reservoir

void setup()
{
  Serial.begin(9600);
  lcd.begin(16, 2);

  pinMode(r1, OUTPUT);
  pinMode(s1, INPUT_PULLUP);
  pinMode(s2, INPUT_PULLUP);
  pinMode(s3, INPUT_PULLUP);

  lcd.createChar(0, pump_full);
  lcd.createChar(1, pump_half);
  lcd.createChar(2, pump_empty);
  lcd.createChar(3, status_tick);
  lcd.createChar(4, status_cross);
  lcd.createChar(5, status_blink);
  lcd.createChar(6, status_heart);
  lcd.createChar(7, status_blank);

  startup();
  lcd_setup();
}

void loop()
{
  int st1 = LOW;
  int st2 = LOW;
  int st3 = LOW;
  int st4 = LOW;

  st1 = digitalRead(s1);
  st2 = digitalRead(s2);
  st3 = digitalRead(s3);
  st4 = digitalRead(r1);

  status(st1, st2, st3);

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
  lcd.setCursor(15, 1);
  lcd.write(byte(7));
  delay(1000);
}

void startup()
{
  lcd.setCursor(0, 0);
  lcd.print(" VBDCSS IoT Club");
  delay(250);
  lcd.setCursor(0, 1);
  lcd.print(" Automatic Pump");
  delay(2000);
  lcd.clear();
}

void lcd_setup()
{
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
}

void status(int s1, int s2, int s3)
{
  if (s1 == HIGH)
  {
    lcd.setCursor(5, 0);
    lcd.write(byte(0));
  }
  else if (s1 == LOW && s2 == HIGH)
  {
    lcd.setCursor(5, 0);
    lcd.write(byte(1));
  }
  else
  {
    lcd.setCursor(5, 0);
    lcd.write(byte(2));
  }

  if (r1 == HIGH)
  {
    lcd.setCursor(15, 0);
    lcd.write(byte(3));
  }
  else
  {
    lcd.setCursor(15, 0);
    lcd.write(byte(4));
  }

  if (s3 == HIGH)
  {
    lcd.setCursor(10, 1);
    lcd.write(byte(3));
  }
  else
  {
    lcd.setCursor(10, 1);
    lcd.write(byte(4));
  }

  lcd.setCursor(15, 1);
  lcd.write(byte(6));
}
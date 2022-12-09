#include <LiquidCrystal.h>
#include <WiFi.h>

const char *ssid = "No Internet LAB";
const char *psk = "87654321";

WiFiServer server(80);

String header;

String pump = "off";
String tank_top = "off";
String tank_bottom = "off";
String reservoir = "off";

unsigned long currentTime = millis();
unsigned long previousTime = 0;
const long timeoutTime = 2000;

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

  WiFi.begin(ssid, psk);
  Serial.print("Connecting to");
  Serial.println(ssid);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();

  lcd_startup();
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
  web_server(st1, st2, st3, st4);

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

void lcd_startup()
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

void web_server(int s1, int s2, int s3, int s4)
{
  WiFiClient client = server.available();
  if (!client)
  {
    return;
  }

  Serial.println("New Client.");
  while (!client.available())
  {
    delay(1);
  }

  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();

  int val = req.indexOf("/?pin=");
  if (val != -1)
  {
    int pin = req.substring(val + 6, val + 7).toInt();
    int state = req.substring(val + 8, val + 9).toInt();
    Serial.print("Pin: ");
    Serial.print(pin);
    Serial.print(" State: ");
    Serial.println(state);

    if (pin == 1)
    {
      digitalWrite(r1, state);
    }
  }

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");

  client.println("<!DOCTYPE HTML>");
  client.println("<html>");

  client.println("<head>");
  client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
  client.println("<link rel=\"icon\" href=\"data:,\">");
  client.println("<style>");
  client.println("html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
  client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
  client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
  client.println(".button2 {background-color: #77878A;}");
  client.println("</style>");
  client.println("</head>");

  client.println("<body>");
  client.println("<h1>VBDCSS IoT Club</h1>");
  client.println("<h2>Automatic Pump</h2>");

  client.println("<p>Tank: ");
  if (s1 == HIGH)
  {
    client.println("Full");
  }
  else if (s1 == LOW && s2 == HIGH)
  {
    client.println("Half");
  }
  else
  {
    client.println("Empty");
  }
  client.println("</p>");

  client.println("<p>Reservoir: ");
  if (s3 == HIGH)
  {
    client.println("Full");
  }
  else
  {
    client.println("Empty");
  }
  client.println("</p>");

  client.println("<p>Pump: ");
  if (s2 == HIGH)
  {
    client.println("ON");
  }
  else
  {
    client.println("OFF");
  }
  client.println("</p>");

  client.println("<p>Reservoir Water Level: ");
  if (s4 == HIGH)
  {
    client.println("Low");
  }
  else
  {
    client.println("Normal");
  }

  client.println("</p>");

  if (s2 == HIGH)
  {
    client.println("<p><a href=\"/?pin=1&state=0\"><button class=\"button button2\">OFF</button></a></p>");
  }
  else
  {
    client.println("<p><a href=\"/?pin=1&state=1\"><button class=\"button\">ON</button></a></p>");
  }

  client.println("</body>");
  client.println("</html>");

  delay(1);
  Serial.println("Client Disconnected.");
}
#include <ESP8266WiFi.h>
#include <ESPMDNS>
#include <ESP8266WebServer.h>
#include <LiquidCrystal_I2C.h>

// wifi details

const String ssid         = "VBDCSS IoT Club LAB Network";
const String password     = "X6zuT7m4yp";
// const String ssid_dev     = "Galaxy M12 250A";
// const String password_dev = "oyxe8115";
const String ssid_dev     = "MoE Wi-Fi";
const String password_dev = "";

const String server_alias = "vbdcss_pump.iot";

// timeout

const unsigned long show_ip_timeout       = 8000;
const unsigned long show_failure_timeout  = 4000;
const unsigned long server_timeout        = 2000;
const unsigned long connection_timeout    = 5000;

// pins

const uint8_t relay     = 15;
const uint8_t t_top     = 14;
const uint8_t t_bottom  = 12;
const uint8_t r_bottom  = 13;

// lcd

const uint8_t rows = 4;
const uint8_t cols = 20;

LiquidCrystal_I2C lcd(cols, rows);

// webserver

ESP8266WebServer server(80);

// previous wifi status

wl_status_t last_wifi_status = WiFi.status();

// emoji

uint8_t tank_empty[]      = { B00000, B01110, B10001, B10001, B10001, B10001, B11111, B00000 };
uint8_t tank_full[]       = { B00000, B01110, B11111, B11111, B11111, B11111, B11111, B00000 };
uint8_t tank_bot_half[]   = { B00000, B01110, B10001, B10001, B11111, B11111, B11111, B00000 };
uint8_t tank_top_half[]   = { B00000, B01110, B11111, B11111, B10001, B10001, B11111, B00000 };
uint8_t pump_off[]        = { B00000, B01110, B10001, B10001, B10001, B01010, B01110, B00000 };
uint8_t pump_on[]         = { B00000, B01110, B11111, B11111, B11111, B01010, B01110, B00000 };
uint8_t disconnected[]    = { B10100, B01000, B10101, B00001, B00101, B00101, B10101, B00000 };
uint8_t connected[]       = { B00000, B00001, B00001, B00101, B00101, B10101, B10101, B00000 };

// sensors

bool is_high(uint8_t pin)
{
  switch (pin) {
    case t_top:
    case t_bottom:
    case r_bottom:
      return digitalRead(pin) == LOW;
    default:
      return digitalRead(pin) == HIGH;
  }
}

/**
 * @return
 * Returns 0 if tank is empty.
 * Returns 1 if tank is full.
 * Returns 2 if tank is half full.
 * Returns 3 if tank is inverted half full.
 */
uint8_t get_tank_state()
{
  if (is_high(t_top) && is_high(t_bottom))
    return 1;
  else if (is_high(t_bottom))
    return 2;
  else if (is_high(t_top))
    return 3;
  return 0;
}

/**
 * @return
 * Returns 0 if reservoir is empty.
 * Returns 1 if reservoir is full.
 */
uint8_t get_reservoir_state()
{
  if (is_high(r_bottom))
    return 1;
  return 0;
}

/**
 * @return
 * Returns 0 if relay is off.
 * Returns 1 if relay is on.
 */
uint8_t get_relay_state()
{
  if (is_high(relay))
    return 1;
  return 0;
}

// utility

void print_status()
{
  lcd.printLine("  Tank", 2);
  lcd.printLine("  Reservoir", 3);
}

void update_status()
{
  lcd.setCursor(cols-3, 2);
  lcd.write(get_tank_state());
  lcd.setCursor(cols-3, 3);
  lcd.write(get_reservoir_state());
  lcd.setCursor(cols-2, 0);
  lcd.write(get_relay_state()+4);
}

// setup lcd

void setup_lcd()
{
  uint8_t address = get_i2c_address();
  Serial.println(String("Found I2C device at ") + address + ".");
  lcd.setAddress(address);
  lcd.init();
  lcd.clear();
  lcd.setCursor(6, 0);
  lcd.print("IoT Club");
  print_status();
  update_status();
  // custom characters
  lcd.createChar(0, tank_empty);
  lcd.createChar(1, tank_full);
  lcd.createChar(2, tank_bot_half);
  lcd.createChar(3, tank_top_half);
  lcd.createChar(4, pump_off);
  lcd.createChar(5, pump_on);
  lcd.createChar(6, disconnected);
  lcd.createChar(7, connected);
}

void handle_root()
{
  if (server.method() != HTTP_GET) return;
  
}

void handle_auth()
{
  if (server.method() != HTTP_POST) return;
}

void setup_server()
{
  if (MDNS.begin(server_alias))
    Serial.println(String("Alias: ") + "http://" + server_alias);
  server.on("/", handle_root);
  server.on("/auth", handle_auth);
  server.onNotFound(handle_root);
  server.begin();
}

void check_connection()
{
  // wifi status changed
  unsigned long display_timeout = 0;
  if (last_wifi_status != WL_CONNECTED && WiFi.status() == WL_CONNECTED) {
    lcd.printLine(WiFi.SSID(), 2);
    lcd.printLine(String("IP: ") + WiFi.localIP().toString(), 3);
    Serial.println(String("Connected to ") + WiFi.SSID() + ".");
    Serial.println(String("IP: ") + WiFi.localIP().toString());
    display_timeout = show_ip_timeout;
    setup_server();
  }
  else if (last_wifi_status == WL_CONNECTED && WiFi.status() != WL_CONNECTED) {
    lcd.printLine("Disconnected.", 2);
    lcd.clearLine(3);
    Serial.println("Disconnected.");
    display_timeout = show_failure_timeout;
  }
  else return;
  last_wifi_status = WiFi.status();
  lcd.setCursor(1, 0);
  lcd.write((last_wifi_status == WL_CONNECTED)+6);
  delay(display_timeout);
  print_status();
  update_status();
}

// setup

void setup()
{
  // pins
  pinMode(relay, OUTPUT);
  pinMode(t_top, INPUT_PULLUP);
  pinMode(t_bottom, INPUT_PULLUP);
  pinMode(r_bottom, INPUT_PULLUP);
  // serial
  Serial.begin(74880);
  Serial.println("\n\n---------- New session ----------\n");
  // lcd
  setup_lcd();
  // wifi
  lcd.setCursor(1, 0);
  lcd.write((last_wifi_status == WL_CONNECTED)+6);
  WiFi.begin(ssid_dev, password_dev);
}

// handle pump

void handle_pump()
{
  if (get_reservoir_state() == 0)
    digitalWrite(relay, LOW);
  else if (get_tank_state() == 0 && get_relay_state() == 0)
    digitalWrite(relay, HIGH);
  else if (get_tank_state() == 1 && get_relay_state() == 1)
    digitalWrite(relay, LOW);
  update_status();
}

// handle client

String get_client_line(WiFiClient client)
{
  String line;
  char ch;
  unsigned long start = millis();
  while (client.available() && millis() - start <= server_timeout && (ch = client.read()) != '\n')
    if (ch != '\r') line += ch;
  return line;
}


// loop

void loop()
{
  if (millis() % 1000 == 0) {
    handle_pump();
    check_connection();
  }
  else if (millis() % 100 == 0)
    if (WiFi.status() == WL_CONNECTED) server.handleClient();
  delay(1);
}

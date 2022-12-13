// Project: Automatic Pump Control
// Version: 5.0
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

// Required Libraries
#include <LiquidCrystal.h>
#include <Arduino.h>
#include <WiFi.h>

// SSID and Password of the WiFi router
const char *ssid = "ROG-G15 1780";
const char *password = "4B67r500";

// set web server port
WiFiServer server(80);

// variable to store the HTTP request
String header;

// time management
unsigned long currentTime = millis();
unsigned long previousTime = 0;
const long timeoutTime = 1000;

// variables to store board information
String ft;
int flash_size;
String fft;
String flash_fq;
String chipId;
String chip_id;
int silicon_revision;
String sdk_version;
int core_c;

// lcd pin configuration
const int rs = 21;
const int en = 19;
const int d4 = 18;
const int d5 = 5;
const int d6 = 17;
const int d7 = 16;

// lcd object
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// lcd custom character array
byte pump_full[8] = {B01110, B11111, B11111, B11111, B11111, B11111, B11111, B11111};
byte pump_half[8] = {B01110, B10001, B10001, B10001, B11111, B11111, B11111, B11111};
byte pump_empty[8] = {B01110, B10001, B10001, B10001, B10001, B10001, B10001, B11111};
byte status_tick[8] = {B00000, B00000, B00001, B00011, B10110, B11100, B01000, B00000};
byte status_cross[8] = {B00000, B00000, B10001, B11011, B01110, B11011, B10001, B00000};
byte status_blink[8] = {B11111, B11111, B11111, B11111, B11111, B11111, B11111, B11111};
byte status_blank[8] = {B00000, B00000, B00000, B00000, B00000, B00000, B00000, B00000};
byte status_heart[8] = {B00000, B01010, B11111, B11111, B01110, B00100, B00000, B00000};

// sensor pin configuration
const int r1 = 0;  // relay
const int s1 = 15; // tank top
const int s2 = 2;  // tank bottom
const int s3 = 4;  // reservoir

// required deault setup function
void setup()
{
  Serial.begin(115200);

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

  lcd_startup();
  lcd_setup();

  web_setup();

  system_info();

  multi_core();
}

// required default loop function
void loop()
{
}


//initialize dual core setup
void multi_core()
{
  xTaskCreatePinnedToCore(core0, "pump_control", 10000, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(core1, "web_server", 10000, NULL, 1, NULL, 1);
}

//core0 function
void core0(void *pvParameters)
{
  for (;;)
  {
    pump_control();
  }
}

//core1 function
void core1(void *pvParameters)
{
  for (;;)
  {
    web_server();
  }
}


// function to get system information
void system_info()
{
  esp_chip_info_t chip_info;
  esp_chip_info(&chip_info);
  String chip_model = String(ESP.getChipModel());
  core_c = chip_info.cores;
  String ft1 = "WiFi";
  String ft2 = " ";
  String ft3 = (chip_info.features & CHIP_FEATURE_BT) ? "BT" : "";
  String ft4 = (chip_info.features & CHIP_FEATURE_BLE) ? "BLE" : "";
  ft = ft1 + ft2 + ft3 + ft2 + ft4;
  flash_size = (spi_flash_get_chip_size() / (1024 * 1024));
  fft = (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "Embeded" : "External";
  flash_fq = String(ESP.getFlashChipSpeed() / (1024 * 1024)) + " MHz";
  chipId = String((uint32_t)ESP.getEfuseMac(), HEX);
  chipId.toUpperCase();
  chip_id = chipId.c_str();
  silicon_revision = chip_info.revision;
  sdk_version = String(ESP.getSdkVersion());
}

// function to display the LCD startup
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

// function to display the LCD setup
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

void pump_control()
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

  // calling the status function to display the status
  status(st1, st2, st3);

  // condition check
  if (st3 == LOW)
  {
    Serial.println("Reservoir Water Level Low");
    digitalWrite(r1, LOW);
    goto END;
  }

  // condition check
  if (st2 == HIGH)
  {
    Serial.println("Pump turned ON");
    digitalWrite(r1, HIGH);
    goto END;
  }

  // condition check
  if (st1 == LOW)
  {
    Serial.println("Pump turned OFF");
    digitalWrite(r1, LOW);
  }

END:
  delay(1000);
  // set the cursor to column 15, line 1
  lcd.setCursor(15, 1);
  // display the status heart
  lcd.write(byte(7));
  delay(5000);
}

// function to display/update the status
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

void web_setup()
{
  // connect to WiFi network
  WiFi.begin(ssid, password);
  Serial.print("Connecting to ");
  Serial.println(ssid);

  while (currentTime == 30000)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connection successful.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void web_server()
{
  // listen for clients
  WiFiClient client = server.available();

  // new client connection
  if (client)
  {
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");
    // store HTTP request
    String currentLine = "";
    // while client is connected
    while (client.connected() && currentTime - previousTime <= timeoutTime)
    {
      currentTime = millis();
      // read data from client
      if (client.available())
      {
        char c = client.read();
        Serial.write(c);
        header += c;
        if (c == '\n')
        {
          // if the current line is blank, you got two newline characters in a row.the request is complete, so send a response
          if (currentLine.length() == 0)
          {
            // send HTTP headers(response) & content type & blank line
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // send HTML content
            client.println("<!doctype html><html lang=\"en\">");
            client.println("<head><meta charset=\"utf-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"><link href=\"https://cdn.jsdelivr.net/npm/bootstrap@5.0.2/dist/css/bootstrap.min.css\" rel=\"stylesheet\" integrity=\"sha384-EVSTQN3/azprG1Anm3QDgpJLIm9Nao0Yz1ztcQTwFspd3yD65VohhpuuCOmLASjC\" crossorigin=\"anonymous\"><title>VBDCSS IoT Club</title></head>");
            client.println("<body><header><div style=\"text-align: center;\">");
            client.println("<img src=\"https://www.dropbox.com/s/uvfdfjr69et5utm/vbdcss_iot_club_logo.png?dl=1\" alt=\"vb logo\" height=\"100\" width=\"100\"></div>");
            client.println("<h1 class=\"text-secondary\" style=\"text-align: center;\">VBDCSS IoT Club</h1><h4 class=\"text-secondary\" style=\"text-align: center; text-decoration: underline;\">Automatic Water Pump</h4>");
            client.println("</header><main><hr size=\"5\"><h5 class=\"text-info\">Tank Status</h5>");
            if (s1 & s2 == LOW)
            {
              client.println("Tank : <span id=\"tank\" class=\"text-danger\">Empty</span>");
            }
            else if (s1 == LOW && s2 == HIGH)
            {
              client.println("Tank : <span id=\"tank\" class=\"text-warning\">Half</span>");
            }
            else
            {
              client.println("Tank : <span id=\"tank\" class=\"text-success\">Full</span>");
            }
            client.println("<br><hr size=\"3\"><h5 class=\"text-info\">Pump Status</h5>");
            if (r1 == LOW)
            {
              client.println("Pump : <span class=\"text-danger\">OFF</span>");
            }
            else
            {
              client.println("Pump : <span class=\"text-success\">ON</span>");
            }
            client.println("<br><hr size=\"3\"><h5 class=\"text-info\">Sensor Status</h5>");
            if (s1 == LOW)
            {
              client.println("Tank Top Sensor : <span class=\"text-danger\">OFF</span>");
            }
            else
            {
              client.println("Tank Top Sensor : <span class=\"text-success\">ON</span>");
            }
            client.println("<br>");
            if (s2 == LOW)
            {
              client.println("Tank Bottom Sensor : <span class=\"text-danger\">OFF</span>");
            }
            else
            {
              client.println("Tank Bottom Sensor : <span class=\"text-success\">ON</span>");
            }
            client.println("<br>");
            if (s3 == LOW)
            {
              client.println("Reservoir Sensor : <span class=\"text-danger\">OFF</span>");
            }
            else
            {
              client.println("Reservoir Sensor : <span class=\"text-success\">ON</span>");
            }
            client.println("<br><hr size=\"3\"><h5 class=\"text-info\">Board Info</h5>");
            client.println("Current Running Time: <span class=\"text-muted\">" + String((currentTime) / 1000) + " Seconds</span>");
            client.println("<br>");
            client.println("CPU Freqency: <span class=\"text-muted\">" + String(ESP.getCpuFreqMHz()) + " MHz</span>");
            client.println("<br>");
            client.println("Free Heap: <span class=\"text-muted\">" + String(ESP.getFreeHeap() / 1024) + " KB</span>");
            client.println("<br>");
            client.println("Sketch Size: <span class=\"text-muted\">" + String(ESP.getSketchSize() / 1024) + " KB</span>");
            client.println("<br>");
            client.println("Sketch MD5: <span class=\"text-muted\">" + String(ESP.getSketchMD5()) + "</span>");
            client.println("<br>");
            client.println("Free Sketch Space:  <span class=\"text-muted\">" + String(ESP.getFreeSketchSpace() / 1024) + " KB</span>");
            client.println("<br>");
            client.println("Chip Model: <span class=\"text-muted\">" + String(ESP.getChipModel()) + "</span>");
            client.println("<br>");
            client.println("Chip Core Count: <span class=\"text-muted\">" + String(core_c) + "</span>");
            client.println("<br>");
            client.println("Chip Connectivity: <span class=\"text-muted\">" + ft + "</span>");
            client.println("<br>");
            client.println("Flash Size: <span class=\"text-muted\">" + String(flash_size) + " MB</span>");
            client.println("<br>");
            client.println("Flash Type: <span class=\"text-muted\">" + fft + "</span>");
            client.println("<br>");
            client.println("Flash Frequency: <span class=\"text-muted\">" + flash_fq + "</span>");
            client.println("<br>");
            client.println("Chip ID: <span class=\"text-muted\">" + chip_id + "</span>");
            client.println("<br>");
            client.println("Silicon Revision: <span class=\"text-muted\">" + String(silicon_revision) + "</span>");
            client.println("<br>");
            client.println("SDK Version: <span class=\"text-muted\">" + sdk_version + "</span>");
            client.println("<br><hr size=\"5\"></main>");
            client.println("<footer><p style=\"text-align: center;\">&copy; 2022 VBDCSS IoT Club by <a href=\"https://github.com/dhrubasaha08\">Dhruba Saha</a></p></footer>");
            client.println("<script src=\"https://cdn.jsdelivr.net/npm/bootstrap@5.0.2/dist/js/bootstrap.bundle.min.js\" integrity=\"sha384-MrcW6ZMFYlzcLA8Nl+NtUVF0sA7MsXsP1UyJoMp4YLEuNSfAP+JcXn/tWtIaxVXM\" crossorigin=\"anonymous\"></script>");
            client.println("</body></html>");
            // HTML content ends here
            client.println(); // end of HTTP response
            break;
          }
          else
          {
            currentLine = ""; // if you got a newline, then clear currentLine
          }
        }
        else if (c != '\r') // if you got anything else but a carriage return character, add it to the end of the currentLine
        {
          currentLine += c; // if you got anything else but a carriage return character, add it to the end of the currentLine
        }
      }
    }

    // clear the HTTP request
    header = "";
    // close connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
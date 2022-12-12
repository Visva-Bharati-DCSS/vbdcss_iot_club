// Static Webpage for ESP32
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

// network credentials
const char *ssid = "No Internet Mesh";    // Enter SSID here
const char *password = "hcWCpHt9hJu6EsyQrUHRxvRESVux8qNKPTRMEzspgyFkhC7WeCyr6G2S3w"; // Enter Password here

// required libraries
#include <WiFi.h>

// set web server port
WiFiServer server(80);

// HTTP request
String header;

// time settings
unsigned long currentTime = millis();
unsigned long previousTime = 0;
const long timeoutTime = 1000;

void setup()
{
  // initialize serial monitor
  Serial.begin(115200);

  // connect to WiFi network
  WiFi.begin(ssid, password);
  Serial.print("Connecting to ");
  Serial.println(ssid);

  while (WiFi.status() != WL_CONNECTED)
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

void loop()
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
            client.println("Tank : <span id=\"tank\" class=\"text-danger\">Empty</span>");
            client.println("Tank : <span id=\"tank\" class=\"text-warning\">Half</span>");
            client.println("Tank : <span id=\"tank\" class=\"text-success\">Full</span>");
            client.println("<br><hr size=\"3\"><h5 class=\"text-info\">Pump Status</h5>");
            client.println("Pump : <span class=\"text-danger\">OFF</span>");
            client.println("Pump : <span class=\"text-success\">ON</span>");
            client.println("<br><hr size=\"3\"><h5 class=\"text-info\">Sensor Status</h5>");
            client.println("Tank Top Sensor : <span class=\"text-danger\">OFF</span>");
            client.println("Tank Top Sensor : <span class=\"text-success\">ON</span>");
            client.println("<br>");
            client.println("Tank Bottom Sensor : <span class=\"text-danger\">OFF</span>");
            client.println("Tank Bottom Sensor : <span class=\"text-success\">ON</span>");
            client.println("<br>");
            client.println("Reservoir Sensor : <span class=\"text-danger\">OFF</span>");
            client.println("Reservoir Sensor : <span class=\"text-success\">ON</span>");
            client.println("<br><hr size=\"3\"><h5 class=\"text-info\">Board Info</h5>");
            client.println("Current Running Time: <span class=\"text-muted\">var</span>");
            client.println("<br>");
            client.println("CPU Temperature : <span class=\"text-muted\">var</span>");
            client.println("<br>");
            client.println("CPU Usage: <span class=\"text-muted\">var</span>");
            client.println("<br>");
            client.println("CPU 0 Usage: <span class=\"text-muted\">var</span>");
            client.println("<br>");
            client.println("CPU 1 Usage: <span class=\"text-muted\">var</span>");
            client.println("<br>");
            client.println("RAM Usage: <span class=\"text-muted\">var</span>");
            client.println("<br>");
            client.println("Chip ID: <span class=\"text-muted\">var</span>");
            client.println("<br>");
            client.println("Chip Type : <span class=\"text-muted\">var</span>");
            client.println("<br>");
            client.println("Silicon Revision : <span class=\"text-muted\">var</span>");
            client.println("<br>");
            client.println("External Flash Size : <span class=\"text-muted\">var</span>");
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
#include <WiFi.h>

const char *ssid = "SSID";
const char *password = "PSK";

WiFiServer server(80);
String header;

unsigned long currentTime = millis();
unsigned long previousTime = 0;
const long timeoutTime = 2000;

void setup()
{
  Serial.begin(115200);

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop()
{
  WiFiClient client = server.available();

  if (client)
  {
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");
    String currentLine = "";
    while (client.connected() && currentTime - previousTime <= timeoutTime)
    {
      currentTime = millis();
      if (client.available())
      {
        char c = client.read();
        Serial.write(c);
        header += c;
        if (c == '\n')
        {
          if (currentLine.length() == 0)
          {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

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
            client.println("<footer><p style=\"text-align: center;\">&copy; 2022 VBDCSS IoT Club</p></footer>");
            client.println("<script src=\"https://cdn.jsdelivr.net/npm/bootstrap@5.0.2/dist/js/bootstrap.bundle.min.js\" integrity=\"sha384-MrcW6ZMFYlzcLA8Nl+NtUVF0sA7MsXsP1UyJoMp4YLEuNSfAP+JcXn/tWtIaxVXM\" crossorigin=\"anonymous\"></script>");
            client.println("</body></html>");

            client.println();
            break;
          }
          else
          {
            currentLine = "";
          }
        }
        else if (c != '\r')
        {
          currentLine += c;
        }
      }
    }

    header = "";
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
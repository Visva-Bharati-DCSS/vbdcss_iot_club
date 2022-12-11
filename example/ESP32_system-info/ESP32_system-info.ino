// System Info Check for ESP32
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

void setup() // The setup() method runs once, when the sketch starts
{
  Serial.begin(115200); // Start the Serial communication to send messages to the computer
}

void loop() // the loop() method runs over and over again, as long as the ESP32 has power
{
  system_info(); // Print system info function
  delay(5000);   // Wait for 5 seconds
}

// Function to print the system info
void system_info()
{
  esp_chip_info_t chip_info;                                                                                                                                                             // Create a variable to store the chip info
  esp_chip_info(&chip_info);                                                                                                                                                             // Get the chip info
  Serial.println("ESP32 System info");                                                                                                                                                   // Print the title
  Serial.println("CPU Freq: " + String(ESP.getCpuFreqMHz()) + " MHz");                                                                                                                   // Print the CPU frequency
  Serial.println("Free Heap: " + String(ESP.getFreeHeap() / 1024) + " kilobytes");                                                                                                       // Print the free heap
  Serial.println("Sketch Size: " + String(ESP.getSketchSize() / 1024) + " kilobytes");                                                                                                   // Print the sketch size
  Serial.println("Sketch MD5: " + String(ESP.getSketchMD5()));                                                                                                                           // Print the sketch MD5
  Serial.println("Free Sketch Space: " + String(ESP.getFreeSketchSpace() / 1024) + " kilobytes");                                                                                        // Print the free sketch space
  Serial.println("Chip Model: " + String(ESP.getChipModel()));                                                                                                                           // Print the chip model
  Serial.printf("Chip Specification: %d cores Wifi %s %s\n", chip_info.cores, (chip_info.features & CHIP_FEATURE_BT) ? "BT" : "", (chip_info.features & CHIP_FEATURE_BLE) ? "BLE" : ""); // Print the chip features
  Serial.printf("Flash size: %dMB %s\n", spi_flash_get_chip_size() / (1024 * 1024), (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "Embeded" : "External");                             // Print the flash size                                                                                                                             // Print the chip revision
  Serial.println("Flash Speed: " + String(ESP.getFlashChipSpeed() / (1024 * 1024)) + " MHz");                                                                                            // Print the flash speed
  String chipId = String((uint32_t)ESP.getEfuseMac(), HEX);                                                                                                                              // Get the chip id
  chipId.toUpperCase();                                                                                                                                                                  // Convert the chip id to upper case
  Serial.printf("Chip id: %s\n", chipId.c_str());                                                                                                                                        // Print the chip id
  Serial.printf("Silicon revision: %d\n", chip_info.revision);
  Serial.println(" ");
}

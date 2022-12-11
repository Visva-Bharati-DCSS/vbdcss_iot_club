void setup()
{
  Serial.begin(115200);
}

void loop()
{
  system_info();
  delay(5000);
}

void system_info()
{
  esp_chip_info_t chip_info;
  esp_chip_info(&chip_info);
  Serial.println("ESP32 System info");
  Serial.println("CPU Freq: " + String(ESP.getCpuFreqMHz()) + "MHz");
  Serial.println("Free Heap: " + String(ESP.getFreeHeap() / 1024) + " kilobytes");
  Serial.println("Flash Size: " + String(ESP.getFlashChipSize() / 1024) + " kilobytes");
  Serial.println("Flash Speed: " + String(ESP.getFlashChipSpeed() / (1024 * 1024)) + "MHz");
  Serial.println("Sketch Size: " + String(ESP.getSketchSize() / 1024) + " kilobytes");
  Serial.println("Sketch MD5: " + String(ESP.getSketchMD5()));
  Serial.println("Free Sketch Space: " + String(ESP.getFreeSketchSpace() / 1024) + " kilobytes");
  Serial.print("Chip Model: " + String(ESP.getChipModel()));
  Serial.printf("%d cores Wifi %s%s", chip_info.cores, (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "", (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");
  Serial.printf("%dMB %s flash", spi_flash_get_chip_size() / (1024 * 1024), (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embeded" : "external");
  Serial.println("\nSilicon revision:" + chip_info.revision);
  String chipId = String((uint32_t)ESP.getEfuseMac(), HEX);
  chipId.toUpperCase();
  Serial.printf("Chip id: %s\n", chipId.c_str());
  Serial.println(" ");
}

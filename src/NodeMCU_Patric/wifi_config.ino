//==========================================================================================================================
void wifiModeSTA(String WIFI_SSID, String WIFI_PASSWORD) {
  Serial.println();
  Serial.println("======================Wifi-Mode-STA===========================");
  byte tries = 15;
  //For STA only or AP + STA mode
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to Wi-Fi");
  while (--tries && WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Non Connecting to WiFi..");
    Serial.println("Write status to EEPROM");

    EEPROM.begin(1024);
    delay(500);

    EEPROM.write(0, 0);
    EEPROM.commit();
    EEPROM.end();

    Serial.println("Reset..");
    ESP.restart();
  }

  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  Serial.println("=================================================");
  Serial.println("Use web browser and navigate to " + WiFi.localIP().toString());
  Serial.println("=================================================");
  Serial.println();

  splitString(WiFi.localIP().toString());
  checkConnectToServer();
  setCommands();
}
//=======================================================================================================================
void wifiModeAP() {
  Serial.println();
  Serial.println("======================Wifi-Mode-AP===========================");

  Serial.print("Setting soft-AP configuration ... ");
  //  "Задаем настройки программной точки доступа ... "
  Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");
  //  "Готово" : "Задать настройки не удалось"

  Serial.print("Setting soft-AP ... ");
  //  "Настройка программной точки доступа ... "
  Serial.println(WiFi.softAP(AP_SSID, AP_PASS) ? "Ready" : "Failed!");
  //  "Готово" : "Настройка не удалась"

  Serial.print("Soft-AP IP address = ");
  //  "IP-адрес программной точки доступа = "
  Serial.println(WiFi.softAPIP());

  setCommands();
}
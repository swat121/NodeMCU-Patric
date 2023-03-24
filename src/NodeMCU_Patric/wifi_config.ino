//==========================================================================================================================
void wifiModeSTA(String WIFI_SSID, String WIFI_PASSWORD) {
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

    EEPROM.begin(2048);
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

  server.on("/help", help);
  server.on("/status", getStatus);
  server.on("/relay1", relay1);
  server.on("/relay2", relay2);
  server.on("/relay3", relay3);
  server.on("/backlight", getBacklight);
  server.on("/temperature", getDataTemp);
  server.on("/light", light);
  server.on("/message", message);
  
}
//=======================================================================================================================
void wifiModeAP() {
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

  //Add the html contents (in html.h) for the web page rendering
  ESPForm.addFileData(index_html, "index.html");

  //Add html element event listener, id "text1" for onchange event
  ESPForm.addElementEventListener("text1", ESPFormClass::EVENT_ON_CHANGE);
  ESPForm.addElementEventListener("text3", ESPFormClass::EVENT_ON_CHANGE);
  ESPForm.addElementEventListener("click", ESPFormClass::EVENT_ON_CLICK);
  //Start ESPForm's Webserver
  ESPForm.begin(formElementEventCallback, serverTimeoutCallback, serverTimeout, true);

  ESPForm.startServer();
}
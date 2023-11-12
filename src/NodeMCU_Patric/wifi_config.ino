
//=======================================================================================================================

//=======================================================================================================================
void setupWifiConfig() {
  server.stop();

  splitString(WiFi.localIP().toString());

  data.mac = WiFi.macAddress();
  data.ssid = ssid;

  connectToServer(700);
  setCommands();

  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  Serial.println("=================================================");
  Serial.println("Use web browser and navigate to " + WiFi.localIP().toString());
  Serial.println("=================================================");
  Serial.println();

  server.begin();  //Запускаем сервер
}

//=======================================================================================================================
void changeWifiMode() {
  Serial.println("Change mode to AP");

  Serial.println("Write status to EEPROM");

  EEPROM.begin(1024);
  delay(500);

  EEPROM.write(0, 0);
  EEPROM.commit();
  EEPROM.end();

  Serial.println("Reset..");
  ESP.restart();
}
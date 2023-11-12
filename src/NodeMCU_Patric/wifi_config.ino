
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

  server.begin();
}


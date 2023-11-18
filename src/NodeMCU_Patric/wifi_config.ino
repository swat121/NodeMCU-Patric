
void setupWifiConfig() {
  server.stop();

  String ip_parts[4];
  splitString(WiFi.localIP().toString(), ip_parts);

  data.mac = WiFi.macAddress();
  data.ssid = ssid;

  connectToServer(ip_parts, 700);
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

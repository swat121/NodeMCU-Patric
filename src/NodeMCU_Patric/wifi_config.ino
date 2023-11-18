
void setupWifiConfig() {
  server.stop();

  

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

void splitString(String ip_address, String parts[4]) {
  int part_index = 0;
  while (ip_address.indexOf('.') >= 0) {
    int dot_pos = ip_address.indexOf('.');
    parts[part_index++] = ip_address.substring(0, dot_pos);
    ip_address = ip_address.substring(dot_pos + 1);
  }
  parts[part_index] = ip_address;
}

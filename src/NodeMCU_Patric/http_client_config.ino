void connectToServer(int timeout) {
  String whiteIp = parts[0] + "." + parts[1] + "." + parts[2] + ".";
  String payload;
  String ip;
  char host[100];
  char karenHost[100];
  int i;
  for (i = 1; i < 255; i++) {
    sprintf(host, "http://%s%d:%d/api/v1%s", whiteIp.c_str(), i, 8080, "/ping");
    Serial.println(host);
    if (ping(host, timeout) == "pong") {

      Serial.print("Karen host is: ");
      ip = whiteIp + i;
      Serial.println(ip);

      sprintf(karenHost, "http://%s%d:%d/api/v1%s", whiteIp.c_str(), i, 8080, "/clients");
      Serial.println(karenHost);

      StaticJsonDocument<200> doc;
      data.ip = WiFi.localIP().toString();
      doc["name"] = data.name;
      doc["ip"] = data.ip;
      doc["mac"] = data.mac;
      doc["ssid"] = data.ssid;
      serializeJson(doc, payload);
      String response = POSTRequest(karenHost, "application/json", payload);


      Serial.println("POST request to Karen");
      Serial.println(response);

      break;
    }
  }
  if (i >= 255) {
    Serial.println("Reset..");
    ESP.restart();
  }
}

String POSTRequest(char link[], String contentType, String payload) {
  String response = "";
  HTTPClient http;
  WiFiClient wifiClient;
  http.begin(wifiClient, link);
  http.addHeader("Content-Type", contentType);
  int httpCode = http.POST(payload);
  Serial.println(httpCode);
  if (httpCode == 200) {
    response = http.getString();
    ledBlink(5, 300);
  } else {
    response = "Error from server: " + http.getString();
  }
  http.end();
  return response;
}

void splitString(String ip_address) {

  int part_index = 0;

  while (ip_address.indexOf('.') >= 0) {
    int dot_pos = ip_address.indexOf('.');
    parts[part_index++] = ip_address.substring(0, dot_pos);
    ip_address = ip_address.substring(dot_pos + 1);
  }
  parts[part_index] = ip_address;
}

String ping(char link[], int timeout) {
  String response = "";
  HTTPClient http;
  WiFiClient wifiClient;
  http.begin(wifiClient, link);
  http.setTimeout(timeout);
  int httpCode = http.GET();
  if (httpCode == 200) {
    response = http.getString();
    Serial.println(response);
  }
  http.end();
  return response;
}
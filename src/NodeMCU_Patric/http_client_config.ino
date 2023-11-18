const int PORT = 8080;
const String PING_PATH = "/api/v1/ping";
const String CLIENTS_PATH = "/api/v1/clients";

void connectToServer(String parts[4], int timeout) {
  String whiteIp = parts[0] + "." + parts[1] + "." + parts[2] + ".";
  String ip, payload, response;
  bool isConnected = false;
  
  for (int i = 1; i < 255; i++) {
    String host = "http://" + whiteIp + String(i) + ":" + String(PORT) + PING_PATH;
    Serial.println(host);

    if (ping(host.c_str(), timeout) == "pong") {
      isConnected = true;
      ip = whiteIp + String(i);
      Serial.print("Karen host is: "); Serial.println(ip);

      String karenHost = "http://" + whiteIp + String(i) + ":" + String(PORT) + CLIENTS_PATH;
      Serial.println(karenHost);

      response = sendClientData(karenHost);
      Serial.println("POST request to Karen"); Serial.println(response);
      break;
    }
  }

  if (!isConnected) {
    Serial.println("Reset..");
    ESP.restart();
  }
}

String sendClientData(const String& host) {
  StaticJsonDocument<200> doc;
  String payload;
  doc["name"] = data.name;
  doc["ip"] = WiFi.localIP().toString();
  doc["mac"] = data.mac;
  doc["ssid"] = data.ssid;
  serializeJson(doc, payload);
  return POSTRequest(host.c_str(), "application/json", payload);
}


String POSTRequest(const String& link, const String& contentType, const String& payload) {
  HTTPClient http;
  WiFiClient wifiClient;
  http.begin(wifiClient, link);
  http.addHeader("Content-Type", contentType);

  int httpCode = http.POST(payload);
  String response = (httpCode == 200) ? http.getString() : "Error from server: " + http.getString();

  if (httpCode == 200) {
    ledBlink(5, 300);
  }

  http.end();
  return response;
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

String ping(const char* link, int timeout) {
  String response;
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

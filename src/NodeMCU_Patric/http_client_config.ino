void connectToServer(int timeout) {
  String whiteIp = parts[0] + "." + parts[1] + "." + parts[2] + ".";
  String clientPayload;
  String boardPayload;
  String ip;
  char host[100];
  char clientApi[100];
  char boardApi[100];
  int i;
  for (i = 1; i < 255; i++) {
    sprintf(host, "http://%s%d:%d/api/v1%s", whiteIp.c_str(), i, 8080, "/ping");
    Serial.println(host);
    if (ping(host, timeout) == "pong") {

      Serial.print("Karen host is: ");
      ip = whiteIp + i;
      Serial.println(ip);

      sprintf(clientApi, "http://%s%d:%d/api/v1%s", whiteIp.c_str(), i, 8080, "/clients");
      Serial.println(clientApi);

      StaticJsonDocument<200> doc;
      data.ip = WiFi.localIP().toString();
      doc["name"] = data.name;
      doc["ip"] = data.ip;
      doc["mac"] = data.mac;
      doc["ssid"] = data.ssid;
      serializeJson(doc, clientPayload);
      String response1 = POSTRequest(clientApi, "application/json", clientPayload);

      Serial.println("POST /clients request to Karen");
      Serial.println(response1);

      sprintf(boardApi, "http://%s%d:%d/api/v1%s", whiteIp.c_str(), i, 8080, "/board-config");
      Serial.println(boardApi);

      boardPayload = createBoardConfigObject();
      
      String response2 = POSTRequest(boardApi, "application/json", boardPayload);

      Serial.println("POST /board-config request to Karen");
      Serial.println(response2);

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

String createBoardConfigObject() {
  StaticJsonDocument<bodySize> jsonDoc;
  String boardPayload;
  jsonDoc["name"] = data.name;

  JsonObject setting = jsonDoc.createNestedObject("setting");

  JsonArray sensors = setting.createNestedArray("sensors");
  JsonObject temperature = sensors.createNestedObject();
  temperature["moduleName"] = "temperature";
  JsonArray temperatureData = temperature.createNestedArray("data");
  JsonObject tempData1 = temperatureData.createNestedObject();
  tempData1["moduleId"] = "1";
  tempData1["pin"] = ONE_WIRE_BUS;

  JsonArray switchers = setting.createNestedArray("switchers");
  JsonObject relay = switchers.createNestedObject();
  relay["moduleName"] = "relay";
  JsonArray relayData = relay.createNestedArray("data");
  JsonObject relayData1 = relayData.createNestedObject();
  relayData1["moduleId"] = "1";
  relayData1["pin"] = PIN_Relay1;

  JsonArray trackers = setting.createNestedArray("trackers");
  // JsonObject relay = switchers.createNestedObject();
  // relay["moduleName"] = "relay";
  // JsonArray relayData = relay.createNestedArray("data");
  // JsonObject relayData1 = relayData.createNestedObject();
  // tempData1["moduleId"] = "1";
  // tempData1["pin"] = PIN_Relay1.toString;

  serializeJson(jsonDoc, boardPayload);
  return boardPayload;
}
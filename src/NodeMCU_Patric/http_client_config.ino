const int PORT = 8080;
const String PING_PATH = "/api/v1/ping";
const String CLIENTS_PATH = "/api/v1/clients";
const String BOARD_PATH = "/api/v1/board-config";

String createUrl(const String& baseIp, int deviceNumber, const String& path) {
  return "http://" + baseIp + String(deviceNumber) + ":" + String(PORT) + path;
}

void connectToServer(String parts[4], int timeout) {
  String baseIp = parts[0] + "." + parts[1] + "." + parts[2] + ".";
  bool isConnected = false;

  for (int i = 1; i < 255; i++) {
    String pingUrl = createUrl(baseIp, i, PING_PATH);
    Serial.println(pingUrl);

    if (ping(pingUrl.c_str(), timeout) == "pong") {
      isConnected = true;
      String serverBaseUrl = "http://" + baseIp + String(i) + ":" + String(PORT);

      String clientDataResponse = sendClientData(createUrl(baseIp, i, CLIENTS_PATH));
      Serial.println("POST request to " + serverBaseUrl + CLIENTS_PATH);
      Serial.println(clientDataResponse);

      String boardDataResponse = sendBoardData(createUrl(baseIp, i, BOARD_PATH));
      Serial.println("POST request to " + serverBaseUrl + BOARD_PATH);
      Serial.println(boardDataResponse);

      break;
    }
  }

  if (!isConnected) {
    Serial.println("Reset..");
    ESP.restart();
  }
}

String sendClientData(const String& host) {
  String payload = createClientDataJson();
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


String sendBoardData(const String& host) {
  String payload = createBoardDataJson();
  return POSTRequest(host.c_str(), "application/json", payload);
}

String createBoardDataJson() {
  // Before change doc, you mus change doc size (check optimize doc size here https://arduinojson.org/v6/assistant/#/step1)
  StaticJsonDocument<512> jsonDoc;
  String payload;
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

  serializeJson(jsonDoc, payload);
  return payload;
}

String createClientDataJson() {
  // Before change doc, you mus change doc size (check optimize doc size here https://arduinojson.org/v6/assistant/#/step1)
  StaticJsonDocument<128> doc;
  String payload;
  doc["name"] = data.name;
  doc["ip"] = WiFi.localIP().toString();
  doc["mac"] = data.mac;
  doc["ssid"] = data.ssid;
  serializeJson(doc, payload);
  return payload;
}

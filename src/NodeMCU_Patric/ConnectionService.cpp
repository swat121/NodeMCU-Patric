#include "ConnectionService.h"

const int ConnectionService::PORT = 8080;
const String ConnectionService::PING_PATH = "/api/v1/ping";
const String ConnectionService::CLIENTS_PATH = "/api/v1/clients";
const String ConnectionService::BOARD_PATH = "/api/v1/board-config";

ConnectionService::ConnectionService() {
}

ConnectionService::ConnectionService(const String& clientDataJson, const String& boardDataJson)
  : payloadClientData(clientDataJson), payloadBoardData(boardDataJson) {
}

String ConnectionService::createUrl(const String& baseIp, int deviceNumber, const String& path) {
  return "http://" + baseIp + String(deviceNumber) + ":" + String(PORT) + path;
}

void ConnectionService::connectToServer(const String parts[4], int timeout) {
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

String ConnectionService::sendBoardData(const String& host) {
  return POSTRequest(host.c_str(), "application/json", payloadBoardData);
}

String ConnectionService::sendClientData(const String& host) {
  return POSTRequest(host.c_str(), "application/json", payloadClientData);
}

String ConnectionService::POSTRequest(const String& link, const String& contentType, const String& payload) {
  HTTPClient http;
  WiFiClient wifiClient;
  http.begin(wifiClient, link);
  http.addHeader("Content-Type", contentType);

  int httpCode = http.POST(payload);
  String response = (httpCode == 200) ? http.getString() : "Error from server: " + http.getString();

  if (httpCode == 200) {
    //ledBlink(5, 300);
  }

  http.end();
  return response;
}

String ConnectionService::ping(const char* link, int timeout) {
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
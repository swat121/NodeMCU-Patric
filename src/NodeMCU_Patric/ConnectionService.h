#ifndef CONNECTION_SERVICE_H
#define CONNECTION_SERVICE_H

#include <ArduinoJson.h>
#include <Arduino.h>
#include "ESP8266WebServer.h"
#include <ESP8266HTTPClient.h>

class ConnectionService {
public:
  ConnectionService();
  ConnectionService(const String& clientDataJson, const String& boardDataJson);
  void connectToServer(const String& ip_address, int timeout);
  String sendClientData(const String& host);
  String sendBoardData(const String& host);

private:
  static const int PORT;
  static const String PING_PATH;
  static const String CLIENTS_PATH;
  static const String BOARD_PATH;

  String payloadClientData;
  String payloadBoardData;

  String ping(const char* link, int timeout);
  String POSTRequest(const String& link, const String& contentType, const String& payload);
  String createUrl(const String& baseIp, int deviceNumber, const String& path);
  void splitString(String ip_address, String parts[4]);
};


#endif
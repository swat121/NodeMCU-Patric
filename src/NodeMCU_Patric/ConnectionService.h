#ifndef CONNECTION_SERVICE_H
#define CONNECTION_SERVICE_H

#include <ArduinoJson.h>
#include <Arduino.h>
#include "ESP8266WebServer.h"
#include <ESP8266HTTPClient.h>
#include <ESP8266mDNS.h>
#include <PubSubClient.h>

enum TopicType {
  DataEvent,
  Command
};

class ConnectionService {
public:
  ConnectionService(const String& deviceName);
  void runMDNS();
  void publishMessage(TopicType topicType, const String& message);
  void startConnectMqtt();
  void mqttLoop();

private:
  void setUpSetting();
  static void mqttCallback(char* topic, byte* payload, unsigned int length);
  void connectMqtt();
  String name;
};

#endif
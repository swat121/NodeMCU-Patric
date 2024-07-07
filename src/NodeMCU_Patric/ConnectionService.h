#ifndef CONNECTION_SERVICE_H
#define CONNECTION_SERVICE_H

#include <ArduinoJson.h>
#include <Arduino.h>
#include "ESP8266WebServer.h"
#include <ESP8266HTTPClient.h>
#include <ESP8266mDNS.h>
#include <PubSubClient.h>

enum TopicType {
  Event,
  Command,
  DataEvent
};

class ConnectionService {
public:
  ConnectionService(const String& deviceName);
  void runMDNS();
  void publishMessage(TopicType topicType, const String& message);
  void startConnectMqtt();
  void mqttLoop();
  bool readyToSendDataMessage;

private:
  void setUpSetting();
  static void mqttCallback(char* topic, byte* payload, unsigned int length);
  void connectMqtt();
  void attemptMqttConnect();

  String name;
  bool mqttConnecting;
  unsigned long lastReconnectAttempt;
};

#endif
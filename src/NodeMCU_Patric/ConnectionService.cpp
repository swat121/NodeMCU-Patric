#include "HardwareSerial.h"
#include "ConnectionService.h"

//------------------------------------------------------------------------

WiFiClient espClient;
PubSubClient mqttClient(espClient);

String getTopicString(TopicType type, const String& name) {
  switch (type) {
    case Event:
      return "home/esp/" + name + "/event";
    case Command:
      return "home/esp/" + name + "/command";
    case DataEvent:
      return "home/esp/data/event";
    default:
      return "";
  }
}

ConnectionService::ConnectionService(const String& deviceName)
  : name(deviceName) {
  mqttClient.setCallback([this](char* topic, byte* payload, unsigned int length) {
    this->mqttCallback(topic, payload, length);
  });
}

void ConnectionService::runMDNS() {
  if (!MDNS.begin(name)) {
    Serial.println("Error setting up MDNS responder!");
    while (1) {
      delay(1000);
    }
  } else {
    Serial.println("mDNS responder started");
  }
  // setUpSetting();
}

void ConnectionService::setUpSetting() {
  MDNS.addService("http", "tcp", 80);
  MDNS.addServiceTxt("http", "tcp", "mac", WiFi.macAddress());
  MDNS.addServiceTxt("http", "tcp", "ssid", "HelloWorld");
  MDNS.addServiceTxt("http", "tcp", "version", "1.0");
}

void ConnectionService::mqttCallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void ConnectionService::findMqttService() {
  Serial.println("Searching for MQTT service using mDNS");
  int n = MDNS.queryService(mqttService, "tcp");
  if (n == 0) {
    Serial.println("No MQTT service found.");
    lastMqttSearchAttempt = millis();
    return;
  }

  IPAddress serverIp = MDNS.IP(0);
  Serial.print("We found MQTT service using mDNS: ");
  Serial.println(serverIp.toString());

  mqttClient.setServer(serverIp, mqttPort);

  mqttServiceFound = true;
}

void ConnectionService::attemptMqttConnect() {
  if (!mqttClient.connected()) {
    String clientId = name + String(random(0xffff), HEX);
    if (mqttClient.connect(clientId.c_str())) {
      Serial.println();
      Serial.println("MQTT connected: " + mqttClient.state());

      String topicCommand = getTopicString(Command, name);
      mqttClient.subscribe(topicCommand.c_str());
      Serial.println("Subscribed to topic: " + topicCommand);

      mqttConnecting = false;
      readyToSendDataMessage = true;
    } else {
      Serial.print("MQTT connect failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" trying again in 5 seconds");
      mqttConnecting = true;
    }
  }
}

void ConnectionService::startFoundingMqttService() {
  if (!mqttClient.connected() && !mqttConnecting) {
    Serial.println(F("====================== START-FOUNDING-MQTT-SERVICE ==========================="));
    findMqttService();
    mqttConnecting = true;
  }
}

void ConnectionService::publishMessage(TopicType topicType, const String& message) {
  Serial.println(F("====================== PUBLISH-MESSAGE ==========================="));

  String topic = getTopicString(topicType, name);
  Serial.println("Send message: " + message + " to topic: " + topic);
  if (mqttClient.publish(topic.c_str(), message.c_str())) {
    Serial.println("Message published successfully");
  } else {
    Serial.println("Message publishing failed");
  }
}

void ConnectionService::mqttLoop() {
  if (mqttServiceFound && mqttConnecting && (millis() - lastReconnectAttempt > 5000)) {
    lastReconnectAttempt = millis();
    attemptMqttConnect();
  }

  if (!mqttServiceFound && (millis() - lastMqttSearchAttempt > mqttSearchInterval)) {
    lastMqttSearchAttempt = millis();
    findMqttService();
  }

  mqttClient.loop();
}
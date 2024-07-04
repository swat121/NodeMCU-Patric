#include "ConnectionService.h"

//------------------------------------------------------------------------

WiFiClient espClient;
PubSubClient mqttClient(espClient);

const char* mqttService = "mqtt";
const uint16_t mqttPort = 1883;

String getTopicString(TopicType type, const String& name) {
  switch (type) {
    case DataEvent:
      return "home/esp/" + name + "/event";
    case Command:
      return "home/esp/" + name + "/command";
    default:
      return "";
  }
}

ConnectionService::ConnectionService(const String& deviceName) : name(deviceName) {
  mqttClient.setCallback([this](char* topic, byte* payload, unsigned int length) { this->mqttCallback(topic, payload, length); });
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

void ConnectionService::connectMqtt() {
  Serial.println("Searching for MQTT service using mDNS");
  int n = MDNS.queryService(mqttService, "tcp");
  if (n == 0) {
    Serial.println("No MQTT service found.");
    return;
  }

  IPAddress serverIp = MDNS.IP(0);
  Serial.print("We found MQTT service using mDNS: ");
  Serial.println(serverIp.toString());

  mqttClient.setServer(serverIp, mqttPort);

  while (!mqttClient.connected()) {
    String clientId = name + String(random(0xffff), HEX);
    if (mqttClient.connect(clientId.c_str())) {
      Serial.println("MQTT connected");

      String topicCommand = "home/esp/" + name + "/command";

      mqttClient.subscribe(topicCommand.c_str());
      Serial.println("Subscribed to topic: " + topicCommand);
    } else {
      Serial.print("MQTT connect failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void ConnectionService::startConnectMqtt() {
  if (!mqttClient.connected()) {
    connectMqtt();
  }
}

void ConnectionService::publishMessage(TopicType topicType, const String& message) {
  String topic = getTopicString(topicType, name);
  mqttClient.publish(topic.c_str(), message.c_str());
}

void ConnectionService::mqttLoop() {
  mqttClient.loop();
}
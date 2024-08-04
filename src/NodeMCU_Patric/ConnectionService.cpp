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

  StaticJsonDocument<200> doc;

  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String message;

  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.println(message);

  DeserializationError error = deserializeJson(doc, message);
  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.f_str());
    return;
  }

  const char* moduleName = doc["moduleName"];
  const char* moduleId = doc["moduleId"];
  const char* mode = doc["mode"];
  const char* userId = doc["userId"];
  const char* type = doc["type"];

  if (strcmp(type, "SWITCHER") == 0) {
    switchHandlerV2(String(moduleName), String(moduleId), String(mode), String(userId));
  }

  if (strcmp(type, "SENSOR") == 0) {
    sensorHandlerV2(String(moduleName), String(moduleId), String(userId));
  }

  if (strcmp(type, "TRACKER") == 0) {
    // Your code for TRACKERS
  }
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
  mqttClient.setCallback(mqttCallback);

  mqttServiceFound = true;
}

void ConnectionService::attemptMqttConnect() {
  if (!mqttClient.connected()) {
    String clientId = name + String(random(0xffff), HEX);
    if (mqttClient.connect(clientId.c_str())) {
      Serial.println();
      Serial.println("MQTT connected: " + mqttClient.state());

      String topicCommand = getTopicString(Command, name);

      if (mqttClient.subscribe(topicCommand.c_str(), 1)) {
        Serial.println();
        Serial.println("Successfully subscribed to topic: " + topicCommand);
      } else {
        Serial.println();
        Serial.println("Failed to subscribed to topic: " + topicCommand);
      }

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
#include "ConnectionService.h"

ConnectionService::ConnectionService() {
}

void ConnectionService::connectToServer() {
 if (!MDNS.begin("esp-plug-1")) {
    Serial.println("Error setting up MDNS responder!");
    while (1) {
      delay(1000);
    }
  }
  setUpSetting();
}

void ConnectionService::setUpSetting() {
  MDNS.addService("http", "tcp", 80);
  MDNS.addServiceTxt("http", "tcp", "name", "patric");
  MDNS.addServiceTxt("http", "tcp", "mac", WiFi.macAddress());
  MDNS.addServiceTxt("http", "tcp", "ssid", "HelloWorld");
  MDNS.addServiceTxt("http", "tcp", "version", "1.0");
  Serial.println("mDNS responder started");
}
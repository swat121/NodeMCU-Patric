#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <Arduino.h>
#include "ESP8266WebServer.h"
#include <ESP8266HTTPClient.h>

class WiFiManager {
public:
    WiFiManager(ESP8266WebServer& server);
    void wifiModeSTA(const String& ssid, const String& password);
    void wifiModeAP(const String& ssid, const String& password);
    void setupWifiConfig();
    void changeWifiMode();
    void writeToEEPROM(const String& ssid, const String& pass);
    void readFromEEPROM();
    // Другие методы по необходимости

private:
    ESP8266WebServer& server;
};

#endif

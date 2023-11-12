#include "WString.h"
#include "WiFiManager.h"

IPAddress local_IP(192, 168, 4, 22);
IPAddress gateway(192, 168, 4, 9);
IPAddress subnet(255, 255, 255, 0);

WiFiManager::WiFiManager(ESP8266WebServer& server)
  : server(server) {
  // Конструктор
}

void WiFiManager::wifiModeSTA(const String& ssid, const String& password) {
  Serial.println();
  Serial.println("======================Wifi-Mode-STA===========================");
  byte tries = 15;

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("Connecting to Wi-Fi");
  while (--tries && WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Non Connecting to WiFi..");
  } else {
    Serial.println("Connected to WiFi..");
  }
  server.begin();
}

void WiFiManager::wifiModeAP(const String& ssid, const String& password) {
  Serial.println();
  Serial.println("======================Wifi-Mode-AP===========================");

  Serial.print("Setting soft-AP configuration ... ");
  //  "Задаем настройки программной точки доступа ... "
  Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");
  //  "Готово" : "Задать настройки не удалось"

  Serial.print("Setting soft-AP ... ");
  Serial.print(ssid);
  Serial.print(password);

  //  "Настройка программной точки доступа ... "
  Serial.println(WiFi.softAP(ssid, password) ? "Ready" : "Failed!");
  //  "Готово" : "Настройка не удалась"

  Serial.print("Soft-AP IP address = ");
  //  "IP-адрес программной точки доступа = "
  Serial.println(WiFi.softAPIP());
}

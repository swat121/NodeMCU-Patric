#include "ESP8266WebServer.h"
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

#include <uri/UriBraces.h>

#include <OneWire.h>
#include <DallasTemperature.h>

#include <EEPROM.h>

#include "GyverButton.h"

// Button variables
#define BTN_PIN 5
GButton switchModeButton(BTN_PIN);

//------------------------------------------------------------------------
struct Data {
  String name = "patric";
  String ip;
  String mac;
  String ssid;
};

Data data;

//------------------------------------------------------------------------
#define AP_SSID data.name
#define AP_PASS "12345678"

//------------------------------------------------------------------------
IPAddress local_IP(192, 168, 4, 22);
IPAddress gateway(192, 168, 4, 9);
IPAddress subnet(255, 255, 255, 0);

//------------------------------------------------------------------------
boolean status = true;
String ssid;
String pass;
String WifiMode;
//------------------------------------------------------------------------

unsigned long timer;
boolean stat = true;
#define PIN_LED_Good 2    //D4
#define PIN_LED_Error 14  //D5
//------------------------------------------------------------------------
int PIN_Relay1 = 12;  //D6
boolean Relay1 = false;

int PIN_Relay2 = 13;  //D7
boolean Relay2 = false;

int PIN_Relay3 = 15;  //D8
boolean Relay3 = false;

int PIN_Power_Module = 4;  //D2
boolean PowerStatus = false;

//------------------------------------------------------------------------

boolean flagForCheckConnect = false;
boolean flagIsConnectToServer = true;

//------------------------------------------------------------------------
ESP8266WebServer server(80);
//------------------------------------------------------------------------

//------------------------------------------------------------------------
int ONE_WIRE_BUS = 0;  //D3
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
unsigned long timerTemp;
int ds18b20Count = 0;
//------------------------------------------------------------------------

String parts[4];
//------------------------------------------------------------------------

#define bodySize 1024


void setup() {

  Serial.begin(115200);
  delay(100);

  switchModeButton.setTimeout(3000);
  switchModeButton.setType(HIGH_PULL);

  switchModeButton.setDirection(NORM_OPEN);

  Serial.println();
  Serial.println("Branch: develop");

  //------------------------------------------------------------------------
  sensors.begin();
  ds18b20Count = sensors.getDeviceCount();
  //------------------------------------------------------------------------

  pinMode(PIN_LED_Good, OUTPUT);
  pinMode(PIN_LED_Error, OUTPUT);

  pinMode(PIN_Relay1, OUTPUT);
  digitalWrite(PIN_Relay1, LOW);

  pinMode(PIN_Relay2, OUTPUT);
  digitalWrite(PIN_Relay2, LOW);

  pinMode(PIN_Relay3, OUTPUT);
  digitalWrite(PIN_Relay3, LOW);

  pinMode(PIN_Power_Module, OUTPUT);
  analogWrite(PIN_Power_Module, 0);

  //---------------------------------------------------------------------------------------------------
  
  readFromEEPROM();

  //---------------------------------------------------------------------------------------------------
  if (status) {
    WifiMode = "STA";
    wifiModeSTA(ssid, pass);
  } else {
    WifiMode = "AP";
    wifiModeAP();
  }
  //-----------------------------------------------------------------------------------------------------
}

//-----------------------------------LOOP--------------------------------------------------------------
void loop() {
  switchModeButton.tick();
  if (switchModeButton.isHolded()) {
    Serial.println("Button is holding");
    
    digitalWrite(PIN_LED_Error, LOW);
    digitalWrite(PIN_LED_Good, LOW);
    ledBlink(3, 100);
    changeWifiMode();
  }

  if (WifiMode == "STA") {
    if (WiFi.status() != WL_CONNECTED) {
      ledDisconnect();
      return;
    }
    digitalWrite(PIN_LED_Error, LOW);

    if (flagIsConnectToServer) {
      setupWifiConfig();
      ledBlink(3, 100);
      flagIsConnectToServer = false;
    }
  }
  server.handleClient();
}

//-----------------------------------------------------------------------------------------------------

void ledBlink(int count, int microsecond) {
  for (int i = 0; i < count; i++) {
    digitalWrite(PIN_LED_Good, HIGH);
    delay(microsecond);
    digitalWrite(PIN_LED_Good, LOW);
    delay(microsecond);
  }
}

//-----------------------------------------------------------------------------------------------------

void ledDisconnect() {
  if (millis() - timer > 1000) {
    Serial.println("Wifi not connected");
    timer = millis();
    digitalWrite(PIN_LED_Error, stat);
    stat = !stat;
  }
}

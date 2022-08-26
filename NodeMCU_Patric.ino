#include "ESP8266WiFi.h"
#include "ESP8266WebServer.h"
#include <ArduinoJson.h>

#include <OneWire.h>
#include <DallasTemperature.h>

//------------------------------------------------------------------------
unsigned long timer;
boolean stat = true;
#define PIN_LED_Good 2 //D4
#define PIN_LED_Error 14 //D5
//------------------------------------------------------------------------
#define PIN_Relay1 12 //D6
boolean Relay1 = false;

#define PIN_Relay2 13 //D7
boolean Relay2 = false;

#define PIN_Relay3 15 //D8
boolean Relay3 = false;

#define PIN_Light 4 //D2
boolean Light = false;
//------------------------------------------------------------------------

boolean backlightStat = false;
boolean connectStat = false;

//------------------------------------------------------------------------
ESP8266WebServer server(80);
const char* ssid = "Parents";
const char* password = "Drim1932";
//------------------------------------------------------------------------



//------------------------------------------------------------------------
#define ONE_WIRE_BUS 0
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
float temperature;
unsigned long timerTemp;
//------------------------------------------------------------------------


byte tries = 15;  // Попыткок подключения к точке доступа

void setup() {

  Serial.begin(115200);
  delay(100);

//------------------------------------------------------------------------
  sensors.begin();
//------------------------------------------------------------------------

  pinMode(PIN_LED_Good, OUTPUT);
  pinMode(PIN_LED_Error, OUTPUT);
  
  pinMode(PIN_Relay1, OUTPUT);
  digitalWrite(PIN_Relay1, LOW);
  
  pinMode(PIN_Relay2, OUTPUT);
  digitalWrite(PIN_Relay2, LOW);
  
  pinMode(PIN_Relay3, OUTPUT);
  digitalWrite(PIN_Relay3, LOW);

  pinMode(PIN_Light, OUTPUT);
  analogWrite(PIN_Light, 0);

//---------------------------------------------------------------------------------------------------
  WiFi.begin(ssid, password);
  
  while (--tries && WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Non Connecting to WiFi..");
  }
  else
  {
    // Иначе удалось подключиться отправляем сообщение
    // о подключении и выводим адрес IP
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    
    ledBlink(3,300);
    
  }
  
  server.on("/help", help);
  server.on("/status", getStatus);
  server.on("/relay1", relay1);
  server.on("/relay2", relay2);
  server.on("/relay3", relay3);
  server.on("/backlight", getBacklight);
  server.on("/temperature", getDataTemp);
  server.on("/light", light);
  server.on("/message", message); 
  server.begin();                    //Запускаем сервер
  Serial.println("Server listening");
//-----------------------------------------------------------------------------------------------------
  
}



//-----------------------------------LOOP--------------------------------------------------------------
void loop() {
  server.handleClient();
  checkConnect();
}
//-----------------------------------------------------------------------------------------------------

void help(){
  sendMessage("help", "/status; /relay1; /relay2; /relay3; /backlight; /temperature; /light; /message;");
  ledBlink(1,100);
}

//-----------------------------------------------------------------------------------------------------

void getStatus(){
  StaticJsonDocument<200> doc;
  String s;
  doc["name"] = "Patric";
  doc["relay1"] = String(Relay1);
  doc["relay2"] = String(Relay2);
  doc["relay3"] = String(Relay3);
  serializeJson(doc, s);
  server.send(200, "application/json", s);
  ledBlink(1,100);
}

//-----------------------------------------------------------------------------------------------------

void relay1(){
  Relay1 = !Relay1;
  digitalWrite(PIN_Relay1, Relay1);
  sendMessage("relay1", String(Relay1));
  ledBlink(1,100);
}

void relay2(){
  Relay2 = !Relay2;
  digitalWrite(PIN_Relay2, Relay2);
  sendMessage("relay2", String(Relay2));
  ledBlink(1,100);
}

void relay3(){
  Relay3 = !Relay3;
  digitalWrite(PIN_Relay3, Relay3);
  sendMessage("relay3", String(Relay3));
  ledBlink(1,100);
}

//-----------------------------------------------------------------------------------------------------

void getBacklight(){
  sendMessage("backlight", "not yet");
  ledBlink(1,100);
}

//-----------------------------------------------------------------------------------------------------

void getDataTemp(){
  sendMessage("temp",String(getTemperature()));
  ledBlink(1,100);
}

//-----------------------------------------------------------------------------------------------------

void light(){
  Light = !Light;
  if(Light == true){
    for(int i = 0; i < 255; i++){
    analogWrite(PIN_Light, i);
    delay(10); 
    }
  }
  if(Light == false){
    for(int i = 254; i > -1; i--){
    analogWrite(PIN_Light, i);
    delay(10); 
    }
  }
  sendMessage("light", String(Light));
  ledBlink(1,100);
}

//-----------------------------------------------------------------------------------------------------

void message(){
  sendMessage("message","not yet");
  ledBlink(1,100);
}

//-----------------------------------------------------------------------------------------------------

void checkConnect(){
  if(WiFi.status() != WL_CONNECTED){
    ledDisconnect();
    connectStat = true;
  } else {
    if (connectStat == true) {
      ledBlink(3,100);
      connectStat = false;
      digitalWrite(PIN_LED_Error, LOW);
    }
  }
}

//-----------------------------------------------------------------------------------------------------

void ledBlink(int count, int microsecond){
  for(int i =0;i<count;i++){
      digitalWrite(PIN_LED_Good, HIGH);
      delay(microsecond);
      digitalWrite(PIN_LED_Good, LOW);
      delay(microsecond);
    }
}

//-----------------------------------------------------------------------------------------------------

void ledDisconnect(){ 
  if(millis()-timer > 1000){
    timer = millis();
    digitalWrite(PIN_LED_Error, stat);
    stat = ! stat;
  }
}

//-----------------------------------------------------------------------------------------------------

float getTemperature() {
  sensors.requestTemperatures();
  temperature = sensors.getTempCByIndex(0);
  return temperature;
}

//-----------------------------------------------------------------------------------------------------

void sendMessage(String key, String value) {
  StaticJsonDocument<200> doc;
  String s;
  doc["name"] = "Patric";
  doc[key] = value;
  serializeJson(doc, s);
  server.send(200, "application/json", s);
}

//-----------------------------------------------------------------------------------------------------

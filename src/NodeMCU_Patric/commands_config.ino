//-----------------------------------------------------------------------------------------------------
void setCommands() {
  Serial.println();
  Serial.println("==================SET-COMMAND===============");
  Serial.println(WifiMode);
  if (WifiMode == "STA") {
    server.on("/help", HTTP_GET, getHelp);
    server.on(UriBraces("/relays/{}"), HTTP_PUT, relayHandle);
    server.on("/power-module", HTTP_PUT, putPowerModule);
    server.on(UriBraces("/temperature/{}"), HTTP_GET, getDataTemp);
    server.on("/light", HTTP_GET, getLight);
    server.on("/message", HTTP_PUT, putMessage);
    server.on("/status", HTTP_GET, getStatus);
  }
  if (WifiMode == "AP") {
    server.on("/", HTTP_GET, handleMainHtmlPage);
    server.on("/submit", HTTP_POST, handleFormSubmit);
  }
}

//-----------------------------------------------------------------------------------------------------

void getHelp() {
  String s;
  StaticJsonDocument<200> doc;
  JsonObject help = doc.createNestedObject("help");

  help["status"] = "GET /status";

  JsonObject sensor = help.createNestedObject("sensor");
  sensor["temp"] = "GET /temperature/{id}";
  sensor["light"] = "GET /light";


  JsonObject switcher = help.createNestedObject("switcher");
  switcher["relay"] = "PUT /relays/{id}";
  switcher["power-module"] = "PUT /power-module";

  sendMessage(doc, 200);
  ledBlink(1, 100);
}

//-----------------------------------------------------------------------------------------------------

void getStatus() {
  StaticJsonDocument<200> doc;
  String s;
  doc["name"] = data.name;
  doc["relay1"] = String(digitalRead(PIN_Relay1));
  doc["relay2"] = String(digitalRead(PIN_Relay2));
  doc["relay3"] = String(digitalRead(PIN_Relay3));
  doc["powerModule"] = String(digitalRead(PIN_Power_Module));
  serializeJson(doc, s);
  server.send(200, "application/json", s);
  ledBlink(1, 100);
}

//-----------------------------------------------------------------------------------------------------

void relayHandle() {
  int idParam = server.pathArg(0).toInt();
  switch (idParam) {
    case 1: relay1(); break;
    case 2: relay2(); break;
    case 3: relay3(); break;
    default: sendMessage("Error", "Relay module not found!", 404);
  }
}

void relay1() {
  Relay1 = !Relay1;
  digitalWrite(PIN_Relay1, Relay1);
  sendMessage("relay1", String(digitalRead(PIN_Relay1)));
  ledBlink(1, 100);
}

void relay2() {
  Relay2 = !Relay2;
  digitalWrite(PIN_Relay2, Relay2);
  delay(500);
  Relay2 = !Relay2;
  digitalWrite(PIN_Relay2, Relay2);
  sendMessage("relay2", "OPEN");
  ledBlink(1, 100);
}

void relay3() {
  Relay3 = !Relay3;
  digitalWrite(PIN_Relay3, Relay3);
  sendMessage("relay3", String(digitalRead(PIN_Relay3)));
  ledBlink(1, 100);
}

//-----------------------------------------------------------------------------------------------------

void putPowerModule() {
  PowerStatus = !PowerStatus;
  if (PowerStatus == true) {
    for (int i = 0; i < 255; i++) {
      analogWrite(PIN_Power_Module, i);
      delay(10);
    }
    digitalWrite(PIN_Power_Module, HIGH);
  }
  if (PowerStatus == false) {
    for (int i = 255; i > -1; i--) {
      analogWrite(PIN_Power_Module, i);
      delay(10);
    }
  }
  sendMessage("PowerStatus", String(digitalRead(PIN_Power_Module)));
  ledBlink(1, 100);
}

//-----------------------------------------------------------------------------------------------------

void getDataTemp() {
  int id = server.pathArg(0).toInt();
  float responce = getTemperatureByIndex(id);

  if (responce == -127.00) {
    sendMessage("temp", "Temperature device not found!", 404);
  } else {
    sendMessage("temp", String(responce));
  }
  ledBlink(1, 100);
}

//-----------------------------------------------------------------------------------------------------

void getLight() {
  sendMessage("backlight", "not yet");
  ledBlink(1, 100);
}

//-----------------------------------------------------------------------------------------------------

void putMessage() {
  sendMessage("message", "not yet");
  ledBlink(1, 100);
}
//-----------------------------------------------------------------------------------------------------

void sendMessage(String key, String value) {
  StaticJsonDocument<200> doc;
  String s;
  doc["name"] = data.name;
  doc[key] = value;
  serializeJson(doc, s);
  server.send(200, "application/json", s);
}

void sendMessage(String key, String value, int statusCode) {
  StaticJsonDocument<200> doc;
  String s;
  doc["name"] = data.name;
  doc[key] = value;
  serializeJson(doc, s);
  server.send(statusCode, "application/json", s);
}

void sendMessage(StaticJsonDocument<200> doc, int statusCode) {
  String s;
  doc["name"] = data.name;
  serializeJson(doc, s);
  server.send(statusCode, "application/json", s);
}
//-----------------------------------------------------------------------------------------------------

float getTemperatureByIndex(int idParam) {
  sensors.requestTemperatures();
  return sensors.getTempCByIndex(idParam);
}

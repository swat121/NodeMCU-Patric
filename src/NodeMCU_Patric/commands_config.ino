//-----------------------------------------------------------------------------------------------------
void setCommands() {
  Serial.println();
  Serial.println("==================SET-COMMAND===============");
  Serial.println(WifiMode);
  if (WifiMode == "STA") {
    server.on("/help", HTTP_GET, getHelp);
    server.on(UriBraces("/relays/{}"), HTTP_PUT, relayHandle);
    server.on("/power-module", HTTP_PUT, putPowerModule);
    server.on("/temperature", HTTP_GET, getDataTemp);
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
  sendMessage("help", "/status; /relay1; /relay2; /relay3; /backlight; /temperature; /light; /message;");
  ledBlink(1, 100);
}

//-----------------------------------------------------------------------------------------------------

void getStatus() {
  StaticJsonDocument<200> doc;
  String s;
  doc["name"] = data.name;
  doc["relay1"] = String(Relay1);
  doc["relay2"] = String(Relay2);
  doc["relay3"] = String(Relay3);
  doc["light"] = String(Light);
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
  }
}

void relay1() {
  Relay1 = !Relay1;
  digitalWrite(PIN_Relay1, Relay1);
  sendMessage("relay1", String(Relay1));
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
  sendMessage("relay3", String(Relay3));
  ledBlink(1, 100);
}

//-----------------------------------------------------------------------------------------------------

void putPowerModule() {
  Light = !Light;
  if (Light == true) {
    for (int i = 0; i < 255; i++) {
      analogWrite(PIN_Light, i);
      delay(10);
    }
    digitalWrite(PIN_Light, HIGH);
  }
  if (Light == false) {
    for (int i = 255; i > -1; i--) {
      analogWrite(PIN_Light, i);
      delay(10);
    }
  }
  sendMessage("light", String(Light));
  ledBlink(1, 100);
}

//-----------------------------------------------------------------------------------------------------

void getDataTemp() {
  sendMessage("temp", String(getTemperature()));
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
  doc["name"] = "Patric";
  doc[key] = value;
  serializeJson(doc, s);
  server.send(200, "application/json", s);
}
//-----------------------------------------------------------------------------------------------------

float getTemperature() {
  sensors.requestTemperatures();
  temperature = sensors.getTempCByIndex(0);
  return temperature;
}

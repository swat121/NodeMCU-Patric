//-----------------------------------------------------------------------------------------------------

void getTelegramResponse() {
  const char* serverName = "http://192.168.0.102:8080/bot/alarm";
  String sensorReadings;
  
  sensorReadings = httpGETRequest(serverName);
  Serial.println(sensorReadings);
  if (sensorReadings == "true") {
    ledBlink(3, 200);
  } else {
    ledBlink(7, 200);
  }
}
//-----------------------------------------------------------------------------------------------------

String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;

  // Your IP address with path or Domain name with URL path
  http.begin(client, serverName);

  // Send HTTP POST request
  int httpResponseCode = http.GET();

  String payload = {};

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
    ledDisconnect();
  }
  // Free resources
  http.end();
  return payload;
}

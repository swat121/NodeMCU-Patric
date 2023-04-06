void checkConnectToServer() {
  String whiteIp = parts[0] + "." + parts[1] + "." + parts[2] + ".";
  int timeout = 700;
  int port = 8080;
  String uri = "/ping";
  char host[100];
  for (int i = 1; i < 255; i++) {
    sprintf(host, "http://%s%d:%d%s", whiteIp.c_str(), i, port, uri.c_str());
    Serial.println(host);
    if (ping(host, timeout) == "pong") {
      Serial.print("Karen host is: ");
      Serial.println(whiteIp + i);
      break;
    }
  }
}

void splitString(String ip_address) {

  int part_index = 0;

  while (ip_address.indexOf('.') >= 0) {
    int dot_pos = ip_address.indexOf('.');
    parts[part_index++] = ip_address.substring(0, dot_pos);
    ip_address = ip_address.substring(dot_pos + 1);
  }
  parts[part_index] = ip_address;
}

String ping(char link[], int timeout) {
  String response = "";
  HTTPClient http;
  WiFiClient wifiClient;
  http.begin(wifiClient, link);
  http.setTimeout(timeout);
  int httpCode = http.GET();
  if (httpCode == 200) {
    response = http.getString();
    Serial.println(response);
  }
  http.end();
  return response;
}
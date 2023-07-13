//================================================================
void writeToEEPROM(String ssid, String pass) {
  Serial.println();
  Serial.println("======================Start-Write-To-EEPROM===========================");
  int ssidLenght = ssid.length();
  int passLenght = pass.length();

  int i = 0;
  int j;

  EEPROM.begin(1024);
  delay(500);

  Serial.println("writing eeprom ssid:");
  EEPROM.write(0, '1');
  Serial.print("Wrote: ");
  Serial.println(1);

  for (i = 1; i < ssidLenght + 1; i++) {
    EEPROM.write(i, ssid[i - 1]);
    Serial.print("Wrote: ");
    Serial.println(ssid[i - 1]);
  }

  EEPROM.write(i, '|');
  Serial.print("Wrote: ");
  Serial.println("|");

  i++;

  for (j = i; j < passLenght + i; j++) {
    EEPROM.write(j, pass[j - i]);
    Serial.print("Wrote: ");
    Serial.println(pass[j - i]);
  }

  EEPROM.write(j, '&');
  Serial.print("Wrote: ");
  Serial.println("&");

  EEPROM.commit();
  EEPROM.end();
  Serial.println("======================END-Write-To-EEPROM===========================");
}
//================================================================
void readFromEEPROM() {
  Serial.println();
  Serial.println("======================Start-Read-From-EEPROM===========================");
  char readSSID;
  char readPASS;
  ssid = "";
  pass = "";
  int i = 1;

  EEPROM.begin(1024);
  delay(200);

  status = boolean(EEPROM.read(0));

  while (readSSID != '|') {
    readSSID = char(EEPROM.read(i));
    if (readSSID == '|') {
      break;
    }
    delay(10);
    ssid += readSSID;
    i++;
    if (i > 40) {
      break;
    }
  }

  i++;

  while (readPASS != '&') {
    readPASS = char(EEPROM.read(i));
    if (readPASS == '&') {
      break;
    }
    delay(10);
    pass += readPASS;
    i++;
    if (i > 80) {
      break;
    }
  }

  EEPROM.end();

  Serial.print("status from EEPROM: ");
  Serial.println(status);
  Serial.print("ssid from EEPROM: ");
  Serial.println(ssid);
  Serial.print("pass from EEPROM: ");
  Serial.println(pass);
  Serial.println("======================END-Read-To-EEPROM===========================");
}
//================================================================
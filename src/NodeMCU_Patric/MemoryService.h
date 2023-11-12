#ifndef MEMORY_SERVICE_H
#define MEMORY_SERVICE_H

#include <EEPROM.h>
#include <Arduino.h>

struct MemoryCredentials {
  String ssid;
  String password;
  boolean status;
};

class MemoryService {
public:
  MemoryService();
  void writeSsidAndPass(const String& ssid, const String& password);
  void writeStatus(char status);
  void startEEPROMSession(int size);
  void endEEPROMSession();
  MemoryCredentials readSsidAndPass();
};

#endif
#ifndef CLIENT_DATA_H
#define CLIENT_DATA_H

#include <Arduino.h>

struct ClientData {
    String name = "patric";
    String ip;
    String mac;
    String ssid;
    String apPass = "12345678";
    String staPass;
};

#endif

#ifndef WIFI_HPP
#define WIFI_HPP

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>

//#include <usb_serial.h>
//#include <core_pins.h>
#include "../ntptime/NtpClientLib.h"

extern int vfdPrint(const char *c, int n);

const char ssid[] = "HellSpot Slow";
const char password[] = "ILikeWiFi";

void connectToWifi();

#endif

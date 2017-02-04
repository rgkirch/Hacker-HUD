#ifndef WIFI_HPP
#define WIFI_HPP

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>

//#include <usb_serial.h>
//#include <core_pins.h>
#include "../vfd/vfd.hpp"

extern std::unique_ptr<VFD> myVFD;

const char ssid[] = "HellSpot Slow";
const char password[] = "ILikeWiFi";

void connectToWifi();

#endif

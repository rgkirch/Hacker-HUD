#ifndef WIFI_HPP
#define WIFI_HPP

#include <ESP8266WiFi.h>
#include "globals.hpp"

void connectToWifi(){
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while(WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

#endif

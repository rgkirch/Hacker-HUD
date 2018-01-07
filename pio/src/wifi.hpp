#ifndef WIFI_HPP
#define WIFI_HPP

#include <ESP8266WiFi.h>
#include <string>
#include "globals.hpp"
using std::string;

template <typename F>
void connectToWifiHelper(F f){
    f(ssid);

    WiFi.begin(ssid, password);

    while(WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        f(".");
    }

    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

template <typename F>
void connectToWifi(F f){
    connectToWifiHelper([&](string s)->void{
        Serial.println(s.c_str());
        f(s);
    });
}

void connectToWifi(){
    connectToWifiHelper([&](string s)->void{
        Serial.println(s.c_str());
    });
}

#endif

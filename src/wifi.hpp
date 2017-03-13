#ifndef WIFI_HPP
#define WIFI_HPP

#include <ESP8266WiFi.h>
#include "globals.hpp"

void connectToWifi(void(*print)(const char*)){
    Serial.println(ssid);
    print(ssid);

    WiFi.begin(ssid, password);

    while(WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
        print(".");
    }

    Serial.println("WiFi connected");
    print("WiFi Connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

//// TODO - do something with this function
////setup NTP time stuff here
//void setupNTP()
//{
//    NTP.onNTPSyncEvent([](NTPSyncEvent_t error) {
//        if (error) {
//            Serial.print("Time Sync error: ");
//            if (error == noResponse)
//                Serial.println("NTP server not reachable");
//            else if (error == invalidAddress)
//                Serial.println("Invalid NTP server address");
//        }
//        else {
//            Serial.print("Got NTP time: ");
//            Serial.println(NTP.getTimeDateString(NTP.getLastNTPSync()));
//        }
//    });
//
//    NTP.begin("pool.ntp.org", -5, true);
//    NTP.setInterval(1800);
//}

#endif

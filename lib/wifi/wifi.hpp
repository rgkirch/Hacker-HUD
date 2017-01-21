#ifndef CLION_WIFI_HPP
#define CLION_WIFI_HPP

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>

//#include <usb_serial.h>
//#include <core_pins.h>
#include "../ntptime/NtpClientLib.h"

extern int vfdPrint(const unsigned char *c, int n);

const char ssid[] = "HellSpot Slow";
const char password[] = "ILikeWiFi";

//Initialize WiFi Connection -----
void InitializeWiFi(){
    const unsigned char connecting_to[] = "Connecting to ";
    Serial.print("Connecting to ");
    vfdPrint(connecting_to, strlen((const char *) connecting_to));
    Serial.println(ssid);
    vfdPrint((const unsigned char *) ssid, strlen(ssid));

    WiFi.begin(ssid, password);

    while(WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    vfdPrint((const unsigned char *) "WiFi Connected", 14);
    delay(4000);
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    //setup NTP time stuff here
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
}


#endif

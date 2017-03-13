#include "wifi.hpp"
#include "../../src/globals.hpp"

template <typename T>
void connectToWifi(std::function<void(T)> print){
    Serial.println(ssid);
    print<const char *>(ssid);

    WiFi.begin(ssid, password);

    while(WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
        print<char>('.');
    }

    Serial.println("WiFi connected");
    print<char const*>("WiFi Connected");
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

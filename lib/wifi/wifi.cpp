#include "wifi.hpp"

void InitializeWiFi(){
    Serial.println(ssid);
    vfdPrint(ssid, strlen(ssid));

    WiFi.begin(ssid, password);

    while(WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
        vfdPrint(".", 1);
    }
    vfdPrint("connected", strlen("connected"));

    Serial.println("WiFi connected");
    vfdPrint("WiFi Connected", 14);
    delay(4000);
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

// TODO - do something with this function
//setup NTP time stuff here
void setupNTP()
{
    NTP.onNTPSyncEvent([](NTPSyncEvent_t error) {
        if (error) {
            Serial.print("Time Sync error: ");
            if (error == noResponse)
                Serial.println("NTP server not reachable");
            else if (error == invalidAddress)
                Serial.println("Invalid NTP server address");
        }
        else {
            Serial.print("Got NTP time: ");
            Serial.println(NTP.getTimeDateString(NTP.getLastNTPSync()));
        }
    });

    NTP.begin("pool.ntp.org", -5, true);
    NTP.setInterval(1800);
}

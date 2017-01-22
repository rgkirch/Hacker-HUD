#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>

#include <Adafruit_MCP9808.h>
#include <Wire.h>

#include <NtpClientLib.h>
#include "../lib/vfd/vfd.hpp"
#include "../lib/vfd/vfdBuilder.h"
#include "../lib/wifi/wifi.hpp"

typedef unsigned char uint8_t;


const char* host = "api.coindesk.com";
const char* hostEth = "api.nanopool.org";
const char* hostTime = "script.google.com";

VFD* myVFD;
int vfdPrint(const char *c, int n)
{
    if(myVFD == nullptr)
    {
        return 0;
    } else {
        return myVFD->print(c, n);
    }
}

void setup() {
    vfdBuilder myVfdBuilder;
    myVfdBuilder.setRx(D5);
    myVfdBuilder.setTx(D6);
    myVfdBuilder.setDisplayWidth(20);
    myVfdBuilder.setDisplayHeight(2);
    myVFD = myVfdBuilder.buildVFD();
//    myVFD = new VFD(14, 12);
    Serial.begin(115200);
    yield();
    InitializeWiFi();
}

void loop() {
    yield();
}

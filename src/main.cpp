#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>

#include <Adafruit_MCP9808.h>
#include <Wire.h>

#include <NtpClientLib.h>
#include "../lib/vfd/vfd.hpp"
#include "../lib/vfd/vfdBuilder.hpp"
#include "../lib/wifi/wifi.hpp"
#include "../lib/scrape-web/scrapeWeb.hpp"
#include "globals.hpp"
#include <NtpClientLib.h>

typedef unsigned char uint8_t;

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
void printEspInfo()
{
    Serial.println(ESP.getVcc());
    Serial.println(ESP.getFreeHeap());
    Serial.println(ESP.getChipId());
    Serial.println(ESP.getSdkVersion());
    Serial.println(ESP.getCoreVersion());
    Serial.println(ESP.getBootVersion());
    Serial.println(ESP.getBootMode());
    Serial.println(ESP.getCpuFreqMHz());
    Serial.println(ESP.getFlashChipId());
    Serial.println(ESP.getFlashChipRealSize()); //gets the actual chip size based on the flash id
    Serial.println(ESP.getFlashChipSize()); //gets the size of the flash as set by the compiler
    Serial.println(ESP.getFlashChipSpeed());
    Serial.println(ESP.getFlashChipMode());
    Serial.println(ESP.getFlashChipSizeByChipId());
}

void setup() {
    Serial.begin(115200);
//    printEspInfo();
    vfdBuilder myVfdBuilder;
    myVfdBuilder.setRx(D5);
    myVfdBuilder.setTx(D6);
    myVfdBuilder.setDisplayWidth(20);
    myVfdBuilder.setDisplayHeight(2);
    myVFD = myVfdBuilder.buildVFD();
    connectToWifi();
//    getJsonValue("norvig.com", "big.txt");
    yield();
}

void loop() {
    if(WiFi.status() != WL_CONNECTED) connectToWifi();
    std::string host = {"api.coindesk.com"};
    std::string price = {"v1/bpi/currentprice.json"};
    std::string rateFloat = {"rate_float"};
    getJsonValue(false, host, price, rateFloat);
//    get(host, price);
    delay(5000);
    yield();
}

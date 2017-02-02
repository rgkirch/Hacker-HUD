#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>

//#include <Adafruit_MCP9808.h>
//#include <Wire.h>

#include <NtpClientLib.h>
#include "../lib/vfd/vfd.hpp"
#include "../lib/wifi/wifi.hpp"
#include "../lib/scrape-web/scrapeWeb.hpp"
#include "globals.hpp"
#include "../lib/scrape-web/site.hpp"

typedef unsigned char uint8_t;

void *memchr(const void *s, int c, size_t n)
{
    unsigned char *p = (unsigned char*)s;
    while( n-- )
        if( *p != (unsigned char)c )
            p++;
        else
            return p;
    return 0;
}

std::unique_ptr<VFD> myVFD;
std::unique_ptr<Site> coindesk;
std::unique_ptr<Site> etheriumHashRate;
std::unique_ptr<Site> etheriumPrice;
std::unique_ptr<Site> github;

int vfdPrint(const char *c, int n)
{
    if(myVFD == nullptr)
    {
        return 0;
    } else {
        return myVFD->print(c, n);
    }
}
void serialPrint(std::string str)
{
    for (char c:str)
    {
        Serial.print(c);
    }
}
void serialPrint(const char* str)
{
    if(str != nullptr) Serial.print(str);
}
void serialPrintln(std::string str)
{
    serialPrint(str);
    Serial.println();
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

int myIndex = 0;
char fox[100] = "the quick brown fox jumps over the lazy dog";

void setup()
{
    Serial.begin(115200);
//    printEspInfo();
    myVFD = VFD::Builder().setRx(D5).setTx(D6).setDisplayWidth(20).setDisplayHeight(2).build();
    coindesk = Site::Builder().setHost(std::string("api.coindesk.com")).setPath(std::string("v1/bpi/currentprice.json")).setSecure(false).build();
    etheriumHashRate = Site::Builder().setHost(std::string("api.nanopool.org")).setPath(std::string("v1/eth/avghashratelimited/0x884e51352e7c68BfC9bA230f487be963a11de11B/1")).setSecure(true).build();
//jsonThing ethereumHashesJson = {"data",6,12};
    etheriumPrice = Site::Builder().setHost(std::string("api.nanopool.org")).setPath(std::string("v1/eth/prices")).setSecure(true).build();
    github = Site::Builder().setHost(std::string("api.github.com")).setPath(std::string("")).setSecure(true).build();
//jsonThing etheriumJson {"price_usd", 11, 16};
//    connectToWifi();
//    getJsonValue("norvig.com", "big.txt");
    yield();
}

void loop()
{
    if(WiFi.status() != WL_CONNECTED) connectToWifi();
    std::string rateFloat = {"rate_float"};
    std::string str;
//    str = get(std::string("api.coindesk.com"), std::string("v1/bpi/currentprice.json"), false);
//    str = get(std::string("api.github.com"), std::string(""), true);
//    str = get(std::string("api.github.com"), std::string("/repos/esp8266/Arduino/commits/master/status"), true);
//    str = get(std::string("api.nanopool.org"), std::string("v1/eth/avghashratelimited/0x884e51352e7c68BfC9bA230f487be963a11de11B/1"), true);
//    str = get(std::string("api.nanopool.org"), std::string("v1/eth/prices"), true);
//    str = get(std::string("httpbin.org"), std::string("ip"), false);
//    str = get(std::string("httpbin.org"), std::string("ip"), true);
//    str = getJsonValue(*etheriumHashRate, std::string("data"));
//    str = getJsonValue(*etheriumPrice, std::string("price_usd"));
//    str = getJsonValue(*github, std::string("message"));
//    serialPrintln(str);
//    myVFD->println(getJsonValue(*coindesk, rateFloat));
    myVFD->print(fox[myIndex]);
    myIndex++;
    if(myIndex == 60) myIndex = 0;
    delay(300);
    yield();
}

// wow, that's a lot of wine. I'm forgetting to breathe
// so, it's...  hang one. woa. umm.ubuc cd5520 <- that shit fuckccc

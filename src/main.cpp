#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <TimeLib.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>

//#include <Adafruit_MCP9808.h>
//#include <Wire.h>

#include "../lib/vfd/vfd.hpp"
#include "../lib/wifi/wifi.hpp"
#include "../lib/scrape-web/scrapeWeb.hpp"
#include "globals.hpp"
#include "../lib/scrape-web/site.hpp"
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif
//#include <sstream>
#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))

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
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

time_t ntpSyncProvider()
{
    return timeClient.getEpochTime();
}

template <typename T> void serialPrint(T t)
{
    Serial.print(t);
}
template <typename T> void serialPrintln(T t)
{
    Serial.println(t);
}
void serialPrint(std::string str)
{
    for (char c:str)
    {
        Serial.print(c);
    }
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

char fox[100] = "the quick brown fox jumps over the lazy dog";

void setup()
{
    Serial.begin(115200);
    ntpUDP.begin(2390);
    setSyncProvider(&ntpSyncProvider);
    timeClient.begin();
    timeClient.forceUpdate();


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
    timeClient.update();
    myVFD->clear();
    myVFD->home();
//    std::stringstream out;
//    out << hour() << ":" << minute() << ":" << second() << std::endl << timeClient.getEpochTime();
    *myVFD << hour() << ":" << minute() << ":" << second() << "\x0A\x0D" << timeClient.getEpochTime();
//    myVFD->print(out.str());
//    serialPrintln(out.str());
    delay(200);
    yield();
}
// todo -
// cd5520

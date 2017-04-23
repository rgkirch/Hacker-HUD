#include <ESP8266WiFi.h>
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif
//#include <sstream>
//#include <functional>
#include <vector>
#include <climits>
#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <Arduino.h>
#include <Adafruit_MCP9808.h>
#include <NtpClientLib.h>
//#include <json.hpp>

//#include <Adafruit_MCP9808.h>
//#include <Wire.h>
#include "site.hpp"
#include "vfd.hpp"
#include "wifi.hpp"
#include "scrapeWeb.hpp"
#include "option.hpp"

extern "C" {
#include "user_interface.h"
}

//std::string ntpTime;
time_t unixTime;
time_t unixTimeUpdated;

os_timer_t myTimer;
VFD *myVFD;
Adafruit_MCP9808 tempsensor;// = Adafruit_MCP9808(); //for MCP9808
Option<std::string> emptyStringOption;
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
//setup ntp time -----
void ntpSetup() {
    NTP.onNTPSyncEvent([&](NTPSyncEvent_t error) {
        Serial.println("print from ntp sync callback lambda");
        if (error) {
            Serial.print("Time Sync error: ");
            if (error == noResponse)
                Serial.println("NTP server not reachable");
            else if (error == invalidAddress)
                Serial.println("Invalid NTP server address");
        } else {
            Serial.print("Got NTP time: ");
            Serial.println(NTP.getTimeDateString(NTP.getLastNTPSync()));
        }
    });

    NTP.begin("pool.ntp.org", -5, true);
    NTP.setInterval(1800);
}
//Initialize Temp Sensor Fnc. -----
void initializeTemp(Adafruit_MCP9808 &tempsensor){
    // Make sure the sensor is found, you can also pass in a different i2c
    // address with tempsensor.begin(0x19) for example
    if (!tempsensor.begin()) {
        Serial.println("Couldn't find MCP9808!");
    }
    pinMode(A0, INPUT);
}
//Read Temp Fnc. -----
float readTemp(Adafruit_MCP9808 &tempsensor) {
    tempsensor.shutdown_wake(0);   // Don't remove this line! required before reading temp

    // Read and convert to *F, then print
    float c = tempsensor.readTempC();
    float f = c * 9.0 / 5.0 + 32;
    Serial.print("C: ");
    Serial.print(c);
    Serial.print(" F: ");
    Serial.print(f);
    delay(250);

//    Serial.println("Shutdown MCP9808.... ");
    tempsensor.shutdown_wake(1); // shutdown MSP9808 - power consumption ~0.1 mikro Ampere
    return f;
//    float temp = analogRead(A0);
//    temp = (((temp/1023)*3.3*100)*1.8) + 32;
}
struct Site coindesk {
        60000,
        INT_MIN,
        httpPort,
        "api.coindesk.com",
        "v1/bpi/currentprice.json",
        emptyStringOption,
        {"bpi", "USD", "rate_float"}
};
struct Site coinMarketCap = {
        .updateInterval = 60000,
        .lastUpdated = INT_MIN,
        .port = httpsPort,
        .host = "coinmarketcap-nexuist.rhcloud.com",
        .path = "/api/eth",
        emptyStringOption,
        .keys = {"price", "usd"}
};
struct Site openWeatherMapHumidity = {
        .updateInterval = 60000,
        .lastUpdated = INT_MIN,
        .port = httpPort,
        .host = "api.openweathermap.org",
        .path = "/data/2.5/weather?q=Tampa,us&units=imperial&APPID=f8ffd4de380fb081bfc12d4ee8c82d29",
        emptyStringOption,
        .keys = {"main", "humidity"}
};
struct Site openWeatherMapTemp = {
        .updateInterval = 60000,
        .lastUpdated = INT_MIN,
        .port = httpPort,
        .host = "api.openweathermap.org",
        .path = "/data/2.5/weather?q=Tampa,us&units=imperial&APPID=f8ffd4de380fb081bfc12d4ee8c82d29",
        emptyStringOption,
        .keys = {"main", "temp"}
};
std::string applyKeys(const JsonObject& o, const std::vector<std::string>::iterator begin, const std::vector<std::string>::iterator end)
{
    auto it = begin;
    if (std::next(it) == end)
    {
        return o[(*it).c_str()].as<const char*>();
    } else {
        return applyKeys(o[(*it).c_str()], std::next(begin), end);
    }
}
Option<std::string> getSiteData(struct Site site)
{
    Option<std::string> o = scrapeJson(site);
    DynamicJsonBuffer jsonBuffer(1000);
        std::function<std::string(std::string)> f([&o, &jsonBuffer, &site](std::string str)->std::string {
            return applyKeys(jsonBuffer.parseObject(str.c_str()), site.keys.begin(), site.keys.end());
        } );
        o.map( f );
    return o;
}
void updateSite(struct Site &site)
{
    if (millis() - site.lastUpdated > site.updateInterval)
    {
        Option<std::string> o = getSiteData(site);
        if (not o.isEmpty()) {
            site.lastResult = o;
            site.lastUpdated = millis();
        }
    }
}
void timerCallback(void *pArg) {
    char buffer[20] = {0};
    char unixBuffer[20] = {0};
    strncpy(buffer, NTP.getTimeStr(unixTime + (millis() - unixTimeUpdated) / 1000).c_str(), 20);
    int len = strlen(buffer);
    memset(&buffer[len], ' ', max(0, 20 - len));
//    snprintf(buffer, 20, "%d:%d:%d", hourFormat12(unixTime), minute(unixTime), second(unixTime));
    myVFD->setUpperLine(buffer);
//    snprintf(unixBuffer, 20, "%d", unixTime);
//    myVFD->setLowerLine(unixBuffer);
}
void setup()
{
    Serial.begin(115200);
    myVFD = VFD::Builder().setRx(D5).setTx(D6).setDisplayWidth(20).setDisplayHeight(2).build();
    ntpSetup();
    initializeTemp(tempsensor);
    myVFD->clear();
    myVFD->home();

    os_timer_setfn(&myTimer, (os_timer_func_t *)timerCallback, NULL);
    os_timer_arm(&myTimer, 1000, true);
}
//void p(const char *cs)
//{
//    myVFD->print(cs);
//}
void loop()
{
//    if(WiFi.status() != WL_CONNECTED) connectToWifi(std::function<void(std::string)> {[](std::string str)->void { myVFD->print(str); }});
    if(WiFi.status() != WL_CONNECTED) connectToWifi();
    unixTime = NTP.getTime();
    unixTimeUpdated = millis();

//    updateSite(coinMarketCap);
//    updateSite(openWeatherMapTemp);
//    updateSite(openWeatherMapHumidity);

//    myVFD->clear();
//    myVFD->home();

    updateSite(coindesk);
    myVFD->setLowerLine("eth", coindesk.lastResult.getOrElse("no data"));
//    myVFD->setLowerLine(coindesk.lastResult.getOrElse("no data"));
//    delay(4000);

//    myVFD->print("btc      ");
//    myVFD->println(coindesk.lastResult.getOrElse("no data"));
//    myVFD->print("eth      ");
//    myVFD->print(coinMarketCap.lastResult.getOrElse("no data"));


//    delay(10000);

//    myVFD->clear();
//    myVFD->home();
//    myVFD->print("temp     "); //(char)223)
//    myVFD->println(openWeatherMapTemp.lastResult.getOrElse("no data"));
//    myVFD->print("humidity ");
//    myVFD->print(openWeatherMapHumidity.lastResult.getOrElse("no data"));

//    delay(10000);

//    myVFD->clear();
//    myVFD->home();
//    myVFD->print("sensor temp "); //(char)223)
//    myVFD->println(readTemp(tempsensor));
//    myVFD->print("time "); //(char)223)

//    ntpTime = NTP.getTimeStr().c_str();
//    unixTime = NTP.getTime();
    delay(10000);

    yield();
}
// cd5220
// JsonObject.success()
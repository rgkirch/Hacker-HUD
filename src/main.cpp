#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <functional>
//#include <json.hpp>

//#include <Adafruit_MCP9808.h>
//#include <Wire.h>

#include "site.hpp"
#include "vfd.hpp"
#include "wifi.hpp"
#include "scrapeWeb.hpp"
//#ifdef min
//#undef min
//#endif
//#ifdef max
//#undef max
//#endif
//#include <sstream>
//#define min(a,b) ((a)<(b)?(a):(b))
//#define max(a,b) ((a)>(b)?(a):(b))

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

VFD *myVFD;

std::string coindesk()
{
    Site coindesk {.host = "api.coindesk.com", .path = "v1/bpi/currentprice.json", .port = httpPort};
    DynamicJsonBuffer jsonBuffer(1000);
    return jsonBuffer.parseObject(scrapeJson(coindesk).getOrElse("").c_str())["bpi"]["USD"]["rate_float"].as<const char*>();
}
std::string coinMarketCap()
{
    Site coinMarketCap = {.host = "coinmarketcap-nexuist.rhcloud.com", .path = "/api/eth", .port = httpsPort};
    DynamicJsonBuffer jsonBuffer(1000);
    return jsonBuffer.parseObject(scrapeJson(coinMarketCap).getOrElse("").c_str())["price"]["usd"].as<const char*>();
}
std::string openWeatherMapHumidity()
{
    Site openWeatherMap = {.host = "api.openweathermap.org", .path = "/data/2.5/weather?q=Tampa,us&units=imperial&APPID=f8ffd4de380fb081bfc12d4ee8c82d29", .port = httpPort};
    DynamicJsonBuffer jsonBuffer(1000);
    return jsonBuffer.parseObject(scrapeJson(openWeatherMap).getOrElse("").c_str())["main"]["humidity"].as<const char*>();
}
std::string openWeatherMapTemp()
{
    Site openWeatherMap = {.host = "api.openweathermap.org", .path = "/data/2.5/weather?q=Tampa,us&units=imperial&APPID=f8ffd4de380fb081bfc12d4ee8c82d29", .port = httpPort};
    DynamicJsonBuffer jsonBuffer(1000);
    return jsonBuffer.parseObject(scrapeJson(openWeatherMap).getOrElse("").c_str())["main"]["temp"].as<const char*>();
}

void setup()
{
    Serial.begin(115200);
    myVFD = VFD::Builder().setRx(D5).setTx(D6).setDisplayWidth(20).setDisplayHeight(2).build();
    myVFD->home();
    myVFD->clear();
}
void p(const char *cs)
{
    myVFD->print(cs);
}
void loop()
{
//    if(WiFi.status() != WL_CONNECTED) connectToWifi(std::function<void(std::string)> {[](std::string str)->void { myVFD->print(str); }});
    if(WiFi.status() != WL_CONNECTED) connectToWifi(p);
    myVFD->clear();
    myVFD->home();
    myVFD->print("btc      ");
    myVFD->println(coindesk());
    myVFD->print("eth      ");
    myVFD->print(coinMarketCap());
    delay(10000);
    myVFD->home();
    myVFD->print("temp     ");
    myVFD->println(openWeatherMapTemp());
    myVFD->print("humidity ");
    myVFD->print(openWeatherMapHumidity());
    delay(10000);
}
// cd5220

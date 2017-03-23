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
#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
//#include <json.hpp>

//#include <Adafruit_MCP9808.h>
//#include <Wire.h>
#include "site.hpp"
#include "vfd.hpp"
#include "wifi.hpp"
#include "scrapeWeb.hpp"
#include "option.hpp"

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
Site coindesk {
        .updateInterval = 60000,
        .lastUpdated = (int)0xFFFFFFFF,
        .port = httpPort,
        .host = "api.coindesk.com",
        .path = "v1/bpi/currentprice.json",
        .lastResult = Option<std::string>(),
        .keys = {"bpi", "USD", "rate_float"}
};
Site coinMarketCap = {
        .updateInterval = 60000,
        .lastUpdated = (int)0xFFFFFFFF,
        .port = httpsPort,
        .host = "coinmarketcap-nexuist.rhcloud.com",
        .path = "/api/eth",
        .lastResult = Option<std::string>(),
        .keys = {"price", "usd"}
};
Site openWeatherMapHumidity = {
        .updateInterval = 60000,
        .lastUpdated = (int)0xFFFFFFFF,
        .port = httpPort,
        .host = "api.openweathermap.org",
        .path = "/data/2.5/weather?q=Tampa,us&units=imperial&APPID=f8ffd4de380fb081bfc12d4ee8c82d29",
        .lastResult = Option<std::string>(),
        .keys = {"main", "humidity"}
};
Site openWeatherMapTemp = {
        .updateInterval = 60000,
        .lastUpdated = (int)0xFFFFFFFF,
        .port = httpPort,
        .host = "api.openweathermap.org",
        .path = "/data/2.5/weather?q=Tampa,us&units=imperial&APPID=f8ffd4de380fb081bfc12d4ee8c82d29",
        .lastResult = Option<std::string>(),
        .keys = {"main", "temp"}
};
std::string apply(JsonObject& o, std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end)
{
    if (std::next(begin) == end)
    {
        return o[(*begin).c_str()].as<const char*>();
    } else {
        return apply(o[(*begin).c_str()], std::next(begin), end);
    }
}
Option<std::string> get(Site site)
{
    DynamicJsonBuffer jsonBuffer(1000);
    return apply(jsonBuffer.parseObject(scrapeJson(site)), site.keys.begin(), site.keys.end());
}
void update(Site site)
{
    if (millis() - site.lastUpdated > site.updateInterval)
    {
        site.lastUpdated = millis();
        Option<std::string> o = get(site);
        if (not o.isEmpty()) {
            site.lastResult = get(site).getOrElse("").c_str();
        }
    }
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

    update(coindesk);
    update(coinMarketCap);
    myVFD->clear();
    myVFD->home();
    myVFD->print("btc      ");
    myVFD->println(coindesk.lastResult.getOrElse("no data"));
    myVFD->print("eth      ");
    myVFD->print(coinMarketCap.lastResult.getOrElse("no data"));

    update(openWeatherMapTemp);
    update(openWeatherMapHumidity);
    delay(10000);

    myVFD->home();
    myVFD->clear();
    myVFD->print("temp     ");
    myVFD->println(openWeatherMapTemp.lastResult.getOrElse("no data"));
    myVFD->print("humidity ");
    myVFD->print(openWeatherMapHumidity.lastResult.getOrElse("no data"));
    delay(10000);
}
// cd5220

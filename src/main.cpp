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
Option<std::string> emptyStringOption;
Site coindesk {
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
Option<std::string> &getSiteData(struct Site site)
{
    Option<std::string> o = scrapeJson(site);
    DynamicJsonBuffer jsonBuffer(1000);
    return o.map(
        [&o, &jsonBuffer, &site](std::string str)->std::string {
            applyKeys(jsonBuffer.parseObject(o.getOrElse("").c_str()), site.keys.begin(), site.keys.end());
        }
    );
}
void updateSite(struct Site site)
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

    updateSite(coindesk);
    updateSite(coinMarketCap);
    myVFD->clear();
    myVFD->home();
    myVFD->print("btc      ");
    myVFD->println(coindesk.lastResult.getOrElse("no data"));
    myVFD->print("eth      ");
    myVFD->print(coinMarketCap.lastResult.getOrElse("no data"));

    updateSite(openWeatherMapTemp);
    updateSite(openWeatherMapHumidity);
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

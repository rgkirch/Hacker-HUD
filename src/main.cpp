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
std::string apply(JsonObject& o, std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end)
{
    if (std::next(begin) == end)
    {
        return o[(*begin).c_str()].as<const char*>();
    } else {
        return apply(o[(*begin).c_str()], std::next(begin), end);
    }
}
std::string get(Site site)
{
    DynamicJsonBuffer jsonBuffer(1000);
    return apply(jsonBuffer.parseObject(scrapeJson(site).getOrElse("").c_str()), site.keys.begin(), site.keys.end());
}

VFD *myVFD;

int updateFrequency = 60000;
std::string coindesk()
{
    static std::string str;
    static int updated = -60000;
    Site coindesk {
            .host = "api.coindesk.com",
            .path = "v1/bpi/currentprice.json",
            .port = httpPort,
            .keys = std::initializer_list<std::string> {"bpi", "USD", "rate_float"}
    };
    if (millis() - updated > updateFrequency)
    {
        str = get(coindesk);
        updated = millis();
    }
    return str;
}
std::string coinMarketCap()
{
    static std::string str;
    static int updated = -60000;
    Site coinMarketCap = {
            .host = "coinmarketcap-nexuist.rhcloud.com",
            .path = "/api/eth",
            .port = httpsPort,
            .keys = std::initializer_list<std::string> {"price", "usd"}
    };
    if (millis() - updated > updateFrequency)
    {
        DynamicJsonBuffer jsonBuffer(1000);
        str = get(coinMarketCap);
        updated = millis();
    }
    return str;
}
std::string openWeatherMapHumidity()
{
    static std::string str;
    static int updated = -60000;
    Site openWeatherMapHumidity = {
            .host = "api.openweathermap.org",
            .path = "/data/2.5/weather?q=Tampa,us&units=imperial&APPID=f8ffd4de380fb081bfc12d4ee8c82d29",
            .port = httpPort,
            .keys = std::initializer_list<std::string> {"main", "humidity"}
    };
    if (millis() - updated > updateFrequency)
    {
        DynamicJsonBuffer jsonBuffer(1000);
        str = get(openWeatherMapHumidity);
        updated = millis();
    }
    return str;
}
std::string openWeatherMapTemp()
{
    static std::string str;
    static int updated = -60000;
    Site openWeatherMapTemp = {
            .host = "api.openweathermap.org",
            .path = "/data/2.5/weather?q=Tampa,us&units=imperial&APPID=f8ffd4de380fb081bfc12d4ee8c82d29",
            .port = httpPort,
            .keys = std::initializer_list<std::string> {"main", "temp"}
    };
    if (millis() - updated > updateFrequency)
    {
        DynamicJsonBuffer jsonBuffer(1000);
        str = get(openWeatherMapTemp);
        updated = millis();
    }
    return str;
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
    myVFD->clear();
    myVFD->print("temp     ");
    myVFD->println(openWeatherMapTemp());
    myVFD->print("humidity ");
    myVFD->print(openWeatherMapHumidity());
    delay(10000);
}
// cd5220

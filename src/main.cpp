#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
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

//class Test {};
//std::string coindeskShit() {
//    return o["bpi"]["USD"]["rate_float"];
//}

//std::function<std::string(Test)> test = std::function<std::string(Test)>([](Test o){ return std::string("hello"); } );
//std::function<std::string(JsonObject)> test = std::function<std::string(JsonObject)>([](JsonObject o){ return o["bpi"]["USD"]["rate_float"].as<std::string>(); } );
//Site coindesk {.host = "api.coindesk.com", .path = "v1/bpi/currentprice.json", .port = httpsPort, .f = std::function<std::string(JsonObject)>([](JsonObject o)->std::string { return std::string(o["bpi"]["USD"]["rate_float"].as<const char*>); }) };
Site coindesk {.host = "api.coindesk.com", .path = "v1/bpi/currentprice.json", .port = httpPort};
//auto f = std::function<const char*(JsonObject)>([](JsonObject o){ return std::string(o["bpi"]["USD"]["rate_float"].as<const char*>()); } );
//auto f = std::function<const char*(JsonObject)>([](JsonObject o){ return std::string(o["bpi"]["USD"]["rate_float"].as<const char*>()); } );
//Site coindesk {.host = "api.coindesk.com", .path = "v1/bpi/currentprice.json", .port = httpsPort, .f = std::function<std::string(JsonObject)>([](JsonObject o)->std::string { return std::string(o["bpi"]["USD"]["rate_float"].as<const char*>); }) };
//Site coinMarketCap {.host = "coinmarketcap-nexuist.rhcloud.com", .path = "/api/eth", .port = httpsPort, .f = [](JsonObject o)->float { return o["price"]["usd"]; } };

void p(const char *cs)
{
    myVFD->print(cs);
}
void (*f)(const char*) = p;

void setup()
{
    Serial.begin(115200);
    myVFD = VFD::Builder().setRx(D5).setTx(D6).setDisplayWidth(20).setDisplayHeight(2).build();
    myVFD->home();
    myVFD->clear();
}

void loop()
{
//    if(WiFi.status() != WL_CONNECTED) connectToWifi(std::function<void(std::string)> {[](std::string str)->void { myVFD->print(str); }});
    if(WiFi.status() != WL_CONNECTED) connectToWifi(f);
    myVFD->home();
    std::string str = scrapeJson(coindesk).getOrElse("");
//    myVFD->print(str);
//    Serial.println(str.c_str());
    DynamicJsonBuffer jsonBuffer(1000);
    JsonObject& root = jsonBuffer.parseObject(str.c_str());
    myVFD->print(root["bpi"]["USD"]["rate_float"].as<const char*>());
    delay(30000);
}
// cd5220

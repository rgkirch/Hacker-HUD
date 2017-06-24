#include <ESP8266WiFi.h>
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif
#include <vector>
#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
#include <ArduinoJson.h>
#include <Adafruit_MCP9808.h>
#include <NtpClientLib.h>
#include <ESP8266HTTPClient.h>

#include "site.hpp"
#include "wifi.hpp"
#include "display.h"
#include "myConcreteSerial.hpp"

#define DELAY 4000

#define DEBUGPRINT
#ifdef DEBUGPRINT
#define LOG(x) do{Serial.println(x);}while(0)
#else
#define LOG(x)
#endif

using std::string;
using std::next;
using std::vector;
using std::function;

//string ntpTime;
time_t unixTime;
time_t unixTimeUpdated;

os_timer_t myTimer;
MyConcretePrint serial(D5, D6);
VFD myVFD(20, 2, &serial);
Adafruit_MCP9808 tempsensor;// = Adafruit_MCP9808(); //for MCP9808
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
        LOG("print from ntp sync callback lambda");
        if (error) {
            LOG("Time Sync error: ");
            if (error == noResponse)
                LOG("NTP server not reachable");
            else if (error == invalidAddress)
                LOG("Invalid NTP server address");
        } else {
            LOG("Got NTP time: ");
            LOG(NTP.getTimeDateString(NTP.getLastNTPSync()));
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
Site coindesk {
        httpPort,
        "api.coindesk.com",
        "/v1/bpi/currentprice.json",
        {"bpi", "USD", "rate_float"}
};
Site coinMarketCap {
        httpsPort,
        "coinmarketcap-nexuist.rhcloud.com",
        "/api/eth",
        {"price", "usd"}
};
Site openWeatherMapHumidity {
        httpPort,
        "api.openweathermap.org",
        "/data/2.5/weather?q=Tampa,us&units=imperial&APPID=f8ffd4de380fb081bfc12d4ee8c82d29",
        {"main", "humidity"}
};
Site openWeatherMapTemp {
        httpPort,
        "api.openweathermap.org",
        "/data/2.5/weather?q=Tampa,us&units=imperial&APPID=f8ffd4de380fb081bfc12d4ee8c82d29",
        {"main", "temp"}
};
Option<string> applyKeys(const JsonObject& o, const vector<string>::iterator begin, const vector<string>::iterator end)
{
    LOG("apply keys");
    Option<string> emptyOption;
    auto it = begin;
    if (next(it) == end)
    {
        const char *r = o[(*it).c_str()].as<const char*>();
        if (r == nullptr) {
            return emptyOption;
        } else return string(r);
    } else {
        return applyKeys(o[(*it).c_str()], next(begin), end);
    }
}
Option<string> getSiteData(Site site)
{
    HTTPClient http;
    string server;
    site.port == httpsPort ? server.append("https://") : server.append("http://");
    server.append(site.host).append(site.path);
    http.begin(server.c_str());
    int httpCode = http.GET();
    LOGN(httpCode);
    LOG(HTTP_CODE_OK);
    string data = http.getString().c_str();
    http.end();
    LOGN("data ");
    LOG(data.c_str());
    LOG("search for the left brace");
    int i = data.find('{');
    if (i == data.npos) {
        i = 0;
    }
    LOG("take the substring of the string");
    data = data.substr(i);
    LOG(data.c_str());
    Option<string> o(data);
    DynamicJsonBuffer jsonBuffer(2000);
    function<Option<string>(std::string)> f([&o, &jsonBuffer, &site](string str)->Option<string> {
        JsonObject &o = jsonBuffer.parseObject(str.c_str());
        if (not o.success()) return Option<string>();
        return applyKeys(o, site.keys.begin(), site.keys.end());
    } );
    LOG("json shit");
    o.map( f );
    return o;
}
void updateSite(Site &site)
{
    if (site.lastUpdated == 0 or millis() - site.lastUpdated > site.updateInterval)
    {
        LOG("update site");
        Option<string> o = getSiteData(site);
        site.lastResult = o;
        site.lastUpdated = millis();
    }
}
void timerCallback(void *pArg) {
    char buffer[20] = {0};
    char unixBuffer[20] = {0};
    strncpy(buffer, NTP.getTimeStr(unixTime + (millis() - unixTimeUpdated) / 1000).c_str(), 20);
    int len = strlen(buffer);
    memset(&buffer[len], ' ', max(0, 20 - len));
//    snprintf(buffer, 20, "%d:%d:%d", hourFormat12(unixTime), minute(unixTime), second(unixTime));
    myVFD.setUpperLine(buffer);
//    snprintf(unixBuffer, 20, "%d", unixTime);
//    myVFD.setLowerLine(unixBuffer);
}

void setup()
{
    Serial.begin(115200);
//    myVFD = VFD::Builder().setRx(D5).setTx(D6).setDisplayWidth(20).setDisplayHeight(2).build();
    ntpSetup();
//    initializeTemp(tempsensor);
    myVFD.clear();
    myVFD.home();

    os_timer_setfn(&myTimer, (os_timer_func_t *)timerCallback, NULL);
    os_timer_arm(&myTimer, 1000, true);
//    grid(myVFD);
}
//void p(const char *cs)
//{
//    myVFD.print(cs);
//}
void loop()
{
    if(WiFi.status() != WL_CONNECTED) connectToWifi();
    LOG("loop begin");
    unixTime = NTP.getTime();
    unixTimeUpdated = millis();

    LOG("coindesk");
    updateSite(coindesk);
    myVFD.setLowerLine("bitcoin", coindesk.lastResult.orElse("no data"));
    delay(DELAY);

    LOG("coinmarketcap");
    updateSite(coinMarketCap);
    myVFD.setLowerLine("etherium", coinMarketCap.lastResult.orElse("no data"));
    delay(DELAY);

    LOG("openweathermap");
    updateSite(openWeatherMapTemp);
    myVFD.setLowerLine("tampa temp", openWeatherMapTemp.lastResult.orElse("no data"));
    delay(DELAY);

    LOG("openweathermaphumidity");
    updateSite(openWeatherMapHumidity);
    myVFD.setLowerLine("tampa humidity", openWeatherMapHumidity.lastResult.orElse("no data"));
    delay(DELAY);

//    myVFD.clear();
//    myVFD.home();
//    myVFD.print("sensor temp "); //(char)223)
//    myVFD.println(readTemp(tempsensor));
//    myVFD.print("time "); //(char)223)

    yield();
}
// cd5220
// JsonObject.success()
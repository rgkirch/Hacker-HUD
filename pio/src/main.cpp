#include <ESP8266WiFi.h>
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif
#include <vector>
#include <climits>
#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
#include <ArduinoJson.h>
#include <Adafruit_MCP9808.h>
#include <NtpClientLib.h>

#include "site.hpp"
#include "wifi.hpp"
#include "display.h"
#include "myConcreteSerial.hpp"
#include "myConcreteConnection.hpp"
#include "myConcreteClient.hpp"

#define DEBUGPRINT
#ifdef DEBUGPRINT
#define LOG(x) do{Serial.println(x);}while(0)
#else
#define LOG(x)
#endif

#define DELAY 4000
#define UPDATE_INTERVAL 60000

#define DEBUGPRINT
#ifdef DEBUGPRINT
#define LOG(x) do{Serial.println(x);}while(0)
#else
#define LOG(x)
#endif

extern "C" {
#include "user_interface.h"
}

//std::string ntpTime;
time_t unixTime;
time_t unixTimeUpdated;

os_timer_t myTimer;
MyConcretePrint serial(D5, D6);
VFD myVFD(20, 2, &serial);
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
struct Site coindesk {
        .updateInterval = UPDATE_INTERVAL,
        .lastUpdated = 0,
        .port = httpPort,
        .host = "api.coindesk.com",
        .path = "v1/bpi/currentprice.json",
        emptyStringOption,
        .keys = {"bpi", "USD", "rate_float"}
};
struct Site coinMarketCap = {
        .updateInterval = UPDATE_INTERVAL,
        .lastUpdated = 0,
        .port = httpsPort,
        .host = "coinmarketcap-nexuist.rhcloud.com",
        .path = "/api/eth",
        emptyStringOption,
        .keys = {"price", "usd"}
};
struct Site openWeatherMapHumidity = {
        .updateInterval = UPDATE_INTERVAL,
        .lastUpdated = 0,
        .port = httpPort,
        .host = "api.openweathermap.org",
        .path = "/data/2.5/weather?q=Tampa,us&units=imperial&APPID=f8ffd4de380fb081bfc12d4ee8c82d29",
        emptyStringOption,
        .keys = {"main", "humidity"}
};
struct Site openWeatherMapTemp = {
        .updateInterval = UPDATE_INTERVAL,
        .lastUpdated = 0,
        .port = httpPort,
        .host = "api.openweathermap.org",
        .path = "/data/2.5/weather?q=Tampa,us&units=imperial&APPID=f8ffd4de380fb081bfc12d4ee8c82d29",
        emptyStringOption,
        .keys = {"main", "temp"}
};
Option<std::string> applyKeys(const JsonObject& o, const std::vector<std::string>::iterator begin, const std::vector<std::string>::iterator end)
{
    LOG("apply keys");
    Option<std::string> emptyOption;
    auto it = begin;
    if (std::next(it) == end)
    {
        const char *r = o[(*it).c_str()].as<const char*>();
        if (r == nullptr) {
            return emptyOption;
        } else return std::string(r);
    } else {
        return applyKeys(o[(*it).c_str()], std::next(begin), end);
    }
}
Option<std::string> getSiteData(struct Site site)
{
    LOG("get site data");
    MyConcreteClient client(site.port);
    MyConcreteConnection connection(&client, site.host, site.path);
    std::string data = connection.read();
    LOG(data.c_str());
    Option<std::string> o(data);
    DynamicJsonBuffer jsonBuffer(2000);
    std::function<Option<std::string>(std::string)> f([&o, &jsonBuffer, &site](std::string str)->Option<std::string> {
        JsonObject &o = jsonBuffer.parseObject(str.c_str());
        if (not o.success()) return Option<std::string>();
        return applyKeys(o, site.keys.begin(), site.keys.end());
    } );
    LOG("json shit");
    o.map( f );
    return o;
}
void updateSite(struct Site &site)
{
    LOG("update site");
    if (site.lastUpdated == 0 or millis() - site.lastUpdated > site.updateInterval)
    {
        Option<std::string> o = getSiteData(site);
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

    LOG("coinmargetcap");
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
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
struct Site weatherUnderground = {
        .updateInterval = 60000,
        .lastUpdated = INT_MIN,
        .port = httpPort,
        .host = "http://api.wunderground.com",
        .path = "/api/d8d4093b718cecaa/forecast/q/CA/San_Francisco.json",
        emptyStringOption,
        .keys = {"forecast", "txt_forecast", "forecastday", "0", "ftctext"}
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
Option<std::string> applyKeys(const JsonObject& o, const std::vector<std::string>::iterator begin, const std::vector<std::string>::iterator end)
{
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
    Option<std::string> o = scrapeJson(site);
    DynamicJsonBuffer jsonBuffer(20000);
    std::function<Option<std::string>(std::string)> f([&o, &jsonBuffer, &site](std::string str)->Option<std::string> {
        JsonObject &o = jsonBuffer.parseObject(str.c_str());
        if (not o.success()) return Option<std::string>();
        return applyKeys(o, site.keys.begin(), site.keys.end());
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
char toByte(int number) {
    return (number / 1000000 % 2 << 6) + (number / 100000 % 2 << 5) + (number / 10000 % 2 << 4) + (number / 1000 % 2 << 3) + (number / 100 % 2 << 2) + (number / 10 % 2 << 1) + (number % 2);
}
class Char {
public:
    Char(const char *c) {
        for (int i = 0; i < 5; ++i) {
            x[i] = c[i];
        }
    };
    Char(Char& c) {
        *this = c;
    };
    Char& operator=(const Char &c) {
        if (this != &c) {
            for (int i = 0; i < 5; ++i) {
                x[i] = c.x[i];
            }
        }
        return *this;
    };
    operator std::string() {
        return std::string(x);
    };
    Char& operator&=(const Char &c) {
        for (int i = 0; i < 5; ++i) {
            x[i] &= c.x[i];
        }
        return *this;
    };
    const Char& operator&(const Char &c) {
        return Char(*this) &= c;
    };
    Char& operator|=(const Char &c) {
        for (int i = 0; i < 5; ++i) {
            x[i] |= c.x[i];
        }
        return *this;
    };
    Char& operator-=(const Char &c) {
        return (Char(*this) &= c) ^= *this;
    };
    Char& operator^=(const Char &c) {
        for (int i = 0; i < 5; ++i) {
            x[i] ^= c.x[i];
        }
        return *this;
    };
    const Char& operator^(const Char &c) {
        return Char(*this) ^= c;
    };
    const Char& operator|(const Char &c) {
        return Char(*this) |= c;
    };
    bool operator==(const Char &c) {
        if (this == &c) return true;
        for (int i = 0; i < 5; ++i) {
            if (x[i] != c.x[i]) return false;
        }
        return true;
    };
    bool operator!=(const Char &c) {
        return !(*this == c);
    };
    Char& operator~() {
        for (int i = 0; i < 5; ++i) {
            x[i] = ~x[i];
        }
        return *this;
    };
    char x[5] {0};
};
// grid 15 rows and 19*7+5 or 138
void map(const char *cs, std::function<void(char)> f) {
    for (int i = 0; i < strlen(cs); i++) {
        f(cs[i]);
    }
}

class Grid {
public:
    const int width = 138;
    const int height = 15;
//    void setOn(int x, int y) {
//        data[(y * width + x) / 8] |= (1 << (8 - ((y * width + x) % 8)));
//    };
//    void setOff(int x, int y) {
//        data[(y * width + x) / 8] &= ~(1 << (8 - ((y * width + x) % 8)));
//    };
    std::string toString() {
        std::string str;
        for (int y = 0; y < 14; y++) {
            for (int x = 0; x < 20 * 5; x++) {
                if (data[x * 2 + y / 8] & 1 << (7 - (y % 8)) != 0) {
                    str += '1';
                } else {
                    str += '0';
                }
            }
            str += '\n';
        }
    };
    void setOn(int x, int y) {
        data[x / 8 + y / 2] |= (1 << (7 - ((y + 1) % 8)));
    };
    void setOff(int x, int y) {
        data[x / 8 + y / 2] &= ~(1 << (7 - ((y + 1) % 8)));
    };
    void dump(std::function<void(char)> print) {
        map("\x1B\x25\x01", print);
        map("\x1B\x26\x01", print);
        char first = '\x21';
        char last = first + 39;
        print(first);
        print(last);
        for (int x = 0; x < 40; x += 2) {
            print(data[x]);
        }
        for (int x = 1; x < 40; x += 2) {
            print(data[x]);
        }
    };
    char data[2 * 5 * 20] {0};
};
Grid grid;
char first = '\x21';
char last = first + 39;
char counter = 0;
void setup()
{
    Serial.begin(115200);
    myVFD = VFD::Builder().setRx(D5).setTx(D6).setDisplayWidth(20).setDisplayHeight(2).build();
    ntpSetup();
//    initializeTemp(tempsensor);
    myVFD->clear();
    myVFD->home();

    os_timer_setfn(&myTimer, (os_timer_func_t *)timerCallback, NULL);
    for (int x = 0; x < 20 * 5; x++) {
        for (int y = 14; y > x % 14; y--) {
            grid.setOn(x, y);
        }
    }
//    memset(grid.data, '\x88', 2 * 5 * 20);
//    grid.dump([](char x){myVFD->print(x);});
//    os_timer_arm(&myTimer, 1000, true);
    myVFD->print("\x1B\x25\x01");
    myVFD->print("\x1B\x26\x01");
    myVFD->print(first);
    myVFD->print(last);
    for (char a = first; a <= last; a++) {
        myVFD->print('\x05');
//        myVFD->print("\x63\x33\x15\x06\x03");
        for (int i = 0; i < 5; i++) {
            myVFD->print(counter++);
        }
    }
}
//void loop()
//{
////    if(WiFi.status() != WL_CONNECTED) connectToWifi(std::function<void(std::string)> {[](std::string str)->void { myVFD->print(str); }});
//    if(WiFi.status() != WL_CONNECTED) connectToWifi();
//    unixTime = NTP.getTime();
//    unixTimeUpdated = millis();
//
//    updateSite(coindesk);
//    myVFD->setLowerLine("bitcoin", coindesk.lastResult.getOrElse("no data"));
//    delay(4000);
//
//    updateSite(coinMarketCap);
//    myVFD->setLowerLine("etherium", coinMarketCap.lastResult.getOrElse("no data"));
//    delay(4000);
//
//    updateSite(openWeatherMapTemp);
//    myVFD->setLowerLine("tampa temp", openWeatherMapTemp.lastResult.getOrElse("no data"));
//    delay(4000);
//
//    updateSite(openWeatherMapHumidity);
//    myVFD->setLowerLine("tampa humidity", openWeatherMapHumidity.lastResult.getOrElse("no data"));
//    delay(4000);
//
////    myVFD->clear();
////    myVFD->home();
////    myVFD->print("sensor temp "); //(char)223)
////    myVFD->println(readTemp(tempsensor));
////    myVFD->print("time "); //(char)223)
//
//    yield();
//}
void loop() {
    myVFD->home();
    for (char c = '\x21'; c < '\x21' + 40; c++) {
        myVFD->print(c);
    }
//    Serial.println(grid.toString().c_str());
//    for (char a = first; a < last; a++) {
//        myVFD->print(a);
//        delay(200);
//    }
    delay(1000);

}
// cd5220
// JsonObject.success()

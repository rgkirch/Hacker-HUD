#include <ESP8266WiFi.h>

#undef min
#undef max

#include <vector>
//#define min(a,b) ((a)<(b)?(a):(b))
//#define max(a,b) ((a)>(b)?(a):(b))
#include <ArduinoJson.h>
#include <Adafruit_MCP9808.h>

#undef min
#undef max

#include <NtpClientLib.h>

#undef min
#undef max

#include <ESP8266HTTPClient.h>

#undef min
#undef max

#include "site.hpp"
#include "wifi.hpp"
#include "display.h"
#include "myConcreteSerial.hpp"
#include "grid.hpp"
#include "apiKeys.hpp"

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
using std::bind;

//string ntpTime;
time_t unixTime;
time_t unixTimeUpdated;
bool callbackEnabled = true;

os_timer_t myTimer;
MyConcretePrint serial(D5, D6);
VFD myVFD(20, 2, &serial);
Adafruit_MCP9808 tempsensor;// = Adafruit_MCP9808(); //for MCP9808
void *memchr(const void *s, int c, size_t n) {
    unsigned char *p = (unsigned char *) s;
    while (n--)
        if (*p != (unsigned char) c)
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
void initializeTemp(Adafruit_MCP9808 &tempsensor) {
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

Option<string>
applyKeys(const JsonObject &o, const vector<string>::iterator begin, const vector<string>::iterator end) {
    LOG("apply keys");
    Option<string> emptyOption;
    auto it = begin;
    if (next(it) == end) {
        const char *r = o[(*it).c_str()].as<const char *>();
        if (r == nullptr) {
            return emptyOption;
        } else return string(r);
    } else {
        return applyKeys(o[(*it).c_str()], next(begin), end);
    }
}

Option<string> getSiteData(uint16_t port, string host, string path, vector<string> keys) {
    HTTPClient http;
    string server;
    port == httpsPort ? server.append("https://") : server.append("http://");
    server.append(host).append(path);
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
    function<Option<string>(std::string)> f([&](string str) -> Option<string> {
        JsonObject &o = jsonBuffer.parseObject(str.c_str());
        if (not o.success()) return Option<string>();
        return applyKeys(o, keys.begin(), keys.end());
    });
    LOG("json shit");
    o.map(f);
    return o;
}

string someFunctionWithHttp(string host, string path, uint16_t port) {
    HTTPClient http;
    string server;
    port == httpsPort ? server.append("https://") : server.append("http://");
    server.append(host).append(path);
    http.begin(server.c_str());
    int httpCode = http.GET();
    LOGN(httpCode);
    LOG(HTTP_CODE_OK);
    string data = http.getString().c_str();
    http.end();
}

Option<string> getSiteData(Site site) {
    string data = someFunctionWithHttp(site.host, site.path, site.port);
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
    function<Option<string>(std::string)> f([&o, &jsonBuffer, &site](string str) -> Option<string> {
        JsonObject &o = jsonBuffer.parseObject(str.c_str());
        if (not o.success()) return Option<string>();
        return applyKeys(o, site.keys.begin(), site.keys.end());
    });
    LOG("json shit");
    o.map(f);
    return o;
}

void updateSite(Site &site) {
    if (site.lastUpdated == 0 or millis() - site.lastUpdated > site.updateInterval) {
        LOG("update site");
        Option<string> o = getSiteData(site);
        site.lastResult = o;
        site.lastUpdated = millis();
    }
}

void timerCallback(void *pArg) {
    if (callbackEnabled) {
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
}

BarGraph grid(100, 14);
Frame *frame;

template<typename T>
struct TimeCache {
    TimeCache(function<T(void)> f, int cacheTime) : f(f), cacheTime(cacheTime) {}
    virtual ~TimeCache() =default;

    T get() {
        if (millis() > lastUpdated + cacheTime) {
            lastUpdated = millis();
            data = f();
        }
        return data;
    }

    T data;
    function<T(void)> f;
    int cacheTime;
    unsigned long lastUpdated;
};

TimeCache<Option<string>> bitcoin([]() {
    auto port = httpPort;
    auto host = "api.coindesk.com";
    auto path = "/v1/bpi/currentprice.json";
    vector<string> keys = {"bpi", "USD", "rate_float"};
    return getSiteData(port, host, path, keys);
}, 60*60*1000);
TimeCache<Option<string>> etherium([]() {
    auto port = httpsPort;
    auto host = "coinmarketcap-nexuist.rhcloud.com";
    auto path = "/api/eth";
    vector<string> keys = {"price", "usd"};
    return getSiteData(port, host, path, keys);
}, 60*60*1000);
TimeCache<Option<string>> openWeatherMapHumidity([]() {
    auto port = httpPort;
    auto host = "api.openweathermap.org";
    auto path = "/data/2.5/weather?q=Tampa,us&units=imperial&APPID=" + openWeatherMapApiKey;
    vector<string> keys = {"main", "humidity"};
    return getSiteData(port, host, path, keys);
}, 60*60*1000);
TimeCache<Option<string>> openWeatherMapTemp([]() {
    auto port = httpPort;
    auto host = "api.openweathermap.org";
    auto path = "/data/2.5/weather?q=Tampa,us&units=imperial&APPID=" + openWeatherMapApiKey;
    vector<string> keys = {"main", "temp"};
    return getSiteData(port, host, path, keys);
}, 60*60*1000);

vector<function<string()>> frames;
int getWhichFrame(int numFrames, long timePerFrame, long currentTime) {
    int currentFrame = (currentTime % (numFrames * timePerFrame)) / timePerFrame;
    return currentFrame;
}
auto whichFrame = [](long currentTime) { return getWhichFrame(4, 4000, currentTime); };

void setup() {
    Serial.begin(115200);
    ntpSetup();
    myVFD.clear();
    myVFD.home();

    // os_timer_setfn(&myTimer, (os_timer_func_t *) timerCallback, NULL);
    // os_timer_arm(&myTimer, 1000, true);

    frames.push_back( [&](){
        return bitcoin.get().orElse("no data");
    });
    frames.push_back( [&](){
        return etherium.get().orElse("no data");
    });
    frames.push_back( [&](){
        return openWeatherMapTemp.get().orElse("no data");
    });
    frames.push_back( [&](){
        return openWeatherMapHumidity.get().orElse("no data");
    });


//    const int stringLength = 20;
//    char endString[stringLength];
//    char startString[stringLength];
//    time_t endTime = 1499516750;
//    time_t startTime = endTime - 100 * 24 * 60 * 60;
////    strftime(startString, stringLength, "%Y-%m-%d", localtime(&t));
//    snprintf(endString, stringLength, "%02d-%02d-%02d", year(endTime), month(endTime), day(endTime));
//    snprintf(startString, stringLength, "%02d-%02d-%02d", year(startTime), month(startTime), day(startTime));
//    const char *urlTemplate = "http://api.coindesk.com/v1/bpi/historical/close.json?start=%s&end=%s";
////    https://etherscan.io/chart/etherprice?output=csv
//    char url[100];
//    snprintf(url, 100, urlTemplate, startString, endString);

//    Serial.println("start time --- end time");
//    Serial.print(startTime);
//    Serial.print(" ");
//    Serial.println(endTime);
//    Serial.print("url is: ");
//    Serial.println(url);

//    HTTPClient http;
//    http.begin(url);
//    delay(300);
//    HTTP_CODE_OK == http.GET() ? Serial.println("http success") : Serial.println("http fail");
//    string data = http.getString().c_str();
////    Serial.println(data.c_str());
//    http.end();

//    string sampleJson {R"cocksucker!({"bpi":{"2017-05-15":1697.3788,"2017-05-16":1718.2013,"2017-05-17":1802.1638,"2017-05-18":1887.3263,"2017-05-19":1968.1025,"2017-05-20":2051.735,"2017-05-21":2055.6175,"2017-05-22":2139.0275,"2017-05-23":2291.4775,"2017-05-24":2476.2963,"2017-05-25":2357.5038,"2017-05-26":2247.4825,"2017-05-27":2106.3075,"2017-05-28":2207.5775,"2017-05-29":2289.87,"2017-05-30":2197.2338,"2017-05-31":2330.2338,"2017-06-01":2452.1813,"2017-06-02":2517.4088,"2017-06-03":2555.6538,"2017-06-04":2552.8088,"2017-06-05":2736.595,"2017-06-06":2914.0825,"2017-06-07":2694.2188,"2017-06-08":2825.0313,"2017-06-09":2826.7,"2017-06-10":2942.345,"2017-06-11":3018.545,"2017-06-12":2682.595,"2017-06-13":2738.9313,"2017-06-14":2494.485,"2017-06-15":2456.9238,"2017-06-16":2528.1025,"2017-06-17":2663.9975,"2017-06-18":2576.1713,"2017-06-19":2641.665,"2017-06-20":2778.8275,"2017-06-21":2712.1575,"2017-06-22":2740.79,"2017-06-23":2738.2138},"disclaimer":"This data was produced from the CoinDesk Bitcoin Price Index. BPI value data returned as USD.","time":{"updated":"Jun 25, 2017 00:03:00 UTC","updatedISO":"2017-06-25T00:03:00+00:00"}})cocksucker!"};
    // string data{
            // R"cocksucker!({"bpi":{"2017-03-17":1070.128,"2017-03-18":970.598,"2017-03-19":1017.8,"2017-03-20":1041.343,"2017-03-21":1115.039,"2017-03-22":1037.44,"2017-03-23":1029.95,"2017-03-24":935.946,"2017-03-25":964.692,"2017-03-26":965.229,"2017-03-27":1040.491,"2017-03-28":1044.251,"2017-03-29":1040.392,"2017-03-30":1037.527,"2017-03-31":1079.748,"2017-04-01":1089.511,"2017-04-02":1098.776,"2017-04-03":1147.631,"2017-04-04":1143.746,"2017-04-05":1134.998,"2017-04-06":1190.597,"2017-04-07":1193.019,"2017-04-08":1184.824,"2017-04-09":1210.055,"2017-04-10":1213.339,"2017-04-11":1224.767,"2017-04-12":1216.505,"2017-04-13":1178.533,"2017-04-14":1183.44,"2017-04-15":1180.699,"2017-04-16":1184.787,"2017-04-17":1203.731,"2017-04-18":1217.596,"2017-04-19":1226.939,"2017-04-20":1255.403,"2017-04-21":1257.135,"2017-04-22":1244.375,"2017-04-23":1248.2175,"2017-04-24":1248.325,"2017-04-25":1263.545,"2017-04-26":1284.845,"2017-04-27":1329.19,"2017-04-28":1320.0513,"2017-04-29":1327.0388,"2017-04-30":1347.9588,"2017-05-01":1402.0838,"2017-05-02":1443.6825,"2017-05-03":1491.9988,"2017-05-04":1515.6288,"2017-05-05":1512.2088,"2017-05-06":1548.2863,"2017-05-07":1555.4713,"2017-05-08":1639.3225,"2017-05-09":1706.9313,"2017-05-10":1756.8025,"2017-05-11":1807.3725,"2017-05-12":1676.9938,"2017-05-13":1759.9613,"2017-05-14":1772.4163,"2017-05-15":1697.3788,"2017-05-16":1718.2013,"2017-05-17":1802.1638,"2017-05-18":1887.3263,"2017-05-19":1968.1025,"2017-05-20":2051.735,"2017-05-21":2055.6175,"2017-05-22":2139.0275,"2017-05-23":2291.4775,"2017-05-24":2476.2963,"2017-05-25":2357.5038,"2017-05-26":2247.4825,"2017-05-27":2106.3075,"2017-05-28":2207.5775,"2017-05-29":2289.87,"2017-05-30":2197.2338,"2017-05-31":2330.2338,"2017-06-01":2452.1813,"2017-06-02":2517.4088,"2017-06-03":2555.6538,"2017-06-04":2552.8088,"2017-06-05":2736.595,"2017-06-06":2914.0825,"2017-06-07":2694.2188,"2017-06-08":2825.0313,"2017-06-09":2826.7,"2017-06-10":2942.345,"2017-06-11":3018.545,"2017-06-12":2682.595,"2017-06-13":2738.9313,"2017-06-14":2494.485,"2017-06-15":2456.9238,"2017-06-16":2528.1025,"2017-06-17":2663.9975,"2017-06-18":2576.1713,"2017-06-19":2641.665,"2017-06-20":2778.8275,"2017-06-21":2712.1575,"2017-06-22":2740.79,"2017-06-23":2738.2138,"2017-06-24":2617.9375},"disclaimer":"This data was produced from the CoinDesk Bitcoin Price Index. BPI value data returned as USD.","time":{"updated":"Jun 25, 2017 00:03:00 UTC","updatedISO":"2017-06-25T00:03:00+00:00"}})cocksucker!"};
    // deleteAll(myVFD);
    // frame = new Frame(normalize(parseThatJsonToDoubles(data), grid.getHeight()));
}

void loop() {
    function<void(char)> g = [&](char c) -> void { myVFD.print(c); };


    int frameTime = 1000;
    if (WiFi.status() != WL_CONNECTED) connectToWifi();
    unixTime = NTP.getTime();
    unixTimeUpdated = millis();

    string str = frames[whichFrame(millis())]();
    Serial.println(str.c_str());
    myVFD.setLowerLine(str);
    delay(500);

    // auto clearFrame = []() -> std::string {
    //     string str;
    //     for (char a = '\x21'; a <= '\x21' + 39; a++) {
    //         str.push_back('\x1B');
    //         str.push_back('\x3F');
    //         str.push_back(a);
    //     }
    //     return str;
    // };
    // callbackEnabled = false;
    // frame->bind(g);
    // myVFD.home();
    // for (char c = '\x21'; c < '\x21' + 40; c++) {
    //     myVFD.print(c);
    // }
    // myVFD.print(clearFrame().c_str());
    // delay(frameTime);
    // myVFD.clear();
    // callbackEnabled = true;
    yield();
}
// cd5220
// JsonObject.success()
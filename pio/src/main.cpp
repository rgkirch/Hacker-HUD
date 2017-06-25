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
#include "grid.hpp"

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
using std::bind;

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

BarGraph grid(20, 2);

vector<string> parseThatJson(string json) {
    vector<string> v{};
    auto i = json.find(':') + 1;
    while(true) {
        auto begin = json.find(':', i+1) + 1;
        auto end = json.find(',', begin);
        if(json[end-1] == '}') {
            v.push_back(json.substr(begin, end-1 - begin));
            break;
        }
        v.push_back(json.substr(begin, end - begin));
        i = end + 1;
    }
    return v;
}

template<class InputIt, class OutputIt, class UnaryOperation>
OutputIt transform(InputIt first1, InputIt last1, OutputIt d_first, UnaryOperation unary_op) {
    while (first1 != last1) {
        *d_first++ = unary_op(*first1++);
    }
    return d_first;
}

template<typename InputIt>
auto maximum(InputIt first, InputIt last)-> decltype(*first) {
    decltype(*first) maximum = *first;
    auto it = first;
    while(it != last) {
        maximum = (*it > maximum ? *it : maximum);
        it++;
    }
    return maximum;
}

void setup()
{
    if(WiFi.status() != WL_CONNECTED) connectToWifi();
    Serial.begin(115200);

//    HTTPClient http;
//    http.begin("http://api.coindesk.com/v1/bpi/historical/close.json?start=2017-03-17&end=2017-06-24");
//    delay(300);
//    HTTP_CODE_OK == http.GET() ? Serial.println("http success") : Serial.println("http fail");
//    string data = http.getString().c_str();
//    Serial.println(data.c_str());
//    http.end();

//    string sampleJson {R"cocksucker!({"bpi":{"2017-05-15":1697.3788,"2017-05-16":1718.2013,"2017-05-17":1802.1638,"2017-05-18":1887.3263,"2017-05-19":1968.1025,"2017-05-20":2051.735,"2017-05-21":2055.6175,"2017-05-22":2139.0275,"2017-05-23":2291.4775,"2017-05-24":2476.2963,"2017-05-25":2357.5038,"2017-05-26":2247.4825,"2017-05-27":2106.3075,"2017-05-28":2207.5775,"2017-05-29":2289.87,"2017-05-30":2197.2338,"2017-05-31":2330.2338,"2017-06-01":2452.1813,"2017-06-02":2517.4088,"2017-06-03":2555.6538,"2017-06-04":2552.8088,"2017-06-05":2736.595,"2017-06-06":2914.0825,"2017-06-07":2694.2188,"2017-06-08":2825.0313,"2017-06-09":2826.7,"2017-06-10":2942.345,"2017-06-11":3018.545,"2017-06-12":2682.595,"2017-06-13":2738.9313,"2017-06-14":2494.485,"2017-06-15":2456.9238,"2017-06-16":2528.1025,"2017-06-17":2663.9975,"2017-06-18":2576.1713,"2017-06-19":2641.665,"2017-06-20":2778.8275,"2017-06-21":2712.1575,"2017-06-22":2740.79,"2017-06-23":2738.2138},"disclaimer":"This data was produced from the CoinDesk Bitcoin Price Index. BPI value data returned as USD.","time":{"updated":"Jun 25, 2017 00:03:00 UTC","updatedISO":"2017-06-25T00:03:00+00:00"}})cocksucker!"};
    string sampleJson {R"cocksucker!({"bpi":{"2017-03-17":1070.128,"2017-03-18":970.598,"2017-03-19":1017.8,"2017-03-20":1041.343,"2017-03-21":1115.039,"2017-03-22":1037.44,"2017-03-23":1029.95,"2017-03-24":935.946,"2017-03-25":964.692,"2017-03-26":965.229,"2017-03-27":1040.491,"2017-03-28":1044.251,"2017-03-29":1040.392,"2017-03-30":1037.527,"2017-03-31":1079.748,"2017-04-01":1089.511,"2017-04-02":1098.776,"2017-04-03":1147.631,"2017-04-04":1143.746,"2017-04-05":1134.998,"2017-04-06":1190.597,"2017-04-07":1193.019,"2017-04-08":1184.824,"2017-04-09":1210.055,"2017-04-10":1213.339,"2017-04-11":1224.767,"2017-04-12":1216.505,"2017-04-13":1178.533,"2017-04-14":1183.44,"2017-04-15":1180.699,"2017-04-16":1184.787,"2017-04-17":1203.731,"2017-04-18":1217.596,"2017-04-19":1226.939,"2017-04-20":1255.403,"2017-04-21":1257.135,"2017-04-22":1244.375,"2017-04-23":1248.2175,"2017-04-24":1248.325,"2017-04-25":1263.545,"2017-04-26":1284.845,"2017-04-27":1329.19,"2017-04-28":1320.0513,"2017-04-29":1327.0388,"2017-04-30":1347.9588,"2017-05-01":1402.0838,"2017-05-02":1443.6825,"2017-05-03":1491.9988,"2017-05-04":1515.6288,"2017-05-05":1512.2088,"2017-05-06":1548.2863,"2017-05-07":1555.4713,"2017-05-08":1639.3225,"2017-05-09":1706.9313,"2017-05-10":1756.8025,"2017-05-11":1807.3725,"2017-05-12":1676.9938,"2017-05-13":1759.9613,"2017-05-14":1772.4163,"2017-05-15":1697.3788,"2017-05-16":1718.2013,"2017-05-17":1802.1638,"2017-05-18":1887.3263,"2017-05-19":1968.1025,"2017-05-20":2051.735,"2017-05-21":2055.6175,"2017-05-22":2139.0275,"2017-05-23":2291.4775,"2017-05-24":2476.2963,"2017-05-25":2357.5038,"2017-05-26":2247.4825,"2017-05-27":2106.3075,"2017-05-28":2207.5775,"2017-05-29":2289.87,"2017-05-30":2197.2338,"2017-05-31":2330.2338,"2017-06-01":2452.1813,"2017-06-02":2517.4088,"2017-06-03":2555.6538,"2017-06-04":2552.8088,"2017-06-05":2736.595,"2017-06-06":2914.0825,"2017-06-07":2694.2188,"2017-06-08":2825.0313,"2017-06-09":2826.7,"2017-06-10":2942.345,"2017-06-11":3018.545,"2017-06-12":2682.595,"2017-06-13":2738.9313,"2017-06-14":2494.485,"2017-06-15":2456.9238,"2017-06-16":2528.1025,"2017-06-17":2663.9975,"2017-06-18":2576.1713,"2017-06-19":2641.665,"2017-06-20":2778.8275,"2017-06-21":2712.1575,"2017-06-22":2740.79,"2017-06-23":2738.2138,"2017-06-24":2617.9375},"disclaimer":"This data was produced from the CoinDesk Bitcoin Price Index. BPI value data returned as USD.","time":{"updated":"Jun 25, 2017 00:03:00 UTC","updatedISO":"2017-06-25T00:03:00+00:00"}})cocksucker!"};
    auto parsed = parseThatJson(sampleJson);
    vector<double> converted{};
    converted.reserve(parsed.size());
    for (auto x : parsed) {
        converted.push_back(atof(x.c_str()));
    }
    for (auto d : converted) {
        Serial.print(d);
        Serial.print(" ");
    }
    Serial.println();

    auto max = maximum(begin(converted), end(converted));
    for (auto& x : converted) {
        x = (x / max) * 7.0;
    }

    for (auto d : converted) {
        Serial.print(d);
        Serial.print(" ");
    }
    Serial.println();

    Serial.print("length of converted ");
    Serial.println(converted.size());
    vector<int> vec{};
    for (auto x : converted) {
        vec.push_back(floor(x));
    }

    Serial.print("length of vec ");
    Serial.println(vec.size());
    for (auto d : vec) {
        Serial.print(d);
        Serial.print(" ");
    }
    Serial.println();


//    const size_t bufferSize = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(40) + 1130;
//    DynamicJsonBuffer jsonBuffer(bufferSize);
//    JsonObject &o = jsonBuffer.parseObject(sampleJson.c_str());
//    auto x = o["bpi"].as<const char*>();
//    Serial.print("printing first value");
//    Serial.println(x);

    deleteAll(myVFD);
    myVFD.clear();
    myVFD.home();
    function<void(char)> g = bind([](VFD& vfd, char c)->void { myVFD.print(c); }, myVFD, std::placeholders::_1);

    auto chars = grid.toChars(vec);
    vector<char> tempForPadding{};
    tempForPadding.resize(100);
    tempForPadding.insert(begin(tempForPadding), begin(chars), end(chars));
    auto msg = grid.set(tempForPadding, g);
    Serial.println(msg.c_str());
}
//void p(const char *cs)
//{
//    myVFD.print(cs);
//}
void loop()
{
    myVFD.home();
    for (char c = '\x21'; c < '\x21' + 40; c++) {
        myVFD.print(c);
    }
    delay(500);

    yield();
}
// cd5220
// JsonObject.success()
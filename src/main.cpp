#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <json.hpp>
#include "site.hpp"

//#include <Adafruit_MCP9808.h>
//#include <Wire.h>

#include "../lib/vfd/vfd.hpp"
#include "../lib/wifi/wifi.hpp"
#include "globals.hpp"
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

std::unique_ptr<VFD> myVFD;
struct Site coindesk {.host = "api.coindesk.com", .path = "v1/bpi/currentprice.json", .port = httpsPort};

void setup()
{
    Serial.begin(115200);
    myVFD = std::unique_ptr<VFD>(VFD::Builder().setRx(D5).setTx(D6).setDisplayWidth(20).setDisplayHeight(2).build());
    myVFD->home();
    myVFD->clear();
}

void loop()
{
    if(WiFi.status() != WL_CONNECTED) connectToWifi();
    myVFD->home();
}
// cd5220

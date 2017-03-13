#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <json.hpp>

//#include <Adafruit_MCP9808.h>
//#include <Wire.h>

#include "globals.hpp"
#include "site.hpp"
#include "../lib/vfd/vfd.hpp"
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

VFD *myVFD = VFD::Builder().setRx(D5).setTx(D6).setDisplayWidth(20).setDisplayHeight(2).build();
struct Site coindesk {.host = "api.coindesk.com", .path = "v1/bpi/currentprice.json", .port = httpsPort};

void setup()
{
    Serial.begin(115200);
    myVFD->home();
    myVFD->clear();
}

void loop()
{
    if(WiFi.status() != WL_CONNECTED) connectToWifi();
    myVFD->home();
}
// cd5220

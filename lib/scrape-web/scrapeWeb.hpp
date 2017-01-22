#ifndef SCRAPEWEB_HPP
#define SCRAPEWEB_HPP

#include <WiFiClient.h>
#include <ESP8266WiFi.h>

//const char* host = "api.coindesk.com";
//const char* hostEth = "api.nanopool.org";
//const char* hostTime = "script.google.com";

//struct jsonThing {
//    const String keyword;
//    int begin;
//    int end;
//};
//
//struct Site {
//    const char* url;
//    const char* host;
//    bool secure;
//};

char* get(const char* host, const char* path);

#endif

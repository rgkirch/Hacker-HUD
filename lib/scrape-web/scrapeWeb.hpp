#ifndef SCRAPEWEB_HPP
#define SCRAPEWEB_HPP

#include <WiFiClient.h>
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


//const char* host = "api.coindesk.com";
//const char* hostEth = "api.nanopool.org";
//const char* hostTime = "script.google.com";

struct jsonThing {
    const String keyword;
    int begin;
    int end;
};

struct Site {
    const char* url;
    const char* host;
    bool secure;
};

std::vector<char> makeGetRequest(std::vector<char> host, std::vector<char> path);
int getJsonValue(const bool secureClient, std::vector<char> host, std::vector<char> path, const char* key);

#endif

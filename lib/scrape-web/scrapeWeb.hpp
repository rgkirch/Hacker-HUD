#ifndef SCRAPEWEB_HPP
#define SCRAPEWEB_HPP

#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include "site.hpp"
//
//#ifdef min
//#undef min
//#endif
//#ifdef max
//#undef max
//#endif
//#include <vector>
//#define min(a,b) ((a)<(b)?(a):(b))
//#define max(a,b) ((a)>(b)?(a):(b))

//const char* host = "api.coindesk.com";
//const char* hostEth = "api.nanopool.org";
//const char* hostTime = "script.google.com";

std::string parseJson(std::string text, std::string key);
std::string makeGetRequest(std::string host, std::string path);
std::string getJsonValue(const Site& client, std::string key);
std::string get(std::string host, std::string path, bool secure);

#endif

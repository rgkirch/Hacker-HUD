#ifndef SCRAPEWEB_HPP
#define SCRAPEWEB_HPP

#include <WiFiClient.h>
#include <ESP8266WiFi.h>
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

class Site {
public:
    class Builder;
private:
    Site(const std::string &host, const std::string &path, const WiFiClient& client) : host(host), path(path), client(client) {}
    const std::string host;
    const std::string path;
    const WiFiClient client;
};

class Site::Builder {
private:
    std::string host = std::string("");
    std::string path = std::string("");
    bool secure = false;
public:
    Site build();
    Builder() = default;
    Site::Builder& setHost(const std::string &host);
    Site::Builder& setPath(const std::string &path);
    Site::Builder& setSecure(const bool secure);
};

std::string parseJson(std::string text, std::string key);
std::string makeGetRequest(std::string host, std::string path);
std::string getJsonValue(const bool secureClient, std::string host, std::string path, std::string key);

#endif

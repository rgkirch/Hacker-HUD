#ifndef SCRAPEWEB_HPP
#define SCRAPEWEB_HPP

#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <ESP8266WiFi.h>
#include <string>
#include <ArduinoJson.h>

#include "option.hpp"
#include "site.hpp"
//#ifdef min
//#undef min
//#endif
//#ifdef max
//#undef max
//#endif
//#include <vector>
//#define min(a,b) ((a)<(b)?(a):(b))
//#define max(a,b) ((a)>(b)?(a):(b))

std::string makeGetRequest(const char *host, const char *path)
{
    std::string request;
    std::string get = {"GET /"};
    std::string http = {" HTTP/1.1\r\nHost: "};
    std::string close = {"\r\nConnection: close\r\n\r\n"};
    request.append(get);
    if (path != nullptr) request.append(path);
    request.append(http);
    if (host != nullptr) request.append(host);
    request.append(close);
    return request;
}
Option<std::string> downloadSiteData(int port, const char *host, const char *path) {
    Option<std::string> emptyOption;
    WiFiClient* client;
    if(port == httpsPort)
    {
        client = new WiFiClientSecure;
    } else if (port == httpPort){
        client = new WiFiClient;
    } else return emptyOption; //"port incorrect"
    if (client == NULL or client == nullptr) return emptyOption; //"couldn't make client"

    if (not client->connect(host, port)) {
        return emptyOption; //"client connect failed"
    }
    if (not client->connected()) {
        return emptyOption; //"client not connected?!?!"
    }
    client->print(makeGetRequest(host, path).c_str());

    unsigned long timeout = millis();
    while (!client->available()) {
        yield();
        if (millis() - timeout > 5000) {
            client->stop();
            return emptyOption; //"client timed out"
        }
    }

    std::string data;
    while(client->connected())
    {
        data.append(client->readString().c_str());
    }
    auto i = data.find('{');
    if (i == data.npos) {
        i = 0;
    }
    client->stop();
    delete client;
    return data.substr(i);
}

#endif

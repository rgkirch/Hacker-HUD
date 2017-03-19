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
Option<std::string> scrapeJson(Site site) {
    WiFiClient* client;
    if(site.port == httpsPort)
    {
        client = new WiFiClientSecure;
    } else if (site.port == httpPort){
        client = new WiFiClient;
    } else return Option<std::string>(); //"site port incorrect"
    if (client == nullptr) return Option<std::string>(); //"couldn't make client"

    if (not client->connect(site.host, site.port)) {
        return Option<std::string>(); //"client connect failed"
    }
    if (not client->connected()) {
        return Option<std::string>(); //"client not connected?!?!"
    }
    client->print(makeGetRequest(site.host, site.path).c_str());

    unsigned long timeout = millis();
    while (!client->available()) {
        yield();
        if (millis() - timeout > 5000) {
            client->stop();
            return Option<std::string>(); //"client timed out"
        }
    }

    // Read all the lines of the reply from server and print them to Serial
    std::string data;
    while(client->connected())
    {
        data.append(client->readString().c_str());
    }
    auto i = data.rfind('\n');
    if (i != data.npos)
    {
        data.erase(0, i + 1);
    }
    client->stop();
    delete client;
    return data;
}
Option<std::string> scrapeSite(Site site) {
    WiFiClient* client;
    if(site.port == httpsPort)
    {
        client = new WiFiClientSecure;
    } else if (site.port == httpPort){
        client = new WiFiClient;
    } else return Option<std::string>(); // "site port incorrect"
    if (client == NULL) return Option<std::string>(); //"couldn't make client"

    if (not client->connect(site.host, site.port)) {
        return Option<std::string>(); //"client connect failed"
    }
    if (not client->connected()) {
        return Option<std::string>(); //"client not connected?!?!"
    }
    client->print(makeGetRequest(site.host, site.path).c_str());
    unsigned long timeout = millis();
    while (!client->available()) {
        yield();
        if (millis() - timeout > 5000) {
            client->stop();
            return Option<std::string>(); //"client timed out"
        }
    }

    // Read all the lines of the reply from server and print them to Serial
    while(client->connected())
    {
        int numBytes = client->available();
        Serial.println(client->readStringUntil('\r'));
    }

    client->stop();
    delete client;
    return Option<std::string>();
}
//std::string makeGetRequest(std::string host, std::string path)
//{
//    std::string request;
////    request.reserve(one.size() + two.size() + three.size() + host.size() + path.size());
//    std::string get = {"GET /"};
//    std::string http = {" HTTP/1.1\r\nHost: "};
////    std::string userAgent = {"\r\nUser-Agent: rgkirch"};
//    std::string close = {"\r\nConnection: close\r\n\r\n"};
//    request.append(get).append(path).append(http).append(host)/*.append(userAgent)*/.append(close);
//    return request;
//}

#endif

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
Option<std::string> scrapeJson(struct Site site) {
    Option<std::string> emptyOption;
    WiFiClient* client;
    if(site.port == httpsPort)
    {
        client = new WiFiClientSecure;
    } else if (site.port == httpPort){
        client = new WiFiClient;
    } else return emptyOption; //"site port incorrect"
    if (client == NULL or client == nullptr) return emptyOption; //"couldn't make client"

    if (not client->connect(site.host, site.port)) {
        return emptyOption; //"client connect failed"
    }
    if (not client->connected()) {
        return emptyOption; //"client not connected?!?!"
    }
    client->print(makeGetRequest(site.host, site.path).c_str());

    unsigned long timeout = millis();
    while (!client->available()) {
        yield();
        if (millis() - timeout > 5000) {
            client->stop();
            return emptyOption; //"client timed out"
        }
    }

    // Read all the lines of the reply from server and print them to Serial
    std::string data;
    while(client->connected())
    {
        data.append(client->readString().c_str());
    }
    auto i = data.find('{');
    if (i != data.npos)
    {
//        data.erase(0, i);
//        i += 2;
    } else i = 0;
//    Serial.print("i is ");
//    Serial.println(i);
    client->stop();
    delete client;
    emptyOption = data.substr(i);
    return emptyOption;
}
Option<std::string>* scrapeSite(struct Site site) {
    WiFiClient* client;
    if(site.port == httpsPort)
    {
        client = new WiFiClientSecure;
    } else if (site.port == httpPort){
        client = new WiFiClient;
    } else return new Option<std::string>; // "site port incorrect"
    if (client == NULL) return new Option<std::string>; //"couldn't make client"

    if (not client->connect(site.host, site.port)) {
        return new Option<std::string>; //"client connect failed"
    }
    if (not client->connected()) {
        return new Option<std::string>; //"client not connected?!?!"
    }
    client->print(makeGetRequest(site.host, site.path).c_str());
    unsigned long timeout = millis();
    while (!client->available()) {
        yield();
        if (millis() - timeout > 5000) {
            client->stop();
            return new Option<std::string>; //"client timed out"
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
    return new Option<std::string>;
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

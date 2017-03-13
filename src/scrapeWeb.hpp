#ifndef SCRAPEWEB_HPP
#define SCRAPEWEB_HPP

#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <ESP8266WiFi.h>
#include <string>
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
    if (path != nullptr) request.append(path)
    request.append(http)
    if (host != nullptr) request.append(host)
    request.append(close);
    return request;

}
Option<std::string> scrapeSite(Site site) {
    WiFiClient* client;
    if(site.port == httpsPort)
    {
        client = new WiFiClientSecure;
    } else if (site.port == httpPort){
        client = new WiFiClient;
    } else return Option::error("site port incorrect");
    if (client == NULL) return Option::error("couldn't make client");

    if (not client->connect(site.host, site.port)) {
        return Option::error("client connect failed");
    }
    if (not client->connected()) {
        return Option::error("client not connected?!?!");
    }
    client->print(makeGetRequest(site.host, site.path));
    unsigned long timeout = millis();
    while (!client->available()) {
        yield();
        if (millis() - timeout > 5000) {
            client->stop();
            return Option::error("client timed out");
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
    return Option<std::string>("");
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

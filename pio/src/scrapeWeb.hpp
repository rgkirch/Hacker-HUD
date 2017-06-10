#ifndef SCRAPEWEB_HPP
#define SCRAPEWEB_HPP

#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <ESP8266WiFi.h>
#include <string>
#include <ArduinoJson.h>

#include "option.hpp"
#include "site.hpp"

//#include "myConnection.hpp"
//#ifdef min
//#undef min
//#endif
//#ifdef max
//#undef max
//#endif
//#include <vector>
//#define min(a,b) ((a)<(b)?(a):(b))
//#define max(a,b) ((a)>(b)?(a):(b))

//#define DEBUGPRINT
#ifdef DEBUGPRINT
#define LOG(x) do{Serial.println(x);}while(0)
#else
#define LOG(x)
#endif

Option<std::string> emptyOption;

std::string makeGetRequest(std::string host, std::string path)
{
    LOG("make get request string overload");
    std::string request;
    std::string get = {"GET /"};
    std::string http = {" HTTP/1.1\r\nHost: "};
    std::string close = {"\r\nConnection: close\r\n\r\n"};
    request.append(get);
    request.append(path);
    request.append(http);
    request.append(host);
    request.append(close);
    return request;
}

std::string makeGetRequest(const char *host, const char *path)
{
    LOG("make get request pointer overload");
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

WiFiClient* createWificlient(int port, const char *host, const char *path) {
    LOG("create wifi client");
    WiFiClient* client;
    if(port == httpsPort) {
        client = new WiFiClientSecure();
    } else if (port == httpPort){
        client = new WiFiClient();
    } else return nullptr; //"port incorrect"

    if (not client->connect(host, port)) {
        return nullptr; //"client connect failed"
    }
    if (not client->connected()) {
        return nullptr; //"client not connected?!?!"
    }
    client->print(makeGetRequest(host, path).c_str());
    LOG("exit create wifi client");
    return client;
}


Option<std::string> downloadSiteData(int port, const char *host, const char *path) {
    LOG("download site data");
    WiFiClient *client = createWificlient(port, host, path);
    if(client == nullptr) {
        LOG("client returned from create wificlient is null");
        return emptyOption;
    }
    delay(500); // actually helps
    std::string data;
    while(client->available())
    {
        LOG("read data from client");
        data.append(client->readString().c_str());
        LOG("after read data from client");
    }
    LOG("find the left brace");
    auto i = data.find('{');
    if (i == data.npos) {
        LOG("didn't find the left brace");
        i = 0;
    }
    LOG("stop client");
    client->stop();
    LOG("delete client");
    delete client;
    LOG("exit download site data");
    return data.substr(i);
}

#endif

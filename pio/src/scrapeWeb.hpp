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

Option<std::string> emptyOption;

std::string makeGetRequest(std::string host, std::string path)
{
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

Client* createWificlient(int port, const char *host, const char *path) {
    Client* client;
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
    return client;
}


Option<std::string> downloadSiteData(int port, const char *host, const char *path) {
    Client *client = createWificlient(port, host, path);
    if(client == nullptr) {
        return emptyOption;
    }
    delay(1000); // actually helps
    std::string data;
    for(int read = 0; (read = client->read()) > -1; data.push_back(static_cast<char>(read)));
//    for(char c; client->readBytes(&c, 1) > -1; data.push_back(c));
//    for(uint8_t c; client->read(&c, 1) > -1; data.push_back(c));
    auto i = data.find('{');
    if (i == data.npos) {
        i = 0;
    }
    client->stop();
    delete client;
    return data.substr(i);
}

#endif

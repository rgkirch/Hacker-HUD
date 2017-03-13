#ifndef SCRAPEWEB_HPP
#define SCRAPEWEB_HPP

#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <ESP8266WiFi.h>
#include <string>
#include "option.hpp"
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

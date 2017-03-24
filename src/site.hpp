#ifndef SITE_HPP
#define SITE_HPP
#include <ArduinoJson.h>
#include <string>
//#include <functional>
#include <vector>
#include "site.hpp"
#include "option.hpp"

const int httpPort = 80;
const int httpsPort = 443;

typedef struct {
    int updateInterval;
    int lastUpdated;
    int port;
    const char* host;
    const char* path;
    Option<std::string> *lastResult;
    std::vector<std::string> keys;
//    typedef <typename T>
//    float(*f)(JsonObject);
} Site;

#endif

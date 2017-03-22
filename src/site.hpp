#ifndef SITE_HPP
#define SITE_HPP
#include <ArduinoJson.h>
#include <string>
//#include <functional>
#include <vector>
#include "site.hpp"

const int httpPort = 80;
const int httpsPort = 443;

typedef struct {
    const char* host;
    const char* path;
    int port;
    std::vector<std::string> keys;
//    typedef <typename T>
//    float(*f)(JsonObject);
} Site;

#endif
